// This project is made for a test assignment

#include "Characters/TPSPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Components/TPSWeaponComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

ATPSPlayerCharacter::ATPSPlayerCharacter()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
    SpringArm->SetupAttachment(GetMesh());

    MainCamera = CreateDefaultSubobject<UCameraComponent>("MainCamera");
    MainCamera->SetupAttachment(SpringArm);

    TPSWeaponComponent = CreateDefaultSubobject<UTPSWeaponComponent>("UTPSWeaponComponent");
}

void ATPSPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPSPlayerCharacter::Move);

        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATPSPlayerCharacter::Look);

        EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ATPSPlayerCharacter::StartFire);
        EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &ATPSPlayerCharacter::StopFire);

        EnhancedInputComponent->BindAction(ChangeWeaponAction, ETriggerEvent::Started, this, &ATPSPlayerCharacter::ChangeWeapon);
    }
}

void ATPSPlayerCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();

    AddMovementInput(GetActorForwardVector(), MovementVector.X);
    AddMovementInput(GetActorRightVector(), MovementVector.Y);
}

void ATPSPlayerCharacter::Look(const FInputActionValue& Value)
{
    if (!GetWorld()) return;

    float DeltatTime = GetWorld()->GetDeltaSeconds();

    FVector2D LookVector = Value.Get<FVector2D>();

    AddControllerYawInput(LookVector.X * DeltatTime * LookInputScale.X);
    AddControllerPitchInput(LookVector.Y * DeltatTime * LookInputScale.Y);
}

void ATPSPlayerCharacter::ChangeWeapon(const FInputActionValue& Value)
{
    if (!TPSWeaponComponent) return;

    TPSWeaponComponent->NextWeapon();
}

void ATPSPlayerCharacter::StartFire()
{
    if (!TPSWeaponComponent) return;

    TPSWeaponComponent->StartFire();
}

void ATPSPlayerCharacter::StopFire()
{
    if (!TPSWeaponComponent) return;

    TPSWeaponComponent->StopFire();
}
