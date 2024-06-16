// This project is made for a test assignment

#include "Components/TPSWeaponComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapons/TPSBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"

UTPSWeaponComponent::UTPSWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    SetIsReplicatedByDefault(true);
}

void UTPSWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    if (GetOwner() && GetOwner()->HasAuthority())
    {
        InitWeapons();
    }

    EquipWeapon(0);
}

void UTPSWeaponComponent::NextWeapon()
{
    int32 CurrentWeaponIndex = CurrentWeapon ? SpawnedWeapons.IndexOfByKey(CurrentWeapon) : 0;

    int32 WeaponToEquipIndex = (CurrentWeaponIndex + 1) % SpawnedWeapons.Num();

    EquipWeapon((CurrentWeaponIndex + 1) % SpawnedWeapons.Num());
}

void UTPSWeaponComponent::StartFire()
{
    if (!CurrentWeapon || !CanFire()) return;

    CurrentWeapon->StartFire();
}

void UTPSWeaponComponent::StopFire()
{
    if (!CurrentWeapon) return;

    CurrentWeapon->StopFire();
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

        SpawnedWeapon->OnAmmoEmpty.BindUObject(this, &UTPSWeaponComponent::Reload);
    }
}

void UTPSWeaponComponent::Reload()
{
    if (!CurrentWeapon || CurrentWeapon->IsAmmoEmpty() || CurrentWeapon->IsClipFull()) return;

    if (!GetOwner()->HasAuthority())
    {
        ServerReload();

        return;
    }

    SetIsReloading(true);
}

void UTPSWeaponComponent::ServerReload_Implementation()
{
    Reload();
}

void UTPSWeaponComponent::PlayReloadMontage()
{
    if (!CurrentWeapon) return;

    auto PlayerCharacter = Cast<ACharacter>(GetOwner());
    if (!PlayerCharacter) return;

    if (!ReloadMontageData.Contains(CurrentWeapon.GetClass())) return;

    const auto MontageToPlay = ReloadMontageData[CurrentWeapon.GetClass()];

    if (!MontageToPlay) return;

    PlayerCharacter->PlayAnimMontage(MontageToPlay);

    if (!GetOwner()->HasAuthority()) return;

    GetWorld()->GetTimerManager().SetTimer(
        ReloadTimer, this, &UTPSWeaponComponent::OnReloadFinished, MontageToPlay->GetPlayLength(), false);
}

void UTPSWeaponComponent::SetIsReloading(bool IsReloading)
{
    Reloading = IsReloading;
    OnRep_Reloading();
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

void UTPSWeaponComponent::OnRep_Reloading()
{
    if (Reloading)
    {
        PlayReloadMontage();
    }
}

void UTPSWeaponComponent::OnReloadFinished()
{
    SetIsReloading(false);

    if (!Reloading && CurrentWeapon)
    {
        CurrentWeapon->ChangeClip();
    }
}

void UTPSWeaponComponent::AttachWeaponToMesh(ATPSBaseWeapon* Weapon, const FName SocketName)
{
    if (!Weapon) return;

    Weapon->AttachToComponent(GetOwnerMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
}

bool UTPSWeaponComponent::CanFire() const
{
    return CurrentWeapon && !Reloading;
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

void UTPSWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon = nullptr;
    for (auto Weapon : SpawnedWeapons)
    {
        if (!Weapon) continue;

        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    SpawnedWeapons.Empty();

    Super::EndPlay(EndPlayReason);
}

void UTPSWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UTPSWeaponComponent, CurrentWeapon);
    DOREPLIFETIME(UTPSWeaponComponent, Reloading);
    DOREPLIFETIME_CONDITION(UTPSWeaponComponent, SpawnedWeapons, COND_OwnerOnly);
}
