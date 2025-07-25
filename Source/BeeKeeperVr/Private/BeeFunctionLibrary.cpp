// Fill out your copyright notice in the Description page of Project Settings.


#include "BeeFunctionLibrary.h"
#include "../BeeGenetic.h"
#include "BeeKeeperVr/Items/CombRecipe.h"

UTexture2D* UBeeFunctionLibrary::GetIcon(UDataTable* IconsTable, const Species Species)
{
	if (!IconsTable)
		return nullptr;

	const FName Name = UEnum::GetValueAsName(Species);
	if (const auto ObjectData = IconsTable->FindRow<FIcon>(Name, {}))
		return ObjectData->Icon;
	
	return nullptr;
}

void UBeeFunctionLibrary::SortHoneycombs(UPARAM(ref) TMap<ECombType, int> &Honeycombs)
{
  Honeycombs.ValueSort([](const int32 Right, const int32 Left) {return Right > Left; });
}

void UBeeFunctionLibrary::AccumulateCombs(const TMap<ECombType, int>& Honeycombs,
                                          UPARAM(ref) TMap<ECombType, FAccumulatorComb>& Accumulator,
                                          UDataTable* RecipeTable)
{
	for (const auto &[CombType, Amount] : Honeycombs)
	{
		const auto Recipe = RecipeTable->FindRow<FCombRecipe>(*UEnum::GetDisplayValueAsText(CombType).ToString(), {});
		if (!Recipe)
		{
			UE_LOG(LogTemp, Error, TEXT("Recipe item for CombType <%s> not found"),
			       *UEnum::GetDisplayValueAsText(CombType).ToString());
			continue;
		}
		auto *AccumulatorComb =	Accumulator.Find(CombType);
		if (!AccumulatorComb)
			AccumulatorComb = &Accumulator.Add(CombType);
		if (AccumulatorComb->Items.IsEmpty())
			for (const auto &RecipeItem : Recipe->RecipeItems)
				AccumulatorComb->Items.Add({ RecipeItem.ItemType, 0, 0, 0 });

		for (const auto &RecipeItem : Recipe->RecipeItems)
		{
			if (FAccumulatorCombItem* CombItem = AccumulatorComb->Items.FindByPredicate(
				[&RecipeItem](const FAccumulatorCombItem& I) { return I.ItemType == RecipeItem.ItemType; }))
				CombItem->Amount += Amount;
			else
				UE_LOG(LogTemp, Error, TEXT("Comb Item not found"));
		}
	}
}

static void ProduceItemsFromLeftovers(const EItemType ItemType, int& Amount, const int Cost, int& RemainingCount,
                                      TMap<EItemType, int>& OutputItems)
{
	int NewItemCount = Amount / Cost;
	if (NewItemCount > RemainingCount)
		NewItemCount = RemainingCount;

    if (NewItemCount == 0)
        return;
	int* Count = OutputItems.Find(ItemType);
	if (!Count)
		Count = &OutputItems.Add(ItemType);
	*Count += NewItemCount;
	Amount -= Cost * NewItemCount;
	RemainingCount -= NewItemCount;
}

void UBeeFunctionLibrary::ProduceItemsFromCombs(UPARAM(ref) TMap<ECombType, FAccumulatorComb>& Accumulator,
                                                UPARAM(ref) TMap<EItemType, int>& OutputItems, UDataTable* RecipeTable)
{
	constexpr int CombSize = 100;
	for (auto& [CombType, AccumulatorComb] : Accumulator)
	{
		const FCombRecipe* Recipes = RecipeTable->FindRow<FCombRecipe>(*UEnum::GetDisplayValueAsText(CombType).ToString(), {});
		if (!Recipes)
		{
			UE_LOG(LogTemp, Error, TEXT("Recipe item for CombType <%s> not found"),
			       *UEnum::GetDisplayValueAsText(CombType).ToString());
			continue;
		}
		for (auto& [ItemType, Amount, Cost, RemainingCount] : AccumulatorComb.Items)
		{
			// Step 1: Try to make items from prev attempt roll
			if (RemainingCount > 0 && Amount > 0)
				ProduceItemsFromLeftovers(ItemType, Amount, Cost, RemainingCount, OutputItems);
			// Step 2: Generate items from full combs
			if (Amount > 0 && Amount / CombSize > 0)
				if (const FCombRecipeItem* Recipe = Recipes->RecipeItems.FindByPredicate(
					[ItemType](const FCombRecipeItem& Left) {return Left.ItemType == ItemType; }))
				{
					// Roll for each full comb
					for (int i = 0; i < Amount / CombSize; ++i)
					{
						int* Count = OutputItems.Find(ItemType);
						if (!Count)
							Count = &OutputItems.Add(ItemType);
						*Count += FMath::RandRange(Recipe->Min, Recipe->Max);
					}
					Amount %= CombSize;
				}
			// Step 3: Try to make items from the remains and for future attempt
			if (RemainingCount == 0 && Amount > 0)
				if (const FCombRecipeItem* Recipe = Recipes->RecipeItems.FindByPredicate(
					[ItemType](const FCombRecipeItem& Left) {return Left.ItemType == ItemType; }))
				{
					RemainingCount = FMath::RandRange(Recipe->Min, Recipe->Max);
					Cost = CombSize / RemainingCount;
					Amount -= CombSize - Cost * RemainingCount; // In case of rounding like 100 - 33 * 3
					ProduceItemsFromLeftovers(ItemType, Amount, Cost, RemainingCount, OutputItems);
				}
		}
	}
}