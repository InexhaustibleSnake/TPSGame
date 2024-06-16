// This project is made for a test assignment

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPSWeaponComponent.generated.h"

class ATPSBaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TPSGAME_API UTPSWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTPSWeaponComponent();

    void NextWeapon();

    void StartFire();
    void StopFire();

    void Reload();

    void SetIsReloading(bool IsReloading);

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void InitWeapons();

    void PlayReloadMontage();

    void EquipWeapon(const int32 WeaponIndex);

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UFUNCTION(Server, Reliable)
    void ServerEquipWeapon(const int32 WeaponIndex);
    void ServerEquipWeapon_Implementation(const int32 WeaponIndex);

    UFUNCTION()
    void OnRep_CurrentWeapon(ATPSBaseWeapon* PreviousWeapon);

    UFUNCTION()
    void OnRep_Reloading();

    void AttachWeaponToMesh(ATPSBaseWeapon* Weapon, const FName SocketName);

    bool CanFire() const;

    FTransform GetSocketTransform(const FName SocketName) const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponComponent")
    TArray<TSubclassOf<ATPSBaseWeapon>> AvaibleWeaponsClasses;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "WeaponComponent")
    TArray<TObjectPtr<ATPSBaseWeapon>> SpawnedWeapons;

    UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon, BlueprintReadOnly, Category = "WeaponComponent")
    TObjectPtr<ATPSBaseWeapon> CurrentWeapon = nullptr;

    UPROPERTY(ReplicatedUsing = OnRep_Reloading, BlueprintReadOnly, Category = "WeaponComponent")
    bool Reloading = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponComponent")
    TObjectPtr<UAnimMontage> EquipMontage = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponComponent")
    TObjectPtr<UAnimMontage> ReloadMontage = nullptr;

    TObjectPtr<USkeletalMeshComponent> GetOwnerMesh() const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponComponent")
    FName WeaponSocketName = "WeaponPoint";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponComponent")
    FName WeaponSpineSocketName = "WeaponSpineSocket";
};
