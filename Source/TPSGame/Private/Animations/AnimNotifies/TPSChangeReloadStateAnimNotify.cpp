// This project is made for a test assignment

#include "Animations/AnimNotifies/TPSChangeReloadStateAnimNotify.h"
#include "Components/TPSWeaponComponent.h"

void UTPSChangeReloadStateAnimNotify::Notify(
    USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    if (!MeshComp) return;

    const auto OwnerActor = MeshComp->GetOwner();
    if (!OwnerActor) return;

    auto WeaponComponent = OwnerActor->FindComponentByClass<UTPSWeaponComponent>();
    if (!WeaponComponent) return;

    WeaponComponent->SetIsReloading(ReloadInProgress);

    Super::Notify(MeshComp, Animation, EventReference);
}
