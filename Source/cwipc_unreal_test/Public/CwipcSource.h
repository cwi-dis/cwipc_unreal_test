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
UCLASS(BlueprintType, Category = "Cwipc Niagara", meta = (DisplayName = "Cwipc Point Cloud Source"))
class CWIPC_UNREAL_TEST_API UCwipcSource : public UObject
{
	friend class UCwipcNiagaraDataInterface;

	GENERATED_UCLASS_BODY()

protected:
	cwipc_source* source;
	cwipc* pc;
	cwipc_point* pc_points;
	
	void _initialize();

public:

	virtual void PostInitProperties() override;
	virtual void PostLoad() override;

	UFUNCTION(BlueprintCallable, Category = "Cwipc Niagara", meta = (DisplayName = "Get number of points from current point cloud"))
	int32 GetNumberOfPoints();

	UPROPERTY(VisibleAnywhere, Category = "Cwipc Niagara", meta = (DisplayName = "Number of points in current point cloud"))
	int pc_count;

	UPROPERTY(VisibleAnywhere, Category = "Cwipc Niagara", meta = (DisplayName = "Timestamp of current point cloud"))
	int pc_timestamp;
	// Sample indexes for each point

	UPROPERTY()
	TArray< FPcIndexes > PointValueIndexes;
	
};
