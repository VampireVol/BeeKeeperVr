// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BeeGenetic.h"
#include "RestoredBeesSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FRestoredBeeQueueEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Bees|Restoration")
	TEnumAsByte<Species> QueueKey;

	UPROPERTY(BlueprintReadWrite, Category = "Bees|Restoration")
	TEnumAsByte<Species> Main;

	UPROPERTY(BlueprintReadWrite, Category = "Bees|Restoration")
	TEnumAsByte<Species> Sec;

	UPROPERTY(BlueprintReadWrite, Category = "Bees|Restoration")
	int32 Speed = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Bees|Restoration")
	int32 Fertility = 0;
};

USTRUCT()
struct FSpeciesGeneChances
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<int32> SpeedBitCounts;

	UPROPERTY()
	TArray<int32> FertilityBitCounts;
};

USTRUCT()
struct FSpeciesQueue
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TObjectPtr<UBeeGenetic>> Bees;

	FSpeciesGeneChances Chances;
};

UCLASS()
class BEEKEEPERVR_API URestoredBeesSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Bees|Restoration")
	void AddBee(UBeeGenetic* Bee);

	UFUNCTION(BlueprintCallable, Category = "Bees|Restoration")
	UBeeGenetic* GetBee(TEnumAsByte<Species> InSpecies);

	UFUNCTION(BlueprintCallable, Category = "Bees|Restoration")
	TArray<FRestoredBeeQueueEntry> GetSerializedQueues() const;

	UFUNCTION(BlueprintCallable, Category = "Bees|Restoration")
	void LoadSerializedQueues(const TArray<FRestoredBeeQueueEntry>& Data);

private:
	static constexpr int32 MaxQueueSize = 20;
	static constexpr int32 SpeedBitCount = 8;
	static constexpr int32 FertilityBitCount = 10;

	UPROPERTY()
	TMap<TEnumAsByte<Species>, FSpeciesQueue> SpeciesQueues;

	void AddToQueue(TEnumAsByte<Species> InSpecies, UBeeGenetic* Bee);
};
