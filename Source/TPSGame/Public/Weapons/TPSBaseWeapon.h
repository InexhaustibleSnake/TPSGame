// This project is made for a test assignment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSBaseWeapon.generated.h"

UCLASS()
class TPSGAME_API ATPSBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ATPSBaseWeapon();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

};
