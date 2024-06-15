// This project is made for a test assignment

#include "Weapons/TPSRifleWeapon.h"
#include "Engine/DamageEvents.h"
#include "DrawDebugHelpers.h"

void ATPSRifleWeapon::MakeShot() 
{
    if (!GetWorld() || IsAmmoEmpty())
    {
        StopFire();
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

    DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 5.0f);

    DecreaseAmmo();
}

void ATPSRifleWeapon::MakeDamage(const FHitResult& HitResult)
{
    const auto DamagedActor = HitResult.GetActor();
    if (!DamagedActor) return;

    FPointDamageEvent PointDamageEvent;
    PointDamageEvent.HitInfo = HitResult;
    DamagedActor->TakeDamage(DamageAmount, PointDamageEvent, GetOwnerController(), this);
}