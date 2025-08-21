#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemDataAsset.h"
#include "SimpleInventoryComponent.generated.h"

// 背包槽位结构
USTRUCT(BlueprintType)
struct FInventorySlot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UItemDataAsset* ItemData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Quantity;

    FInventorySlot()
    {
        ItemData = nullptr;
        Quantity = 0;
    }

    bool IsEmpty() const
    {
        return ItemData == nullptr || Quantity <= 0;
    }
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TESTFORMINIGAME_API USimpleInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USimpleInventoryComponent();

    // 背包槽位数组
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    TArray<FInventorySlot> Slots;

    // 背包最大容量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 MaxSlots = 20;

    // 预配置的物品（可在蓝图中设置）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    TArray<UItemDataAsset*> ConfiguredItems;

protected:
    virtual void BeginPlay() override;

public:
    // 通过ItemDataAsset添加物品
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool AddItem(UItemDataAsset* ItemData, int32 Quantity = 1);

    // 通过索引添加预配置的物品
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool AddConfiguredItem(int32 ItemIndex, int32 Quantity = 1);

    // 移除物品
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool RemoveItem(FString UniqueID, int32 Quantity = 1);

    // 获取物品数量
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    int32 GetItemQuantity(FString UniqueID);

    // 打印背包内容
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void PrintInventory();

private:
    // 尝试堆叠物品
    bool TryStackItem(UItemDataAsset* ItemData, int32& Quantity);

    // 获取空槽位索引
    int32 GetEmptySlotIndex();
};