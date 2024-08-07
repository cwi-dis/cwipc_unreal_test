// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CwipcSource.h"
#include "CwipcSourceCamera.generated.h"

/**
 * 
 */
UCLASS()
class CWIPC_UNREAL_TEST_API UCwipcSourceCamera : public UCwipcSource
{
	GENERATED_BODY()
	
public:
	virtual cwipc_source* _AllocateSource() override;

	UPROPERTY(EditAnywhere, Category = "Cwipc Niagara", meta = (DisplayName = "Path to cameraconfig.json"))
	FString ConfigFileName;
};
