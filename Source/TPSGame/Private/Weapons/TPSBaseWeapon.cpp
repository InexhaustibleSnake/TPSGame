// This project is made for a test assignment

#include "Weapons/TPSBaseWeapon.h"

ATPSBaseWeapon::ATPSBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);

	bReplicates = true;
}

void ATPSBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}
