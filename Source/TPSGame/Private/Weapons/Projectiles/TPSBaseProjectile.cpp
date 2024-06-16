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

    if (HasAuthority())
    {
        if (MovementComponent)
        {
            MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
        }
    }

    if (CollisionComponent)
    {
        CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
        CollisionComponent->OnComponentHit.AddDynamic(this, &ATPSBaseProjectile::OnProjectileHit);
    }

    SetLifeSpan(LifeSeconds);
}

void ATPSBaseProjectile::SetIsExploded(bool IsExploded)
{
    Exploded = IsExploded;
    OnRep_Exploded();
}

void ATPSBaseProjectile::OnRep_Exploded()
{
    auto SpawnedNiagara = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
        this, ExplodeFX, GetActorLocation(), GetActorRotation(), GetActorScale3D(), true, true, ENCPoolMethod::AutoRelease);

    if (!SpawnedNiagara) return;

    SpawnedNiagara->OnSystemFinished.AddDynamic(this, &ATPSBaseProjectile::OnNiagaraFinished);

    SetActorHiddenInGame(true);
}

void ATPSBaseProjectile::OnNiagaraFinished(UNiagaraComponent* PSystem)
{
    Destroy();
}

void ATPSBaseProjectile::OnProjectileHit(
    UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    UGameplayStatics::ApplyRadialDamage(
        this, DamageAmount, Hit.Location, DamageRadius, UDamageType::StaticClass(), {}, this, GetController(), DoFullDamage);

    MovementComponent->StopMovementImmediately();

    SetIsExploded(true);
}

AController* ATPSBaseProjectile::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}

void ATPSBaseProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ATPSBaseProjectile, Exploded);
}