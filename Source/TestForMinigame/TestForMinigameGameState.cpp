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

	OnInsightValueChanged.Broadcast(GlobalInsightValue, OldValue);

    UE_LOG(LogTemp, Warning, TEXT("GameState: 灵视值变化完成 %d -> %d (+%d)"), OldValue, GlobalInsightValue, Amount);
}

void ATestForMinigameGameState::CheckPhaseTransition()
{
    // 检查是否应该进入二阶段
    if (GlobalInsightValue >= MaxInsightValue && !bIsPhaseTwo)
    {
        bIsPhaseTwo = true;
        OnEnterPhaseTwo.Broadcast();

        UE_LOG(LogTemp, Error, TEXT("进入二阶段！灵视值已达到 %d"), MaxInsightValue);
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

void ATestForMinigameGameState::OnRep_GlobalInsightValue(int32 OldValue)
{
    OnInsightValueChanged.Broadcast(GlobalInsightValue, OldValue);
    UE_LOG(LogTemp, Warning, TEXT("Client: 灵视值变化 %d -> %d"), OldValue, GlobalInsightValue);

}

void ATestForMinigameGameState::OnRep_IsPhaseTwo()
{
    UE_LOG(LogTemp, Warning, TEXT("Client: 接收到阶段状态复制: %s"), bIsPhaseTwo ? TEXT("Phase Two") : TEXT("Phase One"));

    if (bIsPhaseTwo)
    {
        OnEnterPhaseTwo.Broadcast();
    }
}