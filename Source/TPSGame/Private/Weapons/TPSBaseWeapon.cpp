// This project is made for a test assignment

#include "Weapons/TPSBaseWeapon.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

ATPSBaseWeapon::ATPSBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);

    bReplicates = true;

    NetUpdateFrequency = 15.0f;
}

void ATPSBaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    CurrentAmmo = DefaultAmmoData;
}

void ATPSBaseWeapon::MakeShot() {}

void ATPSBaseWeapon::StartFire()
{
    if (!HasAuthority())
    {
        ServerStartFire();
    }
}

void ATPSBaseWeapon::ServerStartFire_Implementation()
{
    StartFire();
}

void ATPSBaseWeapon::StopFire()
{
    if (!HasAuthority())
    {
        ServerStopFire();
    }
}

void ATPSBaseWeapon::ServerStopFire_Implementation()
{
    StopFire();
}

void ATPSBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    if (!GetWorld()) return;

    FCollisionQueryParams CollisionParams;
    CollisionParams.bReturnPhysicalMaterial = true;
    CollisionParams.AddIgnoredActor(GetOwner());

    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

void ATPSBaseWeapon::DecreaseAmmo()
{
    if (!HasAuthority() || CurrentAmmo.Bullets == 0) return;

    --CurrentAmmo.Bullets;

    if (IsClipEmpty() && !IsAmmoEmpty())
    {
        StopFire();
    }
}

void ATPSBaseWeapon::ChangeClip()
{
    if (!CurrentAmmo.InfiniteAmmo && CurrentAmmo.Clips != 0)
    { 
        --CurrentAmmo.Clips;
    }
    CurrentAmmo.Bullets = DefaultAmmoData.Bullets;
}

AController* ATPSBaseWeapon::GetOwnerController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}

bool ATPSBaseWeapon::IsAmmoEmpty() const
{
    return !CurrentAmmo.InfiniteAmmo && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ATPSBaseWeapon::IsClipFull() const
{
    return CurrentAmmo.Bullets == DefaultAmmoData.Bullets;
}

bool ATPSBaseWeapon::IsClipEmpty() const
{
    return CurrentAmmo.Bullets == 0;
}

bool ATPSBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter) return false;

    const auto Controller = OwnerCharacter->GetController<APlayerController>();
    if (!Controller) return false;

    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

    return true;
}

bool ATPSBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;

    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    TraceStart = ViewLocation;

    const FVector ShootDirection = ViewRotation.Vector();

    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

    return true;
}

FVector ATPSBaseWeapon::GetMuzzleWorldLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

void ATPSBaseWeapon::OnRep_CurrentShot() {}

UNiagaraComponent* ATPSBaseWeapon::SpawnMuzzleFX()
{
    return UNiagaraFunctionLibrary::SpawnSystemAttached(
        MuzzleFX, WeaponMesh, MuzzleSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
}

void ATPSBaseWeapon::SetCurrentShot(int32 Amount)
{
    CurrentShot = Amount;
    OnRep_CurrentShot();
}

void ATPSBaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION(ATPSBaseWeapon, CurrentAmmo, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(ATPSBaseWeapon, CurrentShot, COND_SkipOwner);
}
