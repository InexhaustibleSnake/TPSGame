// This project is made for a test assignment

#include "Weapons/Projectiles/TPSBaseProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

ATPSBaseProjectile::ATPSBaseProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(5.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    CollisionComponent->bReturnMaterialOnMove = true;
    SetRootComponent(CollisionComponent);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
    MovementComponent->InitialSpeed = 2000.0f;
    MovementComponent->ProjectileGravityScale = 0.0f;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetupAttachment(GetRootComponent());
}

void ATPSBaseProjectile::BeginPlay()
{
    Super::BeginPlay();

    if (MovementComponent)
    {
        MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
    }

    if (CollisionComponent)
    {
        CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
        CollisionComponent->OnComponentHit.AddDynamic(this, &ATPSBaseProjectile::OnProjectileHit);
    }

    SetLifeSpan(LifeSeconds);
}

void ATPSBaseProjectile::OnProjectileHit(
    UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!GetWorld()) return;

    MovementComponent->StopMovementImmediately();

    UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageAmount, GetActorLocation(), DamageRadius, UDamageType::StaticClass(),
        {}, this, GetController(), DoFullDamage);

    Destroy();
}

AController* ATPSBaseProjectile::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}