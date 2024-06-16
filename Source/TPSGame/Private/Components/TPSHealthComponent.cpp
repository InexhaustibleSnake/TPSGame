// This project is made for a test assignment

#include "Components/TPSHealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "Logic/GameModes/TPSGameModeDeathMatch.h"

UTPSHealthComponent::UTPSHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    SetIsReplicatedByDefault(true);
}

void UTPSHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    if (GetOwner())
    {
        GetOwner()->OnTakePointDamage.AddDynamic(this, &UTPSHealthComponent::OnTakePointDamage);
        GetOwner()->OnTakeRadialDamage.AddDynamic(this, &UTPSHealthComponent::OnTakeRadialDamage);
    }
}

void UTPSHealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation,
    class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType,
    AActor* DamageCauser)
{
    const auto FinalDamage = Damage * GetPointDamageModifier(DamagedActor, BoneName);

    ApplyDamage(FinalDamage, InstigatedBy);
}

void UTPSHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin,
    const FHitResult& HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
    ApplyDamage(Damage, InstigatedBy);
}

float UTPSHealthComponent::GetPointDamageModifier(AActor* DamagedActor, const FName& BoneName) const
{
    const auto Character = Cast<ACharacter>(DamagedActor);
    if (!Character || !Character->GetMesh() || !Character->GetMesh()->GetBodyInstance(BoneName)) return 1.0f;

    const auto PhysMaterial = Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
    if (!PhysMaterial || !DamageModifiers.Contains(PhysMaterial)) return 1.0f;

    return DamageModifiers[PhysMaterial];
}

AController* UTPSHealthComponent::GetOwnerController() const
{
    const auto OwnerPawn = Cast<APawn>(GetOwner());

    return OwnerPawn ? OwnerPawn->GetController() : nullptr;
}

void UTPSHealthComponent::ApplyDamage(float Damage, AController* InstigatedBy)
{
    if (Damage <= 0.0f  || IsDead() || !GetWorld()) return;

    SetHealth(Health - Damage);

    if (IsDead())
    {
        OnDeath.Broadcast();

        const auto GameMode = Cast<ATPSGameModeDeathMatch>(GetWorld()->GetAuthGameMode());
        if (!GameMode) return;

        GameMode->OnPlayerKilled(InstigatedBy, GetOwnerController());
    }
}

void UTPSHealthComponent::SetHealth(float NewHealth)
{
    const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    const auto HealthDelta = NextHealth - Health;

    Health = NextHealth;
    OnRep_Health();

    if (IsDead())
    {
        Dead = true;
        OnRep_Dead();
    }
}

void UTPSHealthComponent::OnRep_Health()
{
    OnHealthChanged.Broadcast(GetHealth(), GetHealthPercent());
}

void UTPSHealthComponent::OnRep_Dead()
{
    const auto OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter || !OwnerCharacter->GetMesh()) return;

    OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    OwnerCharacter->GetMesh()->SetSimulatePhysics(true);
}

void UTPSHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION(UTPSHealthComponent, Health, COND_OwnerOnly);
    DOREPLIFETIME(UTPSHealthComponent, Dead);
}
