// Fill out your copyright notice in the Description page of Project Settings.

#include "FrameIconGeneratorComponent.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/SceneCapture2D.h"

UFrameIconGeneratorComponent::UFrameIconGeneratorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFrameIconGeneratorComponent::BeginPlay()
{
	Super::BeginPlay();
	InitPool();
}

void UFrameIconGeneratorComponent::InitPool()
{
	IconPool.Empty();
	for (int32 i = 0; i < PoolSize; ++i)
	{
		UTextureRenderTarget2D* RT = NewObject<UTextureRenderTarget2D>(this);
		RT->InitAutoFormat(RenderTargetSize, RenderTargetSize);
		RT->UpdateResourceImmediate(true);
		IconPool.Add(RT);
	}
}

void UFrameIconGeneratorComponent::EnsureFrameDMI()
{
	if (FrameDMI || !FrameMesh)
	{
		return;
	}
	FrameDMI = FrameMesh->CreateDynamicMaterialInstance(1);
}

void UFrameIconGeneratorComponent::RenderFrameIcon(
	int32 Seed,
	TArray<FCombDistributionSnapshot>& History,
	const TMap<ECombType, int32>& Honeycombs,
	UTextureRenderTarget2D* TargetRT)
{
	// 1. Generate comb pattern texture
	UTexture2D* CombTexture = UBeeFunctionLibrary::GenerateCombTexture(Seed, History, Honeycombs);

	// 2. Apply comb texture to frame material
	EnsureFrameDMI();
	if (FrameDMI && CombTexture)
	{
		FrameDMI->SetTextureParameterValue(TEXT("CombTexture"), CombTexture);
	}

	// 3. Configure capture: show only the frame mesh
	CaptureComponent->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	CaptureComponent->ShowOnlyComponents.Empty();
	CaptureComponent->ShowOnlyComponents.Add(FrameMesh);
	CaptureComponent->TextureTarget = IntermediateRT;
	CaptureComponent->bCaptureEveryFrame = false;

	// 4. Capture the scene into IntermediateRT
	CaptureComponent->CaptureScene();

	// 5. Apply post-process material (background removal) and write to TargetRT
	TargetRT->UpdateResourceImmediate(true);
	UMaterialInstanceDynamic* PostDMI = UMaterialInstanceDynamic::Create(PostProcessMaterial, this);
	PostDMI->SetTextureParameterValue(TEXT("ItemTexture"), IntermediateRT);
	UKismetRenderingLibrary::DrawMaterialToRenderTarget(this, TargetRT, PostDMI);
}

UTextureRenderTarget2D* UFrameIconGeneratorComponent::GenerateFrameIcon(
	int32 Seed,
	TArray<FCombDistributionSnapshot>& History,
	const TMap<ECombType, int32>& Honeycombs)
{
	if (!CaptureComponent || !FrameMesh || !IntermediateRT || !PostProcessMaterial)
	{
		UE_LOG(LogTemp, Warning, TEXT("FrameIconGeneratorComponent: missing refs, call not possible"));
		return nullptr;
	}

	if (IconPool.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("FrameIconGeneratorComponent: pool is empty"));
		return nullptr;
	}

	UTextureRenderTarget2D* PoolRT = IconPool[NextPoolIndex % PoolSize];
	NextPoolIndex++;

	RenderFrameIcon(Seed, History, Honeycombs, PoolRT);
	return PoolRT;
}

void UFrameIconGeneratorComponent::GenerateFrameIconToTarget(
	int32 Seed,
	TArray<FCombDistributionSnapshot>& History,
	const TMap<ECombType, int32>& Honeycombs,
	UTextureRenderTarget2D* TargetRT)
{
	if (!CaptureComponent || !FrameMesh || !IntermediateRT || !PostProcessMaterial)
	{
		UE_LOG(LogTemp, Warning, TEXT("FrameIconGeneratorComponent: missing refs, call not possible"));
		return;
	}

	if (!TargetRT)
	{
		UE_LOG(LogTemp, Warning, TEXT("FrameIconGeneratorComponent: TargetRT is null"));
		return;
	}

	RenderFrameIcon(Seed, History, Honeycombs, TargetRT);
}

void UFrameIconGeneratorComponent::ResetIndex()
{
	NextPoolIndex = 0;
}
