// This project is made for a test assignment

#include "Characters/TPSBaseCharacter.h"
#include "Components/TPSHealthComponent.h"

ATPSBaseCharacter::ATPSBaseCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    TPSHealthComponent = CreateDefaultSubobject<UTPSHealthComponent>("TPSHealthComponent");

    SetupCharacterMesh();
}

void ATPSBaseCharacter::SetupCharacterMesh()
{
    if (!GetMesh()) return;

    const FString MeshAssetPath = "SkeletalMesh'/Game/External/Characters/HeroTPP/HeroTPP.HeroTPP'";

    ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(*MeshAssetPath);

    const auto PlayerMesh = MeshAsset.Object;

    GetMesh()->SetSkeletalMesh(PlayerMesh);

    FVector MeshLocation(0.0f, 0.0f, -90.0f);
    FRotator MeshRotation(0.0f, -90.0f, 0.0f);

    GetMesh()->SetRelativeLocationAndRotation(MeshLocation, MeshRotation);
}
