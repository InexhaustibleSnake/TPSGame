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
    EquipWeapon(0);
}

void UTPSWeaponComponent::NextWeapon() 
{
    int32 CurrentWeaponIndex = CurrentWeapon ? SpawnedWeapons.IndexOfByKey(CurrentWeapon) : 0;

    EquipWeapon((CurrentWeaponIndex + 1) % SpawnedWeapons.Num());
}

void UTPSWeaponComponent::InitWeapons()
{
    if (!GetWorld()) return;

    for (auto OneWeapon : AvaibleWeaponsClasses)
    {
        auto SpawnedWeapon = GetWorld()->SpawnActor<ATPSBaseWeapon>(OneWeapon);
        if (!SpawnedWeapon) continue;

        SpawnedWeapon->SetOwner(GetOwner());

        SpawnedWeapon->AttachToComponent(GetOwnerMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSpineSocketName);
        SpawnedWeapons.AddUnique(SpawnedWeapon);
    }
}

void UTPSWeaponComponent::EquipWeapon(const int32 WeaponIndex)
{
    if (!GetOwner() || !SpawnedWeapons.IsValidIndex(WeaponIndex) || SpawnedWeapons[WeaponIndex] == CurrentWeapon) return;

    if (!GetOwner()->HasAuthority())
    {
        ServerEquipWeapon(WeaponIndex);
    }

    ATPSBaseWeapon* LocalPreviousWeapon = CurrentWeapon;

    CurrentWeapon = SpawnedWeapons[WeaponIndex];
    OnRep_CurrentWeapon(LocalPreviousWeapon);
}

void UTPSWeaponComponent::ServerEquipWeapon_Implementation(const int32 WeaponIndex)
{
    EquipWeapon(WeaponIndex);
}

void UTPSWeaponComponent::OnRep_CurrentWeapon(ATPSBaseWeapon* PreviousWeapon)
{
    if (PreviousWeapon)
    {
        AttachWeaponToMesh(PreviousWeapon, WeaponSpineSocketName);
    }

    if (CurrentWeapon)
    {
        AttachWeaponToMesh(CurrentWeapon, WeaponSocketName);
    }

    auto PlayerCharacter = Cast<ACharacter>(GetOwner());
    if (!PlayerCharacter) return;

    PlayerCharacter->PlayAnimMontage(EquipMontage);
}

void UTPSWeaponComponent::AttachWeaponToMesh(ATPSBaseWeapon* Weapon, const FName SocketName)
{
    if (!Weapon) return;

    Weapon->AttachToComponent(GetOwnerMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
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
