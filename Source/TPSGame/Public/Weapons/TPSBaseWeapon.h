// This project is made for a test assignment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSBaseWeapon.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "1"))
    int32 Bullets = 30;

    UPROPERTY(NotReplicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!InfiniteAmmo"),
        meta = (ClampMin = "0"))
    int32 Clips = 3;

    UPROPERTY(NotReplicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool InfiniteAmmo = false;
};

UCLASS()
class TPSGAME_API ATPSBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ATPSBaseWeapon();

    virtual void StartFire();
    virtual void StopFire();

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    virtual void MakeShot();

    int32 GetCurrentShot() const { return CurrentShot; }

    void SetCurrentShot(int32 Amount);

    UFUNCTION(Server, Reliable)
    void ServerStartFire();
    void ServerStartFire_Implementation();

    UFUNCTION(Server, Reliable)
    void ServerStopFire();
    void ServerStopFire_Implementation();

    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

    void DecreaseAmmo();

    AController* GetOwnerController() const;

    bool IsAmmoEmpty() const;

    bool IsClipEmpty() const;

    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

    bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    UFUNCTION()
    virtual void OnRep_CurrentShot();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    TObjectPtr<USkeletalMeshComponent> WeaponMesh;

    UNiagaraComponent* SpawnMuzzleFX();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TPSBaseWeapon")
    float TraceMaxDistance = 5000.0f;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Ammo")
    FAmmoData CurrentAmmo;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
    FAmmoData DefaultAmmoData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TObjectPtr<UNiagaraSystem> MuzzleFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FName MuzzleSocketName = "MuzzleSocket";

private:
    UPROPERTY(ReplicatedUsing = OnRep_CurrentShot)
    int32 CurrentShot = 0;
};
