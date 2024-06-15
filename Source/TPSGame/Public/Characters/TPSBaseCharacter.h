// This project is made for a test assignment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSBaseCharacter.generated.h"

class UTPSHealthComponent;

UCLASS()
class TPSGAME_API ATPSBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ATPSBaseCharacter();

protected:
    void SetupCharacterMesh();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UTPSHealthComponent> TPSHealthComponent;
};
