// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Items/ItemData.h"
#include "../BeeGenetic.h"
#include "Engine/DataTable.h"
#include "ResearchData.generated.h"

USTRUCT(BlueprintType)
struct BEEKEEPERVR_API FResearchData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemData> RequestItems;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RequestDnaPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ResearchName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D *Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Caption;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ResearchDependency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Endless;
};

