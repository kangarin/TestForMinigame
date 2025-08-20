// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TestForMinigamePlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;

/**
 *  Basic PlayerController class for a third person game
 *  Manages input mappings
 */
UCLASS(abstract)
class ATestForMinigamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATestForMinigamePlayerController();
	
protected:

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category ="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	/** Mobile controls widget to spawn */
	UPROPERTY(EditAnywhere, Category="Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	/** Pointer to the mobile controls widget */
	TObjectPtr<UUserWidget> MobileControlsWidget;

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

	// 服务器RPC - 增加灵视值（这个有Owner，能正常工作）
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Insight")
	void ServerAddInsightValue(int32 Amount);

private:
	void ServerAddInsightValue_Implementation(int32 Amount);

	// 事件绑定
	UFUNCTION()
	void OnInsightChanged(int32 NewValue, int32 OldValue);

	UFUNCTION()
	void OnPhaseTwo();

};
