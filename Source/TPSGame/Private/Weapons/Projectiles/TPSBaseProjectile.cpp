// This project is made for a test assignment

#include "Weapons/Projectiles/TPSBaseProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Net/UnrealNetwork.h"

#include "NiagaraComponent.h"

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

    bReplicates = true;
    SetReplicateMovement(true);
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

void ATPSBaseProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ATPSBaseProjectile, Exploded);
}

void ATPSBaseProjectile::OnRep_Exploded()
{
    auto SpawnedNiagara = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
        this, ExplodeFX, GetActorLocation(), GetActorRotation(), GetActorScale3D(), true, true, ENCPoolMethod::AutoRelease);
    if (!SpawnedNiagara) return;
    SpawnedNiagara->OnSystemFinished.AddDynamic(this, &ATPSBaseProjectile::OnNiagaraFinished);
}

void ATPSBaseProjectile::OnNiagaraFinished(UNiagaraComponent* PSystem)
{
    Destroy();
}

void ATPSBaseProjectile::OnProjectileHit(
    UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!GetWorld()) return;

    Exploded = true;
    OnRep_Exploded();

    MovementComponent->StopMovementImmediately();

    UGameplayStatics::ApplyRadialDamage(
        GetWorld(), DamageAmount, GetActorLocation(), DamageRadius, UDamageType::StaticClass(), {}, this, GetController(), DoFullDamage);
}

AController* ATPSBaseProjectile::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}