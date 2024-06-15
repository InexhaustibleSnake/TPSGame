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

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void InitWeapons();

    void EquipWeapon(const int32 WeaponIndex);

    UFUNCTION(Server, Reliable)
    void ServerEquipWeapon(const int32 WeaponIndex);
    void ServerEquipWeapon_Implementation(const int32 WeaponIndex);

    UFUNCTION()
    void OnRep_CurrentWeapon(ATPSBaseWeapon* PreviousWeapon);

    void AttachWeaponToMesh(ATPSBaseWeapon* Weapon, const FName SocketName);

    FTransform GetSocketTransform(const FName SocketName) const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponComponent")
    TArray<TSubclassOf<ATPSBaseWeapon>> AvaibleWeaponsClasses;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "WeaponComponent")
    TArray<TObjectPtr<ATPSBaseWeapon>> SpawnedWeapons;

    UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon, BlueprintReadOnly, Category = "WeaponComponent")
    TObjectPtr<ATPSBaseWeapon> CurrentWeapon = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponComponent")
    TObjectPtr<UAnimMontage> EquipMontage = nullptr;

    TObjectPtr<USkeletalMeshComponent> GetOwnerMesh() const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponComponent")
    FName WeaponSocketName = "WeaponPoint";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponComponent")
    FName WeaponSpineSocketName = "WeaponSpineSocket";
};
