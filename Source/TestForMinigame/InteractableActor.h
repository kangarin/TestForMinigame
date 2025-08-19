// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "InteractableActor.generated.h"

UCLASS(Abstract)
class TESTFORMINIGAME_API AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor();

protected:

	// 静态网格体组件 - 显示方块
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComponent;

	// 碰撞体组件 - 检测玩家是否靠近
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* InteractionSphere;

public:

    // 是否已经被交互过（网络复制）
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Interaction")
    bool bIsInteracted;

    // 网络相关
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // 检查是否可以交互
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    bool CanInteract() const;

    // 交互执行函数 - 子类必须重写这个函数
    UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
    void OnInteractionExecuted(APawn* InteractingPlayer);

    // C++版本的交互执行函数 - 供子类重写
    virtual void ExecuteInteraction(APawn* InteractingPlayer);

    // 获取交互提示文本 - 子类可以重写
    virtual FString GetInteractionPrompt() const;

    // 碰撞事件
    UFUNCTION()
    void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
    // 当前在交互范围内的玩家列表
    UPROPERTY()
    TArray<APawn*> PlayersInRange;

};
