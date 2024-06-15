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

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void InitWeapons();

    FTransform GetSocketTransform(const FName SocketName) const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponComponent")
    TArray<TSubclassOf<ATPSBaseWeapon>> AvaibleWeaponsClasses;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponComponent")
    TArray<TObjectPtr<ATPSBaseWeapon>> SpawnedWeapons;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "WeaponComponent")
    TObjectPtr<ATPSBaseWeapon> CurrentWeapon;

    TObjectPtr<USkeletalMeshComponent> GetOwnerMesh() const;

    UPROPERTY(BlueprintReadOnly, Category = "WeaponComponent")
    FName WeaponSocketName = "WeaponSocket";

    UPROPERTY(BlueprintReadOnly, Category = "WeaponComponent")
    FName WeaponSpineSocket = "WeaponSpineSocket";
};
