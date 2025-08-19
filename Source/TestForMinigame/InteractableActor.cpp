// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // 设置网络复制
    bReplicates = true;
    bIsInteracted = false;

    // 创建根组件（静态网格体）
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // 设置网格体为立方体
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube"));
    if (CubeMesh.Succeeded())
    {
        MeshComponent->SetStaticMesh(CubeMesh.Object);
    }

    // 创建交互范围球体
    InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
    InteractionSphere->SetupAttachment(RootComponent);
    InteractionSphere->SetSphereRadius(200.0f); // 交互范围200单位
    InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InteractionSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    InteractionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    InteractionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

    // 绑定碰撞事件
    InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActor::OnSphereBeginOverlap);
    InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &AInteractableActor::OnSphereEndOverlap);


}


void AInteractableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AInteractableActor, bIsInteracted);
}

bool AInteractableActor::CanInteract() const
{
    return !bIsInteracted && IsValid(this);
}

void AInteractableActor::ExecuteInteraction(APawn* InteractingPlayer)
{
    // 基类的默认实现：标记为已交互，触发蓝图事件
    if (HasAuthority())
    {
        bIsInteracted = true;
        OnInteractionExecuted(InteractingPlayer);

        // 显示调试信息
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
                FString::Printf(TEXT("Base interaction executed by %s"),
                    InteractingPlayer ? *InteractingPlayer->GetName() : TEXT("Unknown")));
        }
    }
}

FString AInteractableActor::GetInteractionPrompt() const
{
    return TEXT("按E交互");
}

void AInteractableActor::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 检查是否为玩家Pawn
    APawn* Pawn = Cast<APawn>(OtherActor);
    if (Pawn && Pawn->IsPlayerControlled())
    {
        // 添加到交互范围内玩家列表
        PlayersInRange.AddUnique(Pawn);

        // 通知客户端显示交互提示
        APlayerController* PC = Pawn->GetController<APlayerController>();
        if (PC && PC->IsLocalPlayerController())
        {
            // 这里你可以触发UI显示"按E交互"
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, TEXT("按E交互"));
            }
        }
    }
}

void AInteractableActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // 检查是否为玩家Pawn
    APawn* Pawn = Cast<APawn>(OtherActor);
    if (Pawn && Pawn->IsPlayerControlled())
    {
        // 从交互范围内玩家列表移除
        PlayersInRange.Remove(Pawn);

        // 通知客户端隐藏交互提示
        APlayerController* PC = Pawn->GetController<APlayerController>();
        if (PC && PC->IsLocalPlayerController())
        {
            // 这里你可以触发UI隐藏交互提示
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, TEXT("离开交互范围"));
            }
        }
    }
}