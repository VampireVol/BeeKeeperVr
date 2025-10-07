// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BeeGenetic.h"
#include "InheritMutationLibrary.generated.h"

/**
 * 
 */
UCLASS()
class BEEKEEPERVR_API UInheritMutationLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, Category = "Bees")
	static UBeeGenetic *Inherit(const UBeeGenetic *p1, const UBeeGenetic *p2);

	UFUNCTION(BlueprintCallable, Category = "Bees")
	static UBeeGenetic *CopyBeeProps(const UBeeGenetic *from);

	UFUNCTION(BlueprintCallable, Category = "Bees")
	static int32 GetDNAPoints(const UBeeGenetic *bee);
};
