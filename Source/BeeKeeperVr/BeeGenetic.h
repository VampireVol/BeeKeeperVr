// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BEEKEEPERVR_API UBeeGenetic : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBeeGenetic();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeeModel")
	TEnumAsByte<Species> Main = Meadow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeeModel")
	TEnumAsByte<Species> Sec = Meadow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeeModel")
	int32 Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeeModel")
	int32 Fertility;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Init(TEnumAsByte<Species> main, TEnumAsByte<Species> sec, int32 speed, int32 fertility);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Bees")
	int32 GetSpeedValue();
	UFUNCTION(BlueprintCallable, Category = "Bees")
	int32 GetFertiliryValue();
	static UBeeGenetic* Construct(TEnumAsByte<Species> main, TEnumAsByte<Species> sec, int32 speed, int32 fertility);
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
};
