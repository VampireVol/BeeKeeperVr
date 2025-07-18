// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ECombType.h"
#include "EItemType.h"
#include "Engine/DataTable.h"
#include "CombRecipe.generated.h"

USTRUCT(BlueprintType)
struct BEEKEEPERVR_API FCombRecipeItem
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Min;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Max;
};

USTRUCT(BlueprintType)
struct BEEKEEPERVR_API FCombRecipe : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECombType CombType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCombRecipeItem> RecipeItems;
};
