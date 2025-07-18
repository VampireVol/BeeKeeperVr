#pragma once

#include "CoreMinimal.h"
#include "ECombType.generated.h"

UENUM(BlueprintType)
enum class ECombType : uint8
{
	None UMETA(DisplayName = "None"),
	Honey UMETA(DisplayName = "Honey"),
	Plant UMETA(DisplayName = "Plant"),
	Mushroom UMETA(DisplayName = "Mushroom"),
	Berry UMETA(DisplayName = "Berry"),
	Woody UMETA(DisplayName = "Woody"),
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