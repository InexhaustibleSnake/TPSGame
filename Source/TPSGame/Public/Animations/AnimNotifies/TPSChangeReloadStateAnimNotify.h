// This project is made for a test assignment

#pragma once

#include "CoreMinimal.h"
#include "Animations/AnimNotifies/TPSBaseAnimNotify.h"
#include "TPSChangeReloadStateAnimNotify.generated.h"

UCLASS()
class TPSGAME_API UTPSChangeReloadStateAnimNotify : public UTPSBaseAnimNotify
{
    GENERATED_BODY()

protected:
    virtual void Notify(
        USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
    bool ReloadInProgress = false;
};
