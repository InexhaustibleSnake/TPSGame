// This project is made for a test assignment

#pragma once

#include "CoreMinimal.h"
#include "Weapons/TPSBaseWeapon.h"
#include "TPSRifleWeapon.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class TPSGAME_API ATPSRifleWeapon : public ATPSBaseWeapon
{
    GENERATED_BODY()

protected:
    virtual void StartFire() override;
    virtual void StopFire() override;

    virtual void MakeShot() override;

    void MakeDamage(const FHitResult& HitResult);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TimeBetweenShots = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float BulletSpread = 1.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageAmount = 10.0f;

    virtual void OnRep_CurrentShot() override;

private:
    void InitMuzzleFX();
    void SetMuzzleFXVisibility(bool IsVisible);

    FTimerHandle WeaponShotTimer;

    UPROPERTY()
    TObjectPtr<UNiagaraComponent> MuzzleFXComponent;
};
