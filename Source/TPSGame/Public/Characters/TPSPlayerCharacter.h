// This project is made for a test assignment

#pragma once

#include "CoreMinimal.h"
#include "Characters/TPSBaseCharacter.h"
#include "TPSPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

class UTPSWeaponComponent;

class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class TPSGAME_API ATPSPlayerCharacter : public ATPSBaseCharacter
{
    GENERATED_BODY()

public:
    ATPSPlayerCharacter();

protected:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UCameraComponent> MainCamera;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    TObjectPtr<USpringArmComponent> SpringArm;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UTPSWeaponComponent> TPSWeaponComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputMappingContext> DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputAction> JumpAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputAction> LookAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputAction> ShootAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputAction> AimAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
    FVector2D LookInputScale = FVector2D(90.0f, 90.0f);
};
