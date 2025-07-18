#pragma once

#include "CoreMinimal.h"
#include "EItemType.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None UMETA(DisplayName = "None"),
	Honey UMETA(DisplayName = "Honey"),
	Wax UMETA(DisplayName = "Wax"),
	Mushroom UMETA(DisplayName = "Mushroom"),
	Berry UMETA(DisplayName = "Berry"),
	Wood UMETA(DisplayName = "Wood"),
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