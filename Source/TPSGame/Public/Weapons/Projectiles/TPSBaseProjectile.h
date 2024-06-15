// This project is made for a test assignment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSBaseProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class TPSGAME_API ATPSBaseProjectile : public AActor
{
    GENERATED_BODY()

public:
    ATPSBaseProjectile();

    void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<USphereComponent> CollisionComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UProjectileMovementComponent> MovementComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Projectile")
    float DamageRadius = 200.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Projectile")
    float DamageAmount = 50.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Projectile")
    bool DoFullDamage = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Projectile")
    float LifeSeconds = 5.0f;

private:
    FVector ShotDirection;

    UFUNCTION()
    void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

    AController* GetController() const;
};
