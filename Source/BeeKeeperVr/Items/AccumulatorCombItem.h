// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EItemType.h"
#include "AccumulatorCombItem.generated.h"

USTRUCT(BlueprintType)
struct BEEKEEPERVR_API FAccumulatorCombItem
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Cost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RemainingCount;
};

USTRUCT(BlueprintType)
struct BEEKEEPERVR_API FAccumulatorComb
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAccumulatorCombItem> Items;
};
