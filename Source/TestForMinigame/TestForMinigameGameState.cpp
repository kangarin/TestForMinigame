// Fill out your copyright notice in the Description page of Project Settings.


#include "TestForMinigameGameState.h"
#include "Engine/Engine.h"

ATestForMinigameGameState::ATestForMinigameGameState()
{
	// 启用网络复制
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;
}

void ATestForMinigameGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // 注册需要网络复制的属性
    DOREPLIFETIME(ATestForMinigameGameState, GlobalInsightValue);
    DOREPLIFETIME(ATestForMinigameGameState, bIsPhaseTwo);
}

void ATestForMinigameGameState::AddInsightValueDirect(int32 Amount)
{
    // 这个函数只在服务器上被PlayerController调用
    if (!HasAuthority())
    {
        UE_LOG(LogTemp, Error, TEXT("AddInsightValueDirect 只能在服务器调用！"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("GameState: AddInsightValueDirect 被调用，Amount = %d"), Amount);

    int32 OldValue = GlobalInsightValue;
    GlobalInsightValue = FMath::Clamp(GlobalInsightValue + Amount, 0, MaxInsightValue);

    UE_LOG(LogTemp, Warning, TEXT("GameState: 更新灵视值 %d -> %d"), OldValue, GlobalInsightValue);

    // 检查阶段转换
    CheckPhaseTransition();

    // 多播UI更新
    MulticastUpdateUI(OldValue, GlobalInsightValue);

    UE_LOG(LogTemp, Warning, TEXT("GameState: 灵视值变化完成 %d -> %d (+%d)"), OldValue, GlobalInsightValue, Amount);
}

void ATestForMinigameGameState::CheckPhaseTransition()
{
    // 检查是否应该进入二阶段
    if (GlobalInsightValue >= MaxInsightValue && !bIsPhaseTwo)
    {
        bIsPhaseTwo = true;
        MulticastEnterPhaseTwo();

        UE_LOG(LogTemp, Error, TEXT("进入二阶段！灵视值已达到 %d"), MaxInsightValue);
    }
}

void ATestForMinigameGameState::MulticastUpdateUI_Implementation(int32 OldValue, int32 NewValue)
{
    // 在所有客户端执行UI更新
    OnInsightValueChanged.Broadcast(NewValue, OldValue);

	// 屏幕提示
    if (GEngine)
    {
        FString Message = FString::Printf(TEXT("灵视值变化: %d -> %d"), OldValue, NewValue);
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Message);
	}
}

void ATestForMinigameGameState::MulticastEnterPhaseTwo_Implementation()
{
    // 在所有客户端触发二阶段事件
    OnEnterPhaseTwo.Broadcast();

    // 屏幕提示
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("进入二阶段：揭示阶段"));
    }
}

int32 ATestForMinigameGameState::GetCurrentThresholdLevel() const
{
    if (GlobalInsightValue >= 1000)
        return 1000;
    else if (GlobalInsightValue >= 750)
        return 750;
    else if (GlobalInsightValue >= 500)
        return 500;
    else if (GlobalInsightValue >= 250)
        return 250;
    else
        return 0;
}