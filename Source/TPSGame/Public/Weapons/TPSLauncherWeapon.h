// This project is made for a test assignment

#pragma once

#include "CoreMinimal.h"
#include "Weapons/TPSBaseWeapon.h"
#include "TPSLauncherWeapon.generated.h"

class ATPSBaseProjectile;

UCLASS()
class TPSGAME_API ATPSLauncherWeapon : public ATPSBaseWeapon
{
    GENERATED_BODY()

public:
    virtual void StartFire() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<ATPSBaseProjectile> ProjectileClass;

    virtual void MakeShot() override;
};
