// Fill out your copyright notice in the Description page of Project Settings.


#include "BeeFunctionLibrary.h"
#include "../BeeGenetic.h"
#include "BeeKeeperVr/Items/CombRecipe.h"
#include "BeeKeeperVr/Items/ItemData.h"
#include "Math/RandomStream.h"

UTexture2D* UBeeFunctionLibrary::GetIcon(UDataTable* IconsTable, const Species Species)
{
	if (!IconsTable)
		return nullptr;

	const FName Name = UEnum::GetValueAsName(Species);
	if (const auto ObjectData = IconsTable->FindRow<FIcon>(Name, {}))
		return ObjectData->Icon;
	
	return nullptr;
}

UTexture2D* UBeeFunctionLibrary::GetItemIcon(UDataTable *IconsTable, const EItemType ItemType, UTexture2D *DefaultIcon)
{
	if (!IconsTable)
		return nullptr;

	const UEnum *EnumPtr = StaticEnum<EItemType>();
	const FName Name = FName(*EnumPtr->GetNameStringByValue(static_cast<int64>(ItemType)));
	if (const auto ObjectData = IconsTable->FindRow<FItemTableData>(Name, {}))
		return ObjectData->Icon;

	return DefaultIcon;
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

// Inspiration from: https://forums.unrealengine.com/t/call-function-by-name-here-is-it/1715979
bool UBeeFunctionLibrary::CallFunctionByName(UObject *Object, FName FunctionName)
{
	if (Object)
	{
		if (UFunction *Function = Object->FindFunction(FunctionName))
    {
      Object->ProcessEvent(Function, nullptr); 
			return true;
		}
		else
    {
      UE_LOG(LogTemp, Error, TEXT("Trying to run function named %s but is not found"), *FunctionName.ToString());
      if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Trying to run function named <%s> but is not found"), *FunctionName.ToString()));
      return false;
    }
	} 
	return false;
}

FTimerHandle UBeeFunctionLibrary::SetTimerDelegateUnique(FTimerDynamicDelegate Delegate, float Time, bool bLooping, float InitialStartDelay, float InitialStartDelayVariance)
{
	FTimerHandle Handle;
	if (Delegate.IsBound())
	{
		const UWorld *const World = GEngine->GetWorldFromContextObject(Delegate.GetUObject(), EGetWorldErrorMode::LogAndReturnNull);
		if (World)
		{
			InitialStartDelay += FMath::RandRange(-InitialStartDelayVariance, InitialStartDelayVariance);
			if (Time <= 0.f || (Time + InitialStartDelay) < 0.f)
			{
				FString ObjectName = GetNameSafe(Delegate.GetUObject());
				FString FunctionName = Delegate.GetFunctionName().ToString();
				FFrame::KismetExecutionMessage(*FString::Printf(TEXT("%s %s SetTimer passed a negative or zero time. The associated timer may fail to be created/fire! If using InitialStartDelayVariance, be sure it is smaller than (Time + InitialStartDelay)."), *ObjectName, *FunctionName), ELogVerbosity::Warning);
			}

			FTimerManager &TimerManager = World->GetTimerManager();
			TimerManager.SetTimer(Handle, Delegate, Time, bLooping, (Time + InitialStartDelay));
		}
	}
	else
	{
		UE_LOG(LogBlueprintUserMessages, Warning,
			TEXT("SetTimer passed a bad function (%s) or object (%s)"),
			*Delegate.GetFunctionName().ToString(), *GetNameSafe(Delegate.GetUObject()));
	}

	return Handle;
}

// TODO: Replace with DataTable lookup (FCombColorData : FTableRowBase with FLinearColor Color)
static FColor GetCombColor(ECombType Type)
{
	switch (Type)
	{
	case ECombType::Honey:    return FColor(255, 200,   0, 255);
	case ECombType::Plant:    return FColor( 50, 200,  50, 255);
	case ECombType::Mushroom: return FColor(180, 120,  80, 255);
	case ECombType::Berry:    return FColor(180,  20, 100, 255);
	case ECombType::Woody:    return FColor(120,  80,  40, 255);
	case ECombType::Potato:   return FColor(200, 180, 100, 255);
	case ECombType::Wheat:    return FColor(230, 200,  80, 255);
	case ECombType::Grape:    return FColor(130,   0, 180, 255);
	case ECombType::Tomato:   return FColor(220,  40,  40, 255);
	case ECombType::Flour:    return FColor(240, 240, 230, 255);
	case ECombType::Plank:    return FColor(160, 110,  60, 255);
	case ECombType::Paper:    return FColor(250, 245, 220, 255);
	case ECombType::Bread:    return FColor(200, 150,  70, 255);
	case ECombType::Wine:     return FColor(100,   0,  60, 255);
	case ECombType::Boxed:    return FColor( 80, 130, 180, 255);
	case ECombType::Ketchup:  return FColor(190,  20,  20, 255);
	default:                  return FColor(  0,   0,   0,   0); // None — transparent
	}
}

UTexture2D* UBeeFunctionLibrary::GenerateCombTexture(int32 Seed, const TMap<ECombType, int32>& Distribution)
{
	constexpr int32 WorkPixels  = 32 * 25; // 800 — active surface
	constexpr int32 TotalPixels = 32 * 32; // 1024 — full texture (power of 2)

	// Step 1: Map % values → pixel counts (800 is divisible by 100, so no rounding error)
	struct FTypeCount { ECombType Type; int32 Count; };
	TArray<FTypeCount> Counts;

	for (const auto& [Type, Value] : Distribution)
	{
		if (Value <= 0) continue;
		const int32 Count = FMath::Clamp(Value, 0, 100) * WorkPixels / 100;
		Counts.Add({ Type, Count });
	}

	// Step 2: Build and Fisher-Yates shuffle — only working area [0..WorkPixels-1]
	TArray<int32> Indices;
	Indices.Reserve(WorkPixels);
	for (int32 i = 0; i < WorkPixels; i++)
		Indices.Add(i);

	FRandomStream RandStream(Seed);
	for (int32 i = WorkPixels - 1; i > 0; i--)
	{
		const int32 j = RandStream.RandRange(0, i);
		Indices.Swap(i, j);
	}

	// Step 3: Create transient texture
	UTexture2D* Texture = UTexture2D::CreateTransient(32, 32, PF_B8G8R8A8);
	if (!Texture)
		return nullptr;

	Texture->MipGenSettings = TMGS_NoMipmaps;
	Texture->CompressionSettings = TC_EditorIcon; // Uncompressed RGBA
	Texture->SRGB = false;
	Texture->Filter = TF_Nearest;

	// Step 4: Write pixel data (PF_B8G8R8A8 = B,G,R,A order in memory)
	FTexture2DMipMap& Mip = Texture->GetPlatformData()->Mips[0];
	uint8* Pixels = static_cast<uint8*>(Mip.BulkData.Lock(LOCK_READ_WRITE));

	// Init all pixels transparent (covers 800..1023 automatically)
	FMemory::Memzero(Pixels, TotalPixels * 4);

	// Write colors directly at shuffled positions
	int32 Offset = 0;
	for (const auto& [Type, Count] : Counts)
	{
		const FColor Color = GetCombColor(Type);
		for (int32 i = 0; i < Count; i++)
		{
			const int32 Idx = Indices[Offset++];
			Pixels[Idx * 4 + 0] = Color.B;
			Pixels[Idx * 4 + 1] = Color.G;
			Pixels[Idx * 4 + 2] = Color.R;
			Pixels[Idx * 4 + 3] = Color.A;
		}
	}

	Mip.BulkData.Unlock();
	Texture->UpdateResource();
	return Texture;
}
