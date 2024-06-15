// This project is made for a test assignment

#include "Components/TPSWeaponComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapons/TPSBaseWeapon.h"
#include "GameFramework/Character.h"

UTPSWeaponComponent::UTPSWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    SetIsReplicatedByDefault(true);
}

void UTPSWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    InitWeapons();
}

void UTPSWeaponComponent::InitWeapons()
{
    if (!GetWorld()) return;

    for (auto OneWeapon : AvaibleWeaponsClasses)
    {
        if (!OneWeapon) continue;

        auto SpawnedWeapon = GetWorld()->SpawnActorDeferred<ATPSBaseWeapon>(OneWeapon, GetSocketTransform(WeaponSpineSocket), GetOwner());
        if (!SpawnedWeapon) continue;

        SpawnedWeapon->AttachToComponent(GetOwnerMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSpineSocket);
        SpawnedWeapons.AddUnique(SpawnedWeapon);
    }
}

FTransform UTPSWeaponComponent::GetSocketTransform(const FName SocketName) const
{
    return GetOwnerMesh() ? GetOwnerMesh()->GetSocketTransform(SocketName) : FTransform();
}

TObjectPtr<USkeletalMeshComponent> UTPSWeaponComponent::GetOwnerMesh() const
{
    const auto OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter) return nullptr;

    return OwnerCharacter->GetMesh();
}

void UTPSWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION(UTPSWeaponComponent, CurrentWeapon, COND_SkipOwner);
}
