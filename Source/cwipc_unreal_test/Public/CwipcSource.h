// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "cwipc_util/api.h"

#include "CwipcSource.generated.h"

class UCwipcSource;
class cwipc_source;
class cwipc;
struct cwipc_point;

USTRUCT(BlueprintType)
struct FCwipcPoint
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Index;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float X;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Y;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Z;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float R;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float G;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float B;
};

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

	UFUNCTION(BlueprintCallable, Category = "Cwipc Niagara", meta = (DisplayName = "Get point from current point cloud and store it in TArray"))
	TArray<FCwipcPoint> AllPoints();

	UPROPERTY(VisibleAnywhere, Category = "Cwipc Niagara", meta = (DisplayName = "Number of points in current point cloud"))
	int pc_count;

	UPROPERTY(VisibleAnywhere, Category = "Cwipc Niagara", meta = (DisplayName = "Timestamp of current point cloud"))
	int pc_timestamp;
	// Sample indexes for each point

	UPROPERTY()
	TArray< FPcIndexes > PointValueIndexes;

	cwipc_point * GetPoint(int32 index) const;
	
};
