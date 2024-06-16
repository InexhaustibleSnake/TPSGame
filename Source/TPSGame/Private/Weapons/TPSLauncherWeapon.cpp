// This project is made for a test assignment

#include "Weapons/TPSLauncherWeapon.h"
#include "Weapons/Projectiles/TPSBaseProjectile.h"

void ATPSLauncherWeapon::StartFire()
{
    Super::StartFire();

    if (!HasAuthority()) return;

    MakeShot();
}

void ATPSLauncherWeapon::MakeShot()
{
    if (!GetWorld() || IsAmmoEmpty() || IsClipEmpty()) return;

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd)) return;

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
    ATPSBaseProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATPSBaseProjectile>(ProjectileClass, SpawnTransform);

    if (Projectile)
    {
        Projectile->SetShotDirection(Direction);
        Projectile->SetOwner(GetOwner());
        Projectile->FinishSpawning(SpawnTransform);
    }

    DecreaseAmmo();
    SpawnMuzzleFX();

    StopFire();
}