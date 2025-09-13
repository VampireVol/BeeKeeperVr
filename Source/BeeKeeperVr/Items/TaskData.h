// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "../BeeGenetic.h"
#include "Engine/DataTable.h"
#include "TaskData.generated.h"

USTRUCT(BlueprintType)
struct BEEKEEPERVR_API FTaskData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemData> RequestItems;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemData RewardItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<Species> RewardBee;
};

