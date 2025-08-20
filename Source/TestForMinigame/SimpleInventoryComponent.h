// SimpleInventoryComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimpleInventoryComponent.generated.h"

// 简单的物品结构
USTRUCT(BlueprintType)
struct FSimpleItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString UniqueID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Quantity;

    FSimpleItem()
    {
		UniqueID = TEXT("myid");
        ItemName = TEXT("");
        Quantity = 0;
    }
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TESTFORMINIGAME_API USimpleInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USimpleInventoryComponent();

    // 背包物品数组
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    TArray<FSimpleItem> Items;

    // 添加物品 - 接受FSimpleItem结构体
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void AddItem(const FSimpleItem& Item);

    // 移除物品
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool RemoveItem(FString UniqueID, int32 Quantity);

    // 打印背包内容（用于测试）
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void PrintInventory();
};