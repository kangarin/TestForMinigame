// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintInteractableActor.h"

// Sets default values
ABlueprintInteractableActor::ABlueprintInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    // 设置网络复制
    bReplicates = true;
    bIsInteracted = false;

    // 默认值
    InteractionPrompt = TEXT("按E交互");
    bCanInteractMultipleTimes = false;
    bDestroyAfterInteraction = false;

    // 创建根组件（静态网格体）
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // 创建交互范围球体
    InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
    InteractionSphere->SetupAttachment(RootComponent);
    InteractionSphere->SetSphereRadius(200.0f);
    InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InteractionSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    InteractionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    InteractionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

    // 绑定碰撞事件
    InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABlueprintInteractableActor::OnSphereBeginOverlap);
    InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &ABlueprintInteractableActor::OnSphereEndOverlap);
}

void ABlueprintInteractableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ABlueprintInteractableActor, bIsInteracted);
}

bool ABlueprintInteractableActor::CanInteract() const
{
    if (bCanInteractMultipleTimes)
    {
        return IsValid(this);
    }
    return !bIsInteracted && IsValid(this);
}

void ABlueprintInteractableActor::ExecuteInteraction(APawn* InteractingPlayer)
{
    if (HasAuthority())  // 服务器端
    {
        // 1. 执行服务器逻辑（数据修改、业务逻辑）
        BP_ExecuteServerLogic(InteractingPlayer);

        // 2. 多播给所有客户端播放效果（音效、特效）
        MulticastPlayEffects(InteractingPlayer);

        // 3. 发送给特定客户端的反馈（UI、本地效果）
        APlayerController* PC = InteractingPlayer->GetController<APlayerController>();
        if (PC)
        {
            // Set the owner temporarily to the player controller
            SetOwner(PC);
            ClientShowFeedback(InteractingPlayer);
            SetOwner(nullptr); // Reset owner if needed
        }

        // 4. 默认行为
        if (!bCanInteractMultipleTimes)
        {
            bIsInteracted = true;
        }

        if (bDestroyAfterInteraction)
        {
            // 延迟销毁，让效果先播放
            FTimerHandle DestroyTimer;
            GetWorld()->GetTimerManager().SetTimer(DestroyTimer, [this]()
                {
                    Destroy();
                }, 0.5f, false);
        }

        // 调试信息
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
                FString::Printf(TEXT("Server: Interaction executed by %s"),
                    InteractingPlayer ? *InteractingPlayer->GetName() : TEXT("Unknown")));
        }
    }
}

void ABlueprintInteractableActor::MulticastPlayEffects_Implementation(APawn* InteractingPlayer)
{
    // 在所有客户端执行（包括服务器）
    BP_PlayInteractionEffects(InteractingPlayer);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue,
            TEXT("All Clients: Playing interaction effects (sound, particles, etc.)"));
    }
}

void ABlueprintInteractableActor::ClientShowFeedback_Implementation(APawn* InteractingPlayer)
{
    // 只在交互玩家的客户端执行
    BP_HandleClientFeedback(InteractingPlayer, true);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow,
            TEXT("Local Client: Showing UI feedback"));
    }
}

FString ABlueprintInteractableActor::GetInteractionPrompt() const
{
    return InteractionPrompt;
}

void ABlueprintInteractableActor::SetInteractionPrompt(const FString& NewPrompt)
{
    InteractionPrompt = NewPrompt;
}

void ABlueprintInteractableActor::MarkAsInteracted()
{
    if (HasAuthority())
    {
        bIsInteracted = true;
    }
}

void ABlueprintInteractableActor::ResetInteraction()
{
    if (HasAuthority())
    {
        bIsInteracted = false;
    }
}

void ABlueprintInteractableActor::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    APawn* Pawn = Cast<APawn>(OtherActor);
    if (Pawn && Pawn->IsPlayerControlled())
    {
        PlayersInRange.AddUnique(Pawn);

        APlayerController* PC = Pawn->GetController<APlayerController>();
        if (PC && PC->IsLocalPlayerController())
        {
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, GetInteractionPrompt());
            }
        }
    }
}

void ABlueprintInteractableActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    APawn* Pawn = Cast<APawn>(OtherActor);
    if (Pawn && Pawn->IsPlayerControlled())
    {
        PlayersInRange.Remove(Pawn);

        APlayerController* PC = Pawn->GetController<APlayerController>();
        if (PC && PC->IsLocalPlayerController())
        {
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, TEXT("离开交互范围"));
            }
        }
    }
}