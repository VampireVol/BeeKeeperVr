// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Engine/DataTable.h"
#include "BeeGenetic.generated.h"

UENUM(BlueprintType)
enum Species
{
	Meadow UMETA(DisplayName = "Meadow"),
	Forest UMETA(DisplayName = "Forest"),
	River UMETA(DisplayName = "River"),
	Plant UMETA(DisplayName = "Plant"),
	Mushroom UMETA(DisplayName = "Mushroom"),
	Berry UMETA(DisplayName = "Berry"),
	Woody UMETA(DisplayName = "Woody"),
	Potato UMETA(DisplayName = "Potato"),
	Wheat UMETA(DisplayName = "Wheat"),
	Grape UMETA(DisplayName = "Grape"),
	Tomato UMETA(DisplayName = "Tomato"),
	Flour UMETA(DisplayName = "Flour"),
	Plank UMETA(DisplayName = "Plank"),
	Paper UMETA(DisplayName = "Paper"),
	Bread UMETA(DisplayName = "Bread"),
	Wine UMETA(DisplayName = "Wine"),
	Boxed UMETA(DisplayName = "Boxed"),
	Ketchup UMETA(DisplayName = "Ketchup")
};

USTRUCT()
struct FIcon : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor Main;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor MainDark;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor Sec;
};

UENUM(BlueprintType)
enum class EyeColor : uint8
{
	Black UMETA(DisplayName = "Black"),
	Red UMETA(DisplayName = "Red"),
	Green UMETA(DisplayName = "Green"),
	Blue UMETA(DisplayName = "Blue"),
	Size UMETA(DisplayName = "Size")
};

UENUM(BlueprintType)
enum class EffectType : uint8
{
	None UMETA(DisplayName = "Black"),
	Speed UMETA(DisplayName = "Black"),
	Mutation UMETA(DisplayName = "Black"),
	Stability UMETA(DisplayName = "Black"),
	Fertility UMETA(DisplayName = "Black"),
	Infertility UMETA(DisplayName = "Black"),
	Size UMETA(DisplayName = "Size")
};

static TMultiMap<int32, TEnumAsByte<Species>> Tiers{
	{1, Species::Meadow},
	{1, Species::Forest},
	{1, Species::River},
	{2, Species::Plant},
	{2, Species::Mushroom},
	{2, Species::Berry},
	{2, Species::Woody},
	{3, Species::Potato},
	{3, Species::Wheat},
	{3, Species::Grape},
	{3, Species::Tomato},
	{4, Species::Flour},
	{4, Species::Plank},
	{4, Species::Paper},
	{5, Species::Bread},
	{5, Species::Wine},
	{5, Species::Boxed},
	{5, Species::Ketchup},
};

static TMap<int32, int32> CombCostByTier{
 {1, 100},
 {2, 200},
 {3, 500},
 {4, 800},
 {5, 1200},
};

static TMap<int32, float> SpeedCoeff{
	{0, 0.0f},
	{1, 0.5f},
	{2, 1.0f},
	{3, 1.5f},
	{4, 2.0f},
	{5, 2.5f},
	{6, 3.0f},
	{7, 3.5f},
	{8, 4.0f},
};

static TMap<TEnumAsByte<Species>, bool> DiscoveredSpecies{
	{Species::Meadow, false},
	{Species::Forest, false},
	{Species::River, false},
	{Species::Plant, false},
	{Species::Mushroom, false},
	{Species::Berry, false},
	{Species::Woody, false},
	{Species::Potato, false},
	{Species::Wheat, false},
	{Species::Grape, false},
	{Species::Tomato, false},
	{Species::Flour, false},
	{Species::Plank, false},
	{Species::Paper, false},
	{Species::Bread, false},
	{Species::Wine, false},
	{Species::Boxed, false},
	{Species::Ketchup, false},
};

USTRUCT(BlueprintType)
struct FBeeColors
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeeColor")
	FColor Main;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeeColor")
	FColor MainDark;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeeColor")
	FColor Sec;

	FBeeColors() {}
	FBeeColors(FString MainStr, FString MainDarkStr, FString SecStr)
	{
		Main = FColor::FromHex(MainStr);
		MainDark = FColor::FromHex(MainDarkStr);
		Sec = FColor::FromHex(SecStr);
	}
};

static TMap<TEnumAsByte<Species>, FBeeColors> SpeciesColors{
	{Species::Meadow, FBeeColors("CD3B0000", "46180000", "08080800")},
	{Species::Forest, FBeeColors("CD3B0000", "46180000", "04557500")},
	{Species::River, FBeeColors("001DE700", "000E69FF", "08080800")},
	{Species::Plant, FBeeColors("CD3B0000", "46180000", "08080800") },
	{Species::Mushroom, FBeeColors("CD3B0000", "46180000", "08080800") },
	{Species::Berry, FBeeColors("CD3B0000", "46180000", "08080800") },
	{Species::Woody, FBeeColors("CD3B0000", "46180000", "08080800") },
	{Species::Potato, FBeeColors("CD3B0000", "46180000", "08080800") },
	{Species::Wheat, FBeeColors("CD3B0000", "46180000", "08080800") },
	{Species::Grape, FBeeColors("CD3B0000", "46180000", "08080800") },
	{Species::Tomato, FBeeColors("CD3B0000", "46180000", "08080800") },
	{Species::Flour, FBeeColors("CD3B0000", "46180000", "08080800") },
	{Species::Plank, FBeeColors("CD3B0000", "46180000", "08080800") },
	{Species::Paper, FBeeColors("CD3B0000", "46180000", "08080800") },
	{Species::Bread, FBeeColors("CD3B0000", "46180000", "08080800") },
	{Species::Wine, FBeeColors("CD3B0000", "46180000", "08080800") },
	{Species::Boxed, FBeeColors("CD3B0000", "46180000", "08080800") },
	{Species::Ketchup, FBeeColors("CD3B0000", "46180000", "08080800") },
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BEEKEEPERVR_API UBeeGenetic : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBeeGenetic();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeeModel")
	TEnumAsByte<Species> Main = Species::Meadow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeeModel")
	TEnumAsByte<Species> Sec = Species::Meadow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeeModel")
	int32 Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeeModel")
	int32 Fertility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeeModel")
	EyeColor EyeMain = EyeColor::Black;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeeModel")
	EyeColor EyeSec = EyeColor::Black;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeeModel")
	EffectType EffectMain = EffectType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeeModel")
	EffectType EffectSec = EffectType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeeModel")
	int32 Radius;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Init(TEnumAsByte<Species> main, TEnumAsByte<Species> sec, int32 speed, int32 fertility);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Bees")
	int32 GetSpeedValue() const;
	UFUNCTION(BlueprintCallable, Category = "Bees")
	int32 GetFertiliryValue() const;
	UFUNCTION(BlueprintCallable, Category = "Bees")
	int32 GetRadiusValue() const;
	UFUNCTION(BlueprintCallable, Category = "Bees")
	static UBeeGenetic* Construct(TEnumAsByte<Species> _main, TEnumAsByte<Species> _sec, int32 _speed, int32 _fertility);
	UFUNCTION(BlueprintCallable, Category = "Bees")
	FString GetInfoBee();

	UFUNCTION(BlueprintCallable, Category = "Bees")
	FString GetInfoSpecies();
	UFUNCTION(BlueprintCallable, Category = "Bees")
	FString GetInfoSpeed();
	UFUNCTION(BlueprintCallable, Category = "Bees")
	FString GetInfoFertility();

	UFUNCTION(BlueprintCallable, Category = "Bees")
	bool GetGenSpeedValue(int32 index);
	UFUNCTION(BlueprintCallable, Category = "Bees")
	bool GetGenFertilityValue(int32 index);

	UFUNCTION(BlueprintCallable, Category = "Bees")
	int32 GetProductivitySpeed();

	UFUNCTION(BlueprintCallable, Category = "Bees")
	static UBeeGenetic* CreateMeadowBee();

	UFUNCTION(BlueprintCallable, Category = "Bees")
	static UBeeGenetic* CreateForestBee();

	UFUNCTION(BlueprintCallable, Category = "Bees")
	static UBeeGenetic* CreateRiverBee();

	UFUNCTION(BlueprintCallable, Category = "Bees Discover")
	static bool IsDiscoveredSpecies(TEnumAsByte<Species> species);

	UFUNCTION(BlueprintCallable, Category = "Bees")
	static FString GetSpeciesString(TEnumAsByte<Species> species);

	UFUNCTION(BlueprintCallable, Category = "Bees Discover")
	static void DiscoverSpecies(UBeeGenetic *bee);

	UFUNCTION(BlueprintCallable, Category = "Bees Discover")
	static void DiscoverSpeciesType(TEnumAsByte<Species> species);

	UFUNCTION(BlueprintCallable, Category = "Bees Discover")
	static int32 DiscoveredCount();

	UFUNCTION(BlueprintCallable, Category = "Bees Discover")
	static void ClearDiscoveredSpecies();

	UFUNCTION(BlueprintCallable, Category = "Bees")
	static FBeeColors getBeeColors(TEnumAsByte<Species> species);

	UFUNCTION(BlueprintCallable, Category = "Bees")
	static void sortBees(UPARAM(ref) TArray<UBeeGenetic *> &bees);
};
