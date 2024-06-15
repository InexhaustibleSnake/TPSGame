// This project is made for a test assignment

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPSHealthComponent.generated.h"

class UPhysicalMaterial;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, NewHealth, float, NewHealthPercent);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TPSGAME_API UTPSHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTPSHealthComponent();

public:
    float GetHealth() const { return Health; }

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealthPercent() const { return Health / MaxHealth; }

    bool IsDead() const { return FMath::IsNearlyZero(Health); }

    UPROPERTY(BlueprintAssignable, Category = "Health")
    FOnHealthChangedSignature OnHealthChanged;

    UPROPERTY(BlueprintAssignable, Category = "Health")
    FOnDeathSignature OnDeath;

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION()
    void OnRep_Health();

    UFUNCTION()
    void OnRep_Dead();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
    float MaxHealth = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
    TMap<UPhysicalMaterial*, float> DamageModifiers;

private:
    UFUNCTION()
    void OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation,
        class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType,
        AActor* DamageCauser);

    UFUNCTION()
    void OnTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin,
        const FHitResult& HitInfo, class AController* InstigatedBy, AActor* DamageCauser);

    void ApplyDamage(float Damage, AController* InstigatedBy);

    void SetHealth(float NewHealth);

    float GetPointDamageModifier(AActor* DamagedActor, const FName& BoneName);

    UPROPERTY(ReplicatedUsing = OnRep_Health)
    float Health = 100.0f;

    UPROPERTY(ReplicatedUsing = OnRep_Dead)
    bool Dead = false;
};
