// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "cwipc_util/api.h"

#include "CwipcSource.generated.h"

class UCwipcSource;
class cwipc_source;
class cwipc;
struct cwipc_point;

USTRUCT()
struct FPcIndexes
{
	GENERATED_BODY()

	// Simple structure for storing all the sample indexes used for a given point
	UPROPERTY()
	TArray<int32> SampleIndexes;
};
/**
 * 
 */
UCLASS(BlueprintType)
class CWIPC_UNREAL_TEST_API UCwipcSource : public UObject
{
	friend class UCwipcNiagaraDataInterface;

	GENERATED_UCLASS_BODY()

protected:
	cwipc_source* source;
	cwipc* pc;
	cwipc_point* pc_points;
	int pc_count;

public:

	virtual void PostInitProperties() override;
	virtual void PostLoad() override;

	UFUNCTION(BlueprintCallable, Category = "Cwipc Data")
	int32 GetNumberOfPoints();

	// The number of points found in the point cloud
	UPROPERTY(VisibleAnywhere, Category = "Cwipc Data")
	int32 NumberOfPoints;

	// Sample indexes for each point

	UPROPERTY()
	TArray< FPcIndexes > PointValueIndexes;
	
};
