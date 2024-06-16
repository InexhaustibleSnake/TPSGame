// This project is made for a test assignment

#include "Characters/TPSPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Components/TPSWeaponComponent.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

ATPSPlayerCharacter::ATPSPlayerCharacter()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
    SpringArm->SetupAttachment(GetMesh());

    SpringArm->bUsePawnControlRotation = true;

    MainCamera = CreateDefaultSubobject<UCameraComponent>("MainCamera");
    MainCamera->SetupAttachment(SpringArm);

    TPSWeaponComponent = CreateDefaultSubobject<UTPSWeaponComponent>("UTPSWeaponComponent");
}

void ATPSPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    SetupTargeting();
}

void ATPSPlayerCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    TargetingTimeline.TickTimeline(DeltaSeconds);
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

        EnhancedInputComponent->BindAction(ReloadWeaponAction, ETriggerEvent::Started, this, &ATPSPlayerCharacter::Reload);

        EnhancedInputComponent->BindAction(TargetingAction, ETriggerEvent::Started, this, &ATPSPlayerCharacter::OnPlayerTargeting, true);
        EnhancedInputComponent->BindAction(TargetingAction, ETriggerEvent::Completed, this, &ATPSPlayerCharacter::OnPlayerTargeting, false);
    }
}

void ATPSPlayerCharacter::OnDeath()
{
    if (TPSWeaponComponent)
    {
        TPSWeaponComponent->StopFire();
    }

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);

    OnPlayerTargeting(false);
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

    AddControllerYawInput(LookVector.X);
    AddControllerPitchInput(LookVector.Y);
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

void ATPSPlayerCharacter::Reload()
{
    if (!TPSWeaponComponent) return;

    TPSWeaponComponent->Reload();
}

void ATPSPlayerCharacter::ChangeFOV(float Alpha)
{
    if (!MainCamera) return;

    MainCamera->FieldOfView = FMath::Lerp(NonTargetingFOV, TargetingFOV, Alpha);
}

void ATPSPlayerCharacter::OnPlayerTargeting(bool IsTargeting)
{
    IsTargeting ? TargetingTimeline.Play() : TargetingTimeline.Reverse();
}

void ATPSPlayerCharacter::SetupTargeting()
{
    if (!TargetingCurve) return;

    FOnTimelineFloat TargetingUpdate;
    TargetingUpdate.BindUFunction(this, FName("ChangeFOV"));

    TargetingTimeline.AddInterpFloat(TargetingCurve, TargetingUpdate);
    TargetingTimeline.SetLooping(false);
    TargetingTimeline.SetTimelineLength(1.0f);
    TargetingTimeline.SetPlayRate(TargetingTime);
}
