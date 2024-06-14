// This project is made for a test assignment

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TPSCharacterAnimInstance.generated.h"

UCLASS()
class TPSGAME_API UTPSCharacterAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

protected:
    virtual void NativeBeginPlay() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    UFUNCTION(BlueprintCallable, Category = "TPSCharacterAnimInstance")
    float GetOwningPawnMovementVelocity() const;

    UFUNCTION(BlueprintCallable, Category = "TPSCharacterAnimInstance")
    float GetOwningPawnMovementVelocityNormalized() const;

    UFUNCTION(BlueprintCallable, Category = "TPSCharacterAnimInstance")
    float GetOwningPawnMovementDirection() const;

    UFUNCTION(BlueprintCallable, Category = "TPSCharacterAnimInstance")
    FVector GetOwningPawnMovementVector() const;

    UFUNCTION(BlueprintCallable, Category = "TPSCharacterAnimInstance")
    float GetOwnerPawnMaxWalkSpeed() const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TPSCharacterAnimInstance")
    float MovementVelocity = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TPSCharacterAnimInstance")
    float MovementDirection = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TPSCharacterAnimInstance")
    bool IsFalling = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TPSCharacterAnimInstance")
    TObjectPtr<ACharacter> OwnerCharacter = nullptr;
};
