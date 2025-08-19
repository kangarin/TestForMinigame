// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleBox.h"

ADestructibleBox::ADestructibleBox()
{
    // 设置立方体网格
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube"));
    if (CubeMesh.Succeeded())
    {
        MeshComponent->SetStaticMesh(CubeMesh.Object);
    }
}

void ADestructibleBox::ExecuteInteraction(APawn* InteractingPlayer)
{
    // 先调用父类的通用逻辑（标记为已交互）
    Super::ExecuteInteraction(InteractingPlayer);

    // 这个物体特有的逻辑：销毁自己
    if (HasAuthority())
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,
                FString::Printf(TEXT("DestructibleBox destroyed by %s"),
                    InteractingPlayer ? *InteractingPlayer->GetName() : TEXT("Unknown")));
        }

        // 销毁物体
        Destroy();
    }
}

FString ADestructibleBox::GetInteractionPrompt() const
{
    return TEXT("按E破坏箱子");
}