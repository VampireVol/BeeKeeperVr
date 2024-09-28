// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/Texture2D.h"
#include "BeeFunctionLibrary.generated.h"

enum Species;

/**
 * 
 */
UCLASS()
class BEEKEEPERVR_API UBeeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="Bees")
	static UTexture2D* GetIcon(UDataTable* IconsTable, Species Species);
};
