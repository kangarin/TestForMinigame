#include "SimpleInventoryComponent.h"

USimpleInventoryComponent::USimpleInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USimpleInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // 初始化背包槽位
    Slots.SetNum(MaxSlots);
    
    UE_LOG(LogTemp, Warning, TEXT("Inventory initialized with %d slots"), MaxSlots);
    UE_LOG(LogTemp, Warning, TEXT("Configured items count: %d"), ConfiguredItems.Num());
}

bool USimpleInventoryComponent::AddItem(UItemDataAsset* ItemData, int32 Quantity)
{
    if (!ItemData || Quantity <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid item data or quantity"));
        return false;
    }

    UE_LOG(LogTemp, Warning, TEXT("Trying to add %d x %s"), Quantity, *ItemData->ItemName);

    int32 RemainingQuantity = Quantity;

    // 如果物品可以堆叠，先尝试堆叠到现有槽位
    if (ItemData->MaxStackSize > 1)
    {
        TryStackItem(ItemData, RemainingQuantity);
    }

    // 如果还有剩余，添加到空槽位
    while (RemainingQuantity > 0)
    {
        int32 EmptySlotIndex = GetEmptySlotIndex();
        if (EmptySlotIndex == -1)
        {
            UE_LOG(LogTemp, Warning, TEXT("Inventory is full!"));
            return false;
        }

        // 计算这个槽位能放多少
        int32 AddQuantity = FMath::Min(RemainingQuantity, ItemData->MaxStackSize);
        
        // 设置槽位数据
        Slots[EmptySlotIndex].ItemData = ItemData;
        Slots[EmptySlotIndex].Quantity = AddQuantity;
        
        RemainingQuantity -= AddQuantity;

        UE_LOG(LogTemp, Warning, TEXT("Added %d x %s to slot %d"), AddQuantity, *ItemData->ItemName, EmptySlotIndex);
    }

    return true;
}

bool USimpleInventoryComponent::AddConfiguredItem(int32 ItemIndex, int32 Quantity)
{
    if (ItemIndex < 0 || ItemIndex >= ConfiguredItems.Num())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid item index: %d"), ItemIndex);
        return false;
    }

    if (!ConfiguredItems[ItemIndex])
    {
        UE_LOG(LogTemp, Warning, TEXT("Item at index %d is null"), ItemIndex);
        return false;
    }

    return AddItem(ConfiguredItems[ItemIndex], Quantity);
}

bool USimpleInventoryComponent::RemoveItem(FString UniqueID, int32 Quantity)
{
    for (int32 i = 0; i < Slots.Num(); i++)
    {
        FInventorySlot& Slot = Slots[i];
        
        if (!Slot.IsEmpty() && Slot.ItemData->UniqueID == UniqueID)
        {
            if (Slot.Quantity >= Quantity)
            {
                Slot.Quantity -= Quantity;
                UE_LOG(LogTemp, Warning, TEXT("Removed %d x %s from slot %d"), Quantity, *Slot.ItemData->ItemName, i);
                
                // 如果数量为0，清空槽位
                if (Slot.Quantity <= 0)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Slot %d is now empty"), i);
                    Slot.ItemData = nullptr;
                    Slot.Quantity = 0;
                }
                return true;
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Not enough items. Have: %d, Want: %d"), Slot.Quantity, Quantity);
                return false;
            }
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Item with ID %s not found"), *UniqueID);
    return false;
}

int32 USimpleInventoryComponent::GetItemQuantity(FString UniqueID)
{
    int32 TotalQuantity = 0;
    
    for (const FInventorySlot& Slot : Slots)
    {
        if (!Slot.IsEmpty() && Slot.ItemData->UniqueID == UniqueID)
        {
            TotalQuantity += Slot.Quantity;
        }
    }
    
    return TotalQuantity;
}

void USimpleInventoryComponent::PrintInventory()
{
    UE_LOG(LogTemp, Warning, TEXT("=== Inventory Contents ==="));
    
    bool HasItems = false;
    
    for (int32 i = 0; i < Slots.Num(); i++)
    {
        const FInventorySlot& Slot = Slots[i];
        if (!Slot.IsEmpty())
        {
            UE_LOG(LogTemp, Warning, TEXT("Slot %d: %s x%d (ID: %s)"), 
                   i, *Slot.ItemData->ItemName, Slot.Quantity, *Slot.ItemData->UniqueID);
            HasItems = true;
        }
    }
    
    if (!HasItems)
    {
        UE_LOG(LogTemp, Warning, TEXT("Inventory is empty"));
    }
    
    UE_LOG(LogTemp, Warning, TEXT("========================"));
}

bool USimpleInventoryComponent::TryStackItem(UItemDataAsset* ItemData, int32& Quantity)
{
    bool bStacked = false;

    // 遍历所有槽位，寻找可以堆叠的物品
    for (int32 i = 0; i < Slots.Num() && Quantity > 0; i++)
    {
        FInventorySlot& Slot = Slots[i];
        
        // 检查是否是相同物品且有堆叠空间
        if (!Slot.IsEmpty() && 
            Slot.ItemData->UniqueID == ItemData->UniqueID &&
            Slot.Quantity < ItemData->MaxStackSize)
        {
            // 计算可以堆叠的数量
            int32 AvailableSpace = ItemData->MaxStackSize - Slot.Quantity;
            int32 AddQuantity = FMath::Min(Quantity, AvailableSpace);
            
            // 增加数量
            Slot.Quantity += AddQuantity;
            Quantity -= AddQuantity;
            
            bStacked = true;
            
            UE_LOG(LogTemp, Warning, TEXT("Stacked %d x %s to slot %d, total: %d"), 
                   AddQuantity, *ItemData->ItemName, i, Slot.Quantity);
        }
    }

    return bStacked;
}

int32 USimpleInventoryComponent::GetEmptySlotIndex()
{
    for (int32 i = 0; i < Slots.Num(); i++)
    {
        if (Slots[i].IsEmpty())
        {
            return i;
        }
    }
    return -1;
}