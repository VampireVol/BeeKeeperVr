// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/Texture2D.h"
#include "../BeeGenetic.h"
#include "../ECombType.h"
#include "BeeKeeperVr/Items/AccumulatorCombItem.h"
#include "BeeFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class BEEKEEPERVR_API UBeeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="Bees")
	static UTexture2D* GetIcon(UDataTable* IconsTable, const Species Species);
  UFUNCTION(BlueprintCallable, Category = "Util")
  static void SortHoneycombs(UPARAM(ref) TMap<ECombType, int> &Honeycombs);

  UFUNCTION(BlueprintCallable, Category = "Util")
  static void AccumulateCombs(const TMap<ECombType, int>& Honeycombs, UPARAM(ref) TMap<ECombType, FAccumulatorComb>& Accumulator, UDataTable* RecipeTable);
  UFUNCTION(BlueprintCallable, Category = "Util")
  static void ProduceItemsFromCombs(UPARAM(ref) TMap<ECombType, FAccumulatorComb>& Accumulator, UPARAM(ref) TMap<EItemType, int>& OutputItems, UDataTable* RecipeTable);

  UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "Object"), Category = "Util")
  static bool CallFunctionByName(UObject *Object, FName FunctionName);
};
