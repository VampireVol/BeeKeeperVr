// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/StaticMeshComponent.h"
#include "BeeKeeperVr/ECombType.h"
#include "BeeKeeperVr/Public/BeeFunctionLibrary.h"
#include "FrameIconGeneratorComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BEEKEEPERVR_API UFrameIconGeneratorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFrameIconGeneratorComponent();

	// Call from BP_IconsGenerator BeginPlay: assign these refs before using GenerateFrameIcon
	UPROPERTY(BlueprintReadWrite, Category="Icon Generator")
	TObjectPtr<USceneCaptureComponent2D> CaptureComponent;

	UPROPERTY(BlueprintReadWrite, Category="Icon Generator")
	TObjectPtr<UStaticMeshComponent> FrameMesh;

	// InitRT from BP_IconsGenerator — SceneCapture writes here first
	UPROPERTY(BlueprintReadWrite, Category="Icon Generator")
	TObjectPtr<UTextureRenderTarget2D> IntermediateRT;

	// M_IconRenderTarget2D_Mat (or a DMI with IntermediateRT set as texture param)
	UPROPERTY(BlueprintReadWrite, Category="Icon Generator")
	TObjectPtr<UMaterialInterface> PostProcessMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Icon Generator")
	int32 PoolSize = 32;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Icon Generator")
	int32 RenderTargetSize = 256;

	// Main entry point — call from BP_IconsGenerator::GenerateFrameIcon after Break S_FrameData
	UFUNCTION(BlueprintCallable, Category="Icon Generator")
	UTextureRenderTarget2D* GenerateFrameIcon(
		int32 Seed,
		UPARAM(ref) TArray<FCombDistributionSnapshot>& History,
		const TMap<ECombType, int32>& Honeycombs);

	// Reset pool index — call when opening inventory to avoid slot reuse mid-frame
	UFUNCTION(BlueprintCallable, Category="Icon Generator")
	void ResetIndex();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<TObjectPtr<UTextureRenderTarget2D>> IconPool;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> FrameDMI;

	int32 NextPoolIndex = 0;

	void InitPool();
	void EnsureFrameDMI();
};
