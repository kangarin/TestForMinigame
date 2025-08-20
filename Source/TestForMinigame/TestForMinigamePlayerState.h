#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "TestForMinigamePlayerState.generated.h"

// 定义属性类型枚举
UENUM(BlueprintType)
enum class EPlayerAttributeType : uint8
{
    Sanity      UMETA(DisplayName = "理智"),
    Vitality    UMETA(DisplayName = "生命"),
    Speed       UMETA(DisplayName = "移速"),
    Will        UMETA(DisplayName = "意志"),
    Knowledge   UMETA(DisplayName = "知识")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedDelegate, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSanValueChangedDelegate, float, NewSanValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeadStateChangedDelegate, bool, bNewDeadState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGhostStateChangedDelegate, bool, bNewGhostState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChangedDelegate, EPlayerAttributeType, AttributeType, int32, NewValue);


UCLASS(BlueprintType, Blueprintable)
class TESTFORMINIGAME_API ATestForMinigamePlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    ATestForMinigamePlayerState();

    // 基础状态值 - 使用RepNotify
    UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category = "Player Stats")
    float Health = 100.0f;

    UPROPERTY(ReplicatedUsing = OnRep_SanValue, BlueprintReadOnly, Category = "Player Stats")
    float SanValue = 100.0f;

    UPROPERTY(ReplicatedUsing = OnRep_IsDead, BlueprintReadOnly, Category = "Player Stats")
    bool bIsDead = false;

    UPROPERTY(ReplicatedUsing = OnRep_IsGhost, BlueprintReadOnly, Category = "Player Stats")
    bool bIsGhost = false;

    // 五维属性（简化版）- 使用RepNotify
    UPROPERTY(ReplicatedUsing = OnRep_Sanity, BlueprintReadWrite, Category = "Attributes")
    int32 Sanity = 5;       // 理智 (0-10)

    UPROPERTY(ReplicatedUsing = OnRep_Vitality, BlueprintReadWrite, Category = "Attributes")
    int32 Vitality = 5;     // 生命 (0-10)

    UPROPERTY(ReplicatedUsing = OnRep_Speed, BlueprintReadWrite, Category = "Attributes")
    int32 Speed = 5;        // 移速 (0-10)

    UPROPERTY(ReplicatedUsing = OnRep_Will, BlueprintReadWrite, Category = "Attributes")
    int32 Will = 5;         // 意志 (0-10)

    UPROPERTY(ReplicatedUsing = OnRep_Knowledge, BlueprintReadWrite, Category = "Attributes")
    int32 Knowledge = 5;    // 知识 (0-10)

    // RPC函数 - 客户端调用，服务器执行
    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Actions")
    void ServerModifyHealth(float DeltaHealth);

    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Actions")
    void ServerModifySanValue(float DeltaSan);

    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Actions")
    void ServerSetDeadState(bool bNewDeadState);

    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Actions")
    void ServerSetGhostState(bool bNewGhostState);

    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Actions")
    void ServerModifyAttribute(EPlayerAttributeType AttributeType, int32 DeltaValue);

    // 委托事件 - 可以在蓝图中绑定
    UPROPERTY(BlueprintAssignable, Category = "Player Events")
    FOnHealthChangedDelegate OnHealthChanged;

    UPROPERTY(BlueprintAssignable, Category = "Player Events")
    FOnSanValueChangedDelegate OnSanValueChanged;

    UPROPERTY(BlueprintAssignable, Category = "Player Events")
    FOnDeadStateChangedDelegate OnDeadStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Player Events")
	FOnGhostStateChangedDelegate OnGhostStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Player Events")
    FOnAttributeChangedDelegate OnAttributeChanged;

protected:
    // RepNotify函数
    UFUNCTION()
    void OnRep_Health();

    UFUNCTION()
    void OnRep_SanValue();

    UFUNCTION()
    void OnRep_IsDead();

    UFUNCTION()
    void OnRep_IsGhost();

    UFUNCTION()
    void OnRep_Sanity();

    UFUNCTION()
    void OnRep_Vitality();

    UFUNCTION()
    void OnRep_Speed();

    UFUNCTION()
    void OnRep_Will();

    UFUNCTION()
    void OnRep_Knowledge();

    // 网络同步必需的函数
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // 辅助函数
    void ClampHealth();
    void ClampSanValue();
    void ClampAttribute(int32& AttributeValue);
};