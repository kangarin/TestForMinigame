// SimpleInventoryComponent.cpp
#include "SimpleInventoryComponent.h"

USimpleInventoryComponent::USimpleInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USimpleInventoryComponent::AddItem(const FSimpleItem& Item)
{

    // 检查是否已有相同名称的物品
    for (FSimpleItem& ExistingItem : Items)
    {
        if (ExistingItem.UniqueID == Item.UniqueID)
        {
            // 如果找到相同物品，增加数量
            ExistingItem.Quantity += Item.Quantity;
            UE_LOG(LogTemp, Warning, TEXT("Added %d %s, total: %d"), Item.Quantity, *Item.ItemName, ExistingItem.Quantity);
            return;
        }
    }

    // 如果没找到相同物品，添加新物品
    Items.Add(Item);
    UE_LOG(LogTemp, Warning, TEXT("Added new item: %s %d %s"), *Item.UniqueID, Item.Quantity, *Item.ItemName);
}

bool USimpleInventoryComponent::RemoveItem(FString UniqueID, int32 Quantity)
{
    for (int32 i = 0; i < Items.Num(); i++)
    {
        if (Items[i].UniqueID == UniqueID)
        {
            if (Items[i].Quantity >= Quantity)
            {
                Items[i].Quantity -= Quantity;
                UE_LOG(LogTemp, Warning, TEXT("Removed %d %s, remaining: %d"), Quantity, *UniqueID, Items[i].Quantity);

                // 如果数量为0，从数组中移除
                if (Items[i].Quantity <= 0)
                {
                    Items.RemoveAt(i);
                    UE_LOG(LogTemp, Warning, TEXT("%s completely removed from inventory"), *UniqueID);
                }
                return true;
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Not enough %s to remove. Have: %d, Want: %d"), *UniqueID, Items[i].Quantity, Quantity);
                return false;
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Item %s not found in inventory"), *UniqueID);
    return false;
}

void USimpleInventoryComponent::PrintInventory()
{
    UE_LOG(LogTemp, Warning, TEXT("=== Inventory Contents ==="));

    if (Items.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Inventory is empty"));
        return;
    }

    for (int32 i = 0; i < Items.Num(); i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("%d. %s x%d"), i + 1, *Items[i].ItemName, Items[i].Quantity);
    }

    UE_LOG(LogTemp, Warning, TEXT("========================"));
}