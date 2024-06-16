// This project is made for a test assignment

#include "Weapons/TPSRifleWeapon.h"
#include "Engine/DamageEvents.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "NiagaraComponent.h"

void ATPSRifleWeapon::StartFire()
{
    Super::StartFire();

    GetWorldTimerManager().SetTimer(WeaponShotTimer, this, &ATPSRifleWeapon::MakeShot, TimeBetweenShots, true);

    MakeShot();
}

void ATPSRifleWeapon::StopFire()
{
    Super::StopFire();

    GetWorldTimerManager().ClearTimer(WeaponShotTimer);

    SetCurrentShot(0);
}

void ATPSRifleWeapon::MakeShot()
{
    if (!GetWorld() || IsAmmoEmpty())
    {
        StopFire();
        OnAmmoEmpty.ExecuteIfBound();
        return;
    }

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        StopFire();
        return;
    }

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    FVector TraceFXEnd = TraceEnd;
    if (HitResult.bBlockingHit)
    {
        TraceFXEnd = HitResult.ImpactPoint;
        MakeDamage(HitResult);
    }

    DecreaseAmmo();

    SetCurrentShot(GetCurrentShot() + 1);
}

void ATPSRifleWeapon::MakeDamage(const FHitResult& HitResult)
{
    const auto DamagedActor = HitResult.GetActor();
    if (!DamagedActor) return;

    FPointDamageEvent PointDamageEvent;
    PointDamageEvent.HitInfo = HitResult;
    DamagedActor->TakeDamage(DamageAmount, PointDamageEvent, GetOwnerController(), this);
}

void ATPSRifleWeapon::OnRep_CurrentShot()
{
    if (GetNetMode() == ENetMode::NM_DedicatedServer) return;

    if (GetCurrentShot() == 0)
    {
        SetMuzzleFXVisibility(false);
        return;
    }

    InitMuzzleFX();
}

void ATPSRifleWeapon::InitMuzzleFX()
{
    if (!MuzzleFXComponent)
    {
        MuzzleFXComponent = SpawnMuzzleFX();
    }

    SetMuzzleFXVisibility(true);
}

void ATPSRifleWeapon::SetMuzzleFXVisibility(bool IsVisible)
{
    if (!MuzzleFXComponent) return;

    MuzzleFXComponent->SetPaused(!IsVisible);

    MuzzleFXComponent->SetVisibility(IsVisible);
}
