// Fill out your copyright notice in the Description page of Project Settings.


#include "BeeGenetic.h"

const float COMB_AMOUNT_PER_MINUTE = 0.5f;

// Sets default values for this component's properties
UBeeGenetic::UBeeGenetic()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UBeeGenetic::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UBeeGenetic::Init(TEnumAsByte<Species> main, TEnumAsByte<Species> sec, int32 speed, int32 fertility)
{
	Main = main;
	Sec = sec;
	Speed = speed;
	Fertility = fertility;
}

// Called every frame
void UBeeGenetic::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int32 UBeeGenetic::GetSpeedValue()
{
	int32 count = 0;
	int32 mask = 1;
	for (int i = 0; i < 8; ++i)
	{
		if (mask & Speed)
			++count;
		mask <<= 1;
	}
	return count;
}

int32 UBeeGenetic::GetFertiliryValue()
{
	int32 count = 0;
	int32 maskMain = 1;
	int32 maskSec = 2;
	for (int i = 0; i < 5; ++i)
	{
		if (maskMain & Fertility || maskSec & Fertility)
			++count;
		maskMain <<= 2;
		maskSec <<= 2;
	}
	return count;
}

UBeeGenetic *UBeeGenetic::Construct(TEnumAsByte<Species> main, TEnumAsByte<Species> sec, int32 speed, int32 fertility)
{
	UBeeGenetic *bee = NewObject<UBeeGenetic>();
	bee->Init(main, sec, speed, fertility);
	return bee;
}

//TODO: move this to helper functions
const int32 SPEED_GENS_COUNT = 8;
const int32 FERTILITRY_GENS_COUNT = 10;
static bool get_bit(int32 num, int32 pos)
{
	return (num >> pos) & 1U;
}

static FString get_gens_str(int32 gen, int32 gens_count)
{
	FString out;
	for (int i = 0; i < gens_count; ++i)
	{
		out += get_bit(gen, i) ? "1" : "0";
		if (i % 2 == 1)
			out += " ";
	}
	return out;
}

FString UBeeGenetic::GetInfoBee()
{
	FString out = GetInfoSpecies();
	out += ": " + GetInfoSpeed() + "; " + GetInfoFertility();
	return out;
}

FString UBeeGenetic::GetInfoSpecies()
{
	FString out;
	FString mainName = UEnum::GetValueAsString(Main);
	FString secName = UEnum::GetValueAsString(Sec);
	if (mainName.Equals(secName))
		out = mainName;
	else
		out = mainName + "-" + secName;
	out += " bee";
	return out;
}

FString UBeeGenetic::GetInfoSpeed()
{
	return FString::Printf(TEXT("Speed %d %s"), GetSpeedValue(), *get_gens_str(Speed, SPEED_GENS_COUNT));
}

FString UBeeGenetic::GetInfoFertility()
{
	return FString::Printf(TEXT("Fertility %d %s"), GetFertiliryValue(), *get_gens_str(Fertility, FERTILITRY_GENS_COUNT));
}


bool UBeeGenetic::GetGenSpeedValue(int32 index)
{
	return get_bit(Speed, index) == 1;
}

bool UBeeGenetic::GetGenFertilityValue(int32 index)
{
	return get_bit(Fertility, index) == 1;
}

int32 UBeeGenetic::GetProductivitySpeed()
{
	int32 out = 0;
	float speedCoeff = *SpeedCoeff.Find(GetSpeedValue());
	out += *CombCostByTier.Find(*Tiers.FindKey(Main)) * speedCoeff;
	if (Main != Sec)
	{
		out += *CombCostByTier.Find(*Tiers.FindKey(Sec)) * speedCoeff;
		out *= 0.5f;
	}
	return out * COMB_AMOUNT_PER_MINUTE;
}

UBeeGenetic *UBeeGenetic::CreateMeadowBee()
{
	return Construct(Species::Meadow, Species::Meadow, 15, 15);
}

UBeeGenetic *UBeeGenetic::CreateForestBee()
{
	return Construct(Species::Forest, Species::Forest, 51, 51);
}

UBeeGenetic *UBeeGenetic::CreateRiverBee()
{
	return Construct(Species::River, Species::River, 60, 60);
}

