#include "TestForMinigamePlayerState.h"
#include "Engine/Engine.h"

ATestForMinigamePlayerState::ATestForMinigamePlayerState()
{
    // 启用网络同步
    bReplicates = true;

    // 设置默认值
    Health = 100.0f;
    SanValue = 100.0f;
    bIsDead = false;
    bIsGhost = false;

    // 设置默认五维属性
    Sanity = 5;
    Vitality = 5;
    Speed = 5;
    Will = 5;
    Knowledge = 5;
}

void ATestForMinigamePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // 告诉引擎这些变量需要网络同步
    DOREPLIFETIME(ATestForMinigamePlayerState, Health);
    DOREPLIFETIME(ATestForMinigamePlayerState, SanValue);
    DOREPLIFETIME(ATestForMinigamePlayerState, bIsDead);
    DOREPLIFETIME(ATestForMinigamePlayerState, bIsGhost);
    DOREPLIFETIME(ATestForMinigamePlayerState, Sanity);
    DOREPLIFETIME(ATestForMinigamePlayerState, Vitality);
    DOREPLIFETIME(ATestForMinigamePlayerState, Speed);
    DOREPLIFETIME(ATestForMinigamePlayerState, Will);
    DOREPLIFETIME(ATestForMinigamePlayerState, Knowledge);
}

// RPC实现
void ATestForMinigamePlayerState::ServerModifyHealth_Implementation(float DeltaHealth)
{
    Health += DeltaHealth;
    ClampHealth();

    // 如果血量归零，设置死亡状态
    if (Health <= 0.0f && !bIsDead)
    {
        bIsDead = true;
    }
}

void ATestForMinigamePlayerState::ServerModifySanValue_Implementation(float DeltaSan)
{
    SanValue += DeltaSan;
    ClampSanValue();
}

void ATestForMinigamePlayerState::ServerSetDeadState_Implementation(bool bNewDeadState)
{
    bIsDead = bNewDeadState;
}

void ATestForMinigamePlayerState::ServerSetGhostState_Implementation(bool bNewGhostState)
{
    bIsGhost = bNewGhostState;
}

void ATestForMinigamePlayerState::ServerModifyAttribute_Implementation(EPlayerAttributeType AttributeType, int32 DeltaValue)
{
    switch (AttributeType)
    {
    case EPlayerAttributeType::Sanity:
        Sanity += DeltaValue;
        ClampAttribute(Sanity);
        break;
    case EPlayerAttributeType::Vitality:
        Vitality += DeltaValue;
        ClampAttribute(Vitality);
        break;
    case EPlayerAttributeType::Speed:
        Speed += DeltaValue;
        ClampAttribute(Speed);
        break;
    case EPlayerAttributeType::Will:
        Will += DeltaValue;
        ClampAttribute(Will);
        break;
    case EPlayerAttributeType::Knowledge:
        Knowledge += DeltaValue;
        ClampAttribute(Knowledge);
        break;
    }
}

// RepNotify实现
void ATestForMinigamePlayerState::OnRep_Health()
{
    if (GEngine)
    {
        FString Message = FString::Printf(TEXT("[%s] 血量变化: %.1f"), *GetPlayerName(), Health);
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, Message);
    }
    OnHealthChanged.Broadcast(Health);
}

void ATestForMinigamePlayerState::OnRep_SanValue()
{
    if (GEngine)
    {
        FString Message = FString::Printf(TEXT("[%s] 理智变化: %.1f"), *GetPlayerName(), SanValue);
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, Message);
    }
    OnSanValueChanged.Broadcast(SanValue);
}

void ATestForMinigamePlayerState::OnRep_IsDead()
{
    if (GEngine)
    {
        FString Message = FString::Printf(TEXT("[%s] 死亡状态: %s"), *GetPlayerName(), bIsDead ? TEXT("死亡") : TEXT("存活"));
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Purple, Message);
    }
    OnDeadStateChanged.Broadcast(bIsDead);
}

void ATestForMinigamePlayerState::OnRep_IsGhost()
{
    if (GEngine)
    {
        FString Message = FString::Printf(TEXT("[%s] 幽灵状态: %s"), *GetPlayerName(), bIsGhost ? TEXT("幽灵") : TEXT("实体"));
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, Message);
    }
    OnGhostStateChanged.Broadcast(bIsGhost);
}

void ATestForMinigamePlayerState::OnRep_Sanity()
{
    if (GEngine)
    {
        FString Message = FString::Printf(TEXT("[%s] 理智属性: %d"), *GetPlayerName(), Sanity);
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, Message);
    }
    OnAttributeChanged.Broadcast(EPlayerAttributeType::Sanity, Sanity);
}

void ATestForMinigamePlayerState::OnRep_Vitality()
{
    if (GEngine)
    {
        FString Message = FString::Printf(TEXT("[%s] 生命属性: %d"), *GetPlayerName(), Vitality);
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, Message);
    }
    OnAttributeChanged.Broadcast(EPlayerAttributeType::Vitality, Vitality);
}

void ATestForMinigamePlayerState::OnRep_Speed()
{
    if (GEngine)
    {
        FString Message = FString::Printf(TEXT("[%s] 移速属性: %d"), *GetPlayerName(), Speed);
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, Message);
    }
    OnAttributeChanged.Broadcast(EPlayerAttributeType::Speed, Speed);
}

void ATestForMinigamePlayerState::OnRep_Will()
{
    if (GEngine)
    {
        FString Message = FString::Printf(TEXT("[%s] 意志属性: %d"), *GetPlayerName(), Will);
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, Message);
    }
    OnAttributeChanged.Broadcast(EPlayerAttributeType::Will, Will);
}

void ATestForMinigamePlayerState::OnRep_Knowledge()
{
    if (GEngine)
    {
        FString Message = FString::Printf(TEXT("[%s] 知识属性: %d"), *GetPlayerName(), Knowledge);
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, Message);
    }
    OnAttributeChanged.Broadcast(EPlayerAttributeType::Knowledge, Knowledge);
}

// 辅助函数实现
void ATestForMinigamePlayerState::ClampHealth()
{
    Health = FMath::Clamp(Health, 0.0f, 100.0f);
}

void ATestForMinigamePlayerState::ClampSanValue()
{
    SanValue = FMath::Clamp(SanValue, 0.0f, 100.0f);
}

void ATestForMinigamePlayerState::ClampAttribute(int32& AttributeValue)
{
    AttributeValue = FMath::Clamp(AttributeValue, 0, 10);
}