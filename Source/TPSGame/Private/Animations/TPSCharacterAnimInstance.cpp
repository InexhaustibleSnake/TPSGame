// This project is made for a test assignment

#include "Animations/TPSCharacterAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UTPSCharacterAnimInstance::NativeBeginPlay() 
{
    Super::NativeBeginPlay();

    OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
}

void UTPSCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    MovementVelocity = GetOwningPawnMovementVelocityNormalized();

    MovementDirection = GetOwningPawnMovementDirection();

    IsFalling = !FMath::IsNearlyZero(GetOwningPawnMovementVector().Z);
}

float UTPSCharacterAnimInstance::GetOwningPawnMovementVelocity() const
{
    return OwnerCharacter ? OwnerCharacter->GetVelocity().Length() : 0.0f;
}

float UTPSCharacterAnimInstance::GetOwningPawnMovementVelocityNormalized() const
{
    return UKismetMathLibrary::NormalizeToRange(GetOwningPawnMovementVelocity(), 0.0f, GetOwnerPawnMaxWalkSpeed());
}

FVector UTPSCharacterAnimInstance::GetOwningPawnMovementVector() const
{
    return TryGetPawnOwner() ? TryGetPawnOwner()->GetVelocity() : FVector();
}

float UTPSCharacterAnimInstance::GetOwnerPawnMaxWalkSpeed() const
{
    if (!OwnerCharacter || !OwnerCharacter->GetCharacterMovement()) return 0.0f;

    return OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed;
}

float UTPSCharacterAnimInstance::GetOwningPawnMovementDirection() const
{
    if (!TryGetPawnOwner()) return 0.0f;

    const auto VelocityVector = TryGetPawnOwner()->GetVelocity().GetSafeNormal();

    if (VelocityVector.IsNearlyZero()) return 0.0f;

    const auto ForwardVector = TryGetPawnOwner()->GetActorForwardVector();

    const auto MovementAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ForwardVector, VelocityVector)));

    const auto CrossProduct = FVector::CrossProduct(ForwardVector, VelocityVector);

    return CrossProduct.IsZero() ? MovementAngle : MovementAngle * FMath::Sign(CrossProduct.Z);
}