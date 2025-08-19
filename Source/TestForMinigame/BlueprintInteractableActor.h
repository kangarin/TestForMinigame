// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "BlueprintInteractableActor.generated.h"

UCLASS(BlueprintType, Blueprintable)
class TESTFORMINIGAME_API ABlueprintInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlueprintInteractableActor();

protected:

	// 静态网格体组件 - 显示物体
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComponent;

	// 碰撞体组件 - 检测玩家是否靠近
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* InteractionSphere;

public:
	// 是否已经被交互过（网络复制）
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Interaction")
	bool bIsInteracted;

	// 交互提示文本 - 可在蓝图中设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FString InteractionPrompt;

	// 是否可以重复交互（比如开关）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bCanInteractMultipleTimes;

	// 交互后是否自动销毁
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bDestroyAfterInteraction;

    // 网络相关
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // 检查是否可以交互
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    virtual bool CanInteract() const;

    // ★ 服务器逻辑：业务逻辑，数据修改 ★
    UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
    void BP_ExecuteServerLogic(APawn* InteractingPlayer);

    // ★ 多播逻辑：音效、特效、动画 ★
    UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
    void BP_PlayInteractionEffects(APawn* InteractingPlayer);

    // ★ 客户端逻辑：UI反馈、本地效果 ★
    UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
    void BP_HandleClientFeedback(APawn* InteractingPlayer, bool bIsLocalPlayer);

    // 网络函数：多播给所有客户端播放效果
    UFUNCTION(NetMulticast, Reliable)
    void MulticastPlayEffects(APawn* InteractingPlayer);

    // 网络函数：发送给特定客户端的反馈
    UFUNCTION(Client, Reliable)
    void ClientShowFeedback(APawn* InteractingPlayer);

	// C++的交互执行函数，内部调用蓝图事件
    virtual void ExecuteInteraction(APawn* InteractingPlayer);

    // 获取交互提示文本
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    virtual FString GetInteractionPrompt() const;

    // 设置交互提示文本
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void SetInteractionPrompt(const FString& NewPrompt);

    // 碰撞事件
    UFUNCTION()
    void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    // 蓝图可调用的工具函数
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void MarkAsInteracted();

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void ResetInteraction();

private:
    // 当前在交互范围内的玩家列表
    UPROPERTY()
    TArray<APawn*> PlayersInRange;

};
