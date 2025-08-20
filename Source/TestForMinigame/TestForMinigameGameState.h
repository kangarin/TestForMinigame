// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "TestForMinigameGameState.generated.h"

// 灵视值变化的委托声明
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInsightValueChanged, int32, NewValue, int32, OldValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnterPhaseTwo);

/**
 * 
 */
UCLASS()
class TESTFORMINIGAME_API ATestForMinigameGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ATestForMinigameGameState();

    // ========== 公开属性 ==========

    // 当前灵视值 (网络复制)
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Insight")
    int32 GlobalInsightValue = 0;

    // 是否已进入二阶段
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Insight")
    bool bIsPhaseTwo = false;

    // 灵视值上限
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Insight")
    int32 MaxInsightValue = 1000;

    // ========== 委托事件 ==========

    // 灵视值变化时触发
    UPROPERTY(BlueprintAssignable, Category = "Insight Events")
    FOnInsightValueChanged OnInsightValueChanged;

    // 进入二阶段时触发
    UPROPERTY(BlueprintAssignable, Category = "Insight Events")
    FOnEnterPhaseTwo OnEnterPhaseTwo;

    // ========== 公开函数 ==========

    // 供PlayerController在服务器上调用的直接函数
    UFUNCTION(BlueprintCallable, Category = "Insight")
    void AddInsightValueDirect(int32 Amount);

    // 获取当前应显示的阈值等级 (250/500/750/1000)
    UFUNCTION(BlueprintPure, Category = "Insight")
    int32 GetCurrentThresholdLevel() const;

protected:
    // ========== 网络复制函数 ==========
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
    // 检查并处理阶段转换
    void CheckPhaseTransition();

    // 触发UI更新 (多播)
    UFUNCTION(NetMulticast, Reliable)
    void MulticastUpdateUI(int32 OldValue, int32 NewValue);

    // 触发二阶段转换 (多播)
    UFUNCTION(NetMulticast, Reliable)
    void MulticastEnterPhaseTwo();
	
};
