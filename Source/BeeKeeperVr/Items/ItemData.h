// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EItemType.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct BEEKEEPERVR_API FItemData
{
  GENERATED_BODY()
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  EItemType Type;
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  int Count;
};

USTRUCT(BlueprintType)
struct BEEKEEPERVR_API FItemTableData : public FTableRowBase
{
  GENERATED_BODY()
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  EItemType Type;
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  UTexture2D *Icon;
};
