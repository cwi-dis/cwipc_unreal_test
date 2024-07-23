// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "cwipc_util/api.h"

#include "CwipcSource.generated.h"

class UCwipcSource;
class cwipc_source;
class cwipc;
struct cwipc_point;
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
	int pc_points_count;

	/// <summary>
	/// Initialize the source, if it has not been initialized already.
	/// </summary>
	/// <returns>Returns true if the source is available for use (no matter whether it was created now or pre-existing)</returns>
	bool _OptionalInitializeSource();
	/// <summary>
	/// Get a new point cloud into pc, if one is available.
	/// </summary>
	/// <returns>Returns true if a point cloud is available (no matter whether it is new or old)</returns>
	bool _CheckForNewPointCloudAvailable();

	/// <summary>
	/// De-allocates everything in the right order.
	/// </summary>
	void _CleanupEverything();

public:

	virtual void PostInitProperties() override;
	virtual void PostLoad() override;

	UFUNCTION(BlueprintCallable, Category = "Cwipc Niagara", meta = (DisplayName = "Get number of points from current point cloud"))
	int32 GetNumberOfPoints();

	UFUNCTION(BlueprintCallable, Category = "Cwipc Niagara", meta = (DisplayName = "Get particle size for current point cloud"))
	float GetParticleSize();

	UPROPERTY(EditAnywhere, Category = "Cwipc Niagara", meta = (DisplayName = "Factor to convert point size to particle size"))
	float particle_size_factor = 1.0;

	UPROPERTY(EditAnywhere, Category = "Cwipc Niagara", meta = (DisplayName = "Synthetic generator: wanted point count"))
	int synthetic_wanted_pointcount = 400;

	UPROPERTY(EditAnywhere, Category = "Cwipc Niagara", meta = (DisplayName = "Synthetic generator: wanted clouds per second"))
	int synthetic_wanted_fps = 5;
	
	// Sample indexes for each point

	cwipc_point * GetPoint(int32 index) const;
	
};
