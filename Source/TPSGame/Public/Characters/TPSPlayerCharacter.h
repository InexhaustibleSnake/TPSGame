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

    void ChangeWeapon(const FInputActionValue& Value);

    void StartFire();
    void StopFire();

    void Reload();

    void SetTargeting(bool IsTargeting);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UCameraComponent> MainCamera;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    TObjectPtr<USpringArmComponent> SpringArm;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UTPSWeaponComponent> TPSWeaponComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Aim")
    UCurveFloat* TargetingCurve = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Aim")
    float TargetingTime = 2.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Aim")
    bool Targeting = false;

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
    TObjectPtr<UInputAction> TargetingAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputAction> ChangeWeaponAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputAction> ReloadWeaponAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
    FVector2D LookInputScale = FVector2D(90.0f, 90.0f);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    FName SpringArmAttachName = "";
};
