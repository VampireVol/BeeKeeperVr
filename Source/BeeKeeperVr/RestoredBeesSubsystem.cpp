// Fill out your copyright notice in the Description page of Project Settings.

#include "RestoredBeesSubsystem.h"

void URestoredBeesSubsystem::AddBee(UBeeGenetic* Bee)
{
	if (!Bee)
	{
		UE_LOG(LogTemp, Error, TEXT("RestoredBeesSubsystem::AddBee: Bee is null"));
		return;
	}

	AddToQueue(Bee->Main, Bee);

	if (Bee->Main != Bee->Sec)
	{
		AddToQueue(Bee->Sec, Bee);
	}
}

void URestoredBeesSubsystem::AddToQueue(TEnumAsByte<Species> InSpecies, UBeeGenetic* Bee)
{
	FSpeciesQueue& Queue = SpeciesQueues.FindOrAdd(InSpecies);

	if (Queue.Chances.SpeedBitCounts.IsEmpty())
	{
		Queue.Chances.SpeedBitCounts.Init(0, SpeedBitCount);
		Queue.Chances.FertilityBitCounts.Init(0, FertilityBitCount);
	}

	if (Queue.Bees.Num() >= MaxQueueSize)
	{
		UBeeGenetic* OldBee = Queue.Bees[0];
		for (int32 i = 0; i < Queue.Chances.SpeedBitCounts.Num(); ++i)
		{
			if ((OldBee->Speed >> i) & 1)
				--Queue.Chances.SpeedBitCounts[i];
		}
		for (int32 i = 0; i < Queue.Chances.FertilityBitCounts.Num(); ++i)
		{
			if ((OldBee->Fertility >> i) & 1)
				--Queue.Chances.FertilityBitCounts[i];
		}
		Queue.Bees.RemoveAt(0);
	}

	for (int32 i = 0; i < Queue.Chances.SpeedBitCounts.Num(); ++i)
	{
		if ((Bee->Speed >> i) & 1)
			++Queue.Chances.SpeedBitCounts[i];
	}
	for (int32 i = 0; i < Queue.Chances.FertilityBitCounts.Num(); ++i)
	{
		if ((Bee->Fertility >> i) & 1)
			++Queue.Chances.FertilityBitCounts[i];
	}

	Queue.Bees.Add(Bee);
}

UBeeGenetic* URestoredBeesSubsystem::GetBee(TEnumAsByte<Species> InSpecies)
{
	FSpeciesQueue* Queue = SpeciesQueues.Find(InSpecies);
	if (!Queue || Queue->Bees.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("RestoredBeesSubsystem::GetBee: No bees for species %s"),
			*UBeeGenetic::GetSpeciesString(InSpecies));
		return UBeeGenetic::Construct(InSpecies, InSpecies, 0, 0);
	}

	const int32 n = Queue->Bees.Num();
	int32 NewSpeed = 0;
	int32 NewFertility = 0;

	for (int32 i = 0; i < Queue->Chances.SpeedBitCounts.Num(); ++i)
	{
		if (FMath::RandHelper(n) < Queue->Chances.SpeedBitCounts[i])
			NewSpeed |= (1 << i);
	}
	for (int32 i = 0; i < Queue->Chances.FertilityBitCounts.Num(); ++i)
	{
		if (FMath::RandHelper(n) < Queue->Chances.FertilityBitCounts[i])
			NewFertility |= (1 << i);
	}

	return UBeeGenetic::Construct(InSpecies, InSpecies, NewSpeed, NewFertility);
}

TArray<FRestoredBeeQueueEntry> URestoredBeesSubsystem::GetSerializedQueues() const
{
	TArray<FRestoredBeeQueueEntry> Result;
	for (const auto& Pair : SpeciesQueues)
	{
		const TEnumAsByte<Species> QueueKey = Pair.Key;
		for (const UBeeGenetic* Bee : Pair.Value.Bees)
		{
			FRestoredBeeQueueEntry Entry;
			Entry.QueueKey = QueueKey;
			Entry.Main = Bee->Main;
			Entry.Sec = Bee->Sec;
			Entry.Speed = Bee->Speed;
			Entry.Fertility = Bee->Fertility;
			Result.Add(Entry);
		}
	}
	return Result;
}

void URestoredBeesSubsystem::LoadSerializedQueues(const TArray<FRestoredBeeQueueEntry>& Data)
{
	SpeciesQueues.Reset();
	for (const FRestoredBeeQueueEntry& Entry : Data)
	{
		UBeeGenetic* Bee = UBeeGenetic::Construct(Entry.Main, Entry.Sec, Entry.Speed, Entry.Fertility);
		AddToQueue(Entry.QueueKey, Bee);
	}
}
