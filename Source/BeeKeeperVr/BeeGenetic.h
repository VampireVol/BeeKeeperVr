// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "BeeGenetic.generated.h"

UENUM(BlueprintType)
enum Species
{
	Meadow UMETA(DisplayName = "Meadow"),
	Forest UMETA(DisplayName = "Forest"),
	River UMETA(DisplayName = "River"),
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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BEEKEEPERVR_API UBeeGenetic : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBeeGenetic();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeeModel")
	TEnumAsByte<Species> Main = Meadow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeeModel")
	TEnumAsByte<Species> Sec = Meadow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeeModel")
	int32 Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeeModel")
	int32 Fertility;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Init(TEnumAsByte<Species> main, TEnumAsByte<Species> sec, int32 speed, int32 fertility);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Bees")
	int32 GetSpeedValue();
	UFUNCTION(BlueprintCallable, Category = "Bees")
	int32 GetFertiliryValue();
	static UBeeGenetic* Construct(TEnumAsByte<Species> main, TEnumAsByte<Species> sec, int32 speed, int32 fertility);
	UFUNCTION(BlueprintCallable, Category = "Bees")
	FString GetInfoBee();
};
