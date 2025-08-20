// SimpleInventoryComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimpleInventoryComponent.generated.h"

// �򵥵���Ʒ�ṹ
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

    // ������Ʒ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    TArray<FSimpleItem> Items;

    // �����Ʒ - ����FSimpleItem�ṹ��
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void AddItem(const FSimpleItem& Item);

    // �Ƴ���Ʒ
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool RemoveItem(FString UniqueID, int32 Quantity);

    // ��ӡ�������ݣ����ڲ��ԣ�
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void PrintInventory();
};