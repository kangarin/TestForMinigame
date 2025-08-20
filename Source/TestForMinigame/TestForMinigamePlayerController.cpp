// Copyright Epic Games, Inc. All Rights Reserved.


#include "TestForMinigamePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "TestForMinigame.h"
#include "Widgets/Input/SVirtualJoystick.h"
#include "TestForMinigameGameState.h"
#include "Engine/Engine.h"

ATestForMinigamePlayerController::ATestForMinigamePlayerController()
{
	bReplicates = true;
}

void ATestForMinigamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 只有本地的PlayerController才绑定事件
	if (IsLocalPlayerController())
	{
		UE_LOG(LogTemp, Warning, TEXT("本地PlayerController绑定事件"));

		if (ATestForMinigameGameState* GS = GetWorld()->GetGameState<ATestForMinigameGameState>())
		{
			GS->OnInsightValueChanged.AddDynamic(this, &ATestForMinigamePlayerController::OnInsightChanged);
			GS->OnEnterPhaseTwo.AddDynamic(this, &ATestForMinigamePlayerController::OnPhaseTwo);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("非本地PlayerController，跳过事件绑定"));
	}

	// only spawn touch controls on local player controllers
	if (SVirtualJoystick::ShouldDisplayTouchInterface() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogTestForMinigame, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void ATestForMinigamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!SVirtualJoystick::ShouldDisplayTouchInterface())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}

void ATestForMinigamePlayerController::ServerAddInsightValue_Implementation(int32 Amount)
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerController Server RPC 执行！Amount = %d"), Amount);

	// 在服务器上执行，直接操作GameState
	if (ATestForMinigameGameState* GS = GetWorld()->GetGameState<ATestForMinigameGameState>())
	{
		int32 OldValue = GS->GlobalInsightValue;
		GS->GlobalInsightValue = FMath::Clamp(GS->GlobalInsightValue + Amount, 0, GS->MaxInsightValue);

		UE_LOG(LogTemp, Warning, TEXT("PlayerController修改灵视值: %d -> %d"), OldValue, GS->GlobalInsightValue);

		// 检查阶段转换
		if (GS->GlobalInsightValue >= GS->MaxInsightValue && !GS->bIsPhaseTwo)
		{
			GS->bIsPhaseTwo = true;
			GS->MulticastEnterPhaseTwo();
			UE_LOG(LogTemp, Error, TEXT("通过PlayerController进入二阶段！"));
		}

		// 多播UI更新
		GS->MulticastUpdateUI(OldValue, GS->GlobalInsightValue);
	}
}

void ATestForMinigamePlayerController::OnInsightChanged(int32 NewValue, int32 OldValue)
{
	if (GEngine)
	{
		FString Message = FString::Printf(TEXT("Controller收到灵视值变化: %d -> %d"), OldValue, NewValue);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, Message);
	}

	UE_LOG(LogTemp, Log, TEXT("PlayerController收到灵视值变化: %d -> %d"), OldValue, NewValue);
}

void ATestForMinigamePlayerController::OnPhaseTwo()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("🔥 PlayerController收到二阶段通知！🔥"));
	}

	UE_LOG(LogTemp, Error, TEXT("PlayerController收到二阶段通知！"));
}