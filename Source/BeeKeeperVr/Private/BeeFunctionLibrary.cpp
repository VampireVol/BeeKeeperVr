// Fill out your copyright notice in the Description page of Project Settings.


#include "BeeFunctionLibrary.h"
#include "../BeeGenetic.h"

UTexture2D* UBeeFunctionLibrary::GetIcon(UDataTable* IconsTable, Species Species)
{
	if (!IconsTable)
		return nullptr;

	const FName Name = UEnum::GetValueAsName(Species);
	if (const auto ObjectData = IconsTable->FindRow<FIcon>(Name, {}))
		return ObjectData->Icon;
	
	return nullptr;
}
