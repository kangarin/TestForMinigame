// ItemDataAsset.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

UCLASS(BlueprintType)
class TESTFORMINIGAME_API UItemDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
    FString UniqueID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
    FString ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
    UTexture2D* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
    int32 MaxStackSize = 1;
};