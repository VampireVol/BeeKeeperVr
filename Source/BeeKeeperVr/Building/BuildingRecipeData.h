// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Items/ItemData.h"
#include "Engine/DataTable.h"
#include "BuildingRecipeData.generated.h"

UENUM(BlueprintType)
enum class EBuildingType : uint8
{
  None UMETA(DisplayName = "None"),
  Beehive UMETA(DisplayName = "Beehive"),
  Centrifuge UMETA(DisplayName = "Centrifuge"),
  BeeHotel UMETA(DisplayName = "Bee Hotel"),
  BeeAltar UMETA(DisplayName = "Bee Altar"),
  ExtractorDNA UMETA(DisplayName = "Extractor DNA"),
};

USTRUCT(BlueprintType)
struct BEEKEEPERVR_API FBuildingRecipeData : public FTableRowBase
{
  GENERATED_BODY()
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  EBuildingType Type;
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  UTexture2D *Icon;
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  TArray<FItemData> Cost;
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  TSubclassOf<class AActor> BuildingClass;
};
