// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "DestructibleBox.generated.h"

/**
 * 
 */
UCLASS()
class TESTFORMINIGAME_API ADestructibleBox : public AInteractableActor
{
	GENERATED_BODY()

public:
    ADestructibleBox();

protected:
    // 重写交互逻辑
    virtual void ExecuteInteraction(APawn* InteractingPlayer) override;

    // 重写交互提示
    virtual FString GetInteractionPrompt() const override;
};
