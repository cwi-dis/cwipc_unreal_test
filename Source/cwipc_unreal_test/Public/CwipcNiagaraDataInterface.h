// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraDataInterface.h"
#include "CwipcSource.h"
#include "CwipcNiagaraDataInterface.generated.h"

class cwipc_source;
class cwipc;
struct cwipc_point;

/**
 * 
 */


UCLASS(EditInlineNew, Category = "Cwipc Niagara", meta = (DisplayName = "Cwipc Point Cloud Niagara Data Interface"))
class CWIPC_UNREAL_TEST_API UCwipcNiagaraDataInterface : public UNiagaraDataInterface
{
	GENERATED_UCLASS_BODY()
	BEGIN_SHADER_PARAMETER_STRUCT(FShaderParameters, )
	SHADER_PARAMETER(FVector4f, DummyTestMousePosition)
	END_SHADER_PARAMETER_STRUCT()


	FORCEINLINE int32 GetNumberOfPoints()const { return CwipcPointCloudSourceAsset ? CwipcPointCloudSourceAsset->GetNumberOfPoints() : 0; }


protected:
	virtual bool CopyToInternal(UNiagaraDataInterface* Destination) const override;

public:

	UPROPERTY(EditAnywhere, Category = "Cwipc Niagara", meta = (DisplayName = "Cwipc Point Cloud Source Asset"))
	TObjectPtr<UCwipcSource> CwipcPointCloudSourceAsset;
	//----------------------------------------------------------------------------
	// UObject Interface
	virtual void PostInitProperties() override;
	virtual void PostLoad() override;
	virtual bool CanExecuteOnTarget(ENiagaraSimTarget Target) const override { return true; }
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	//----------------------------------------------------------------------------
	// UNiagaraDataInterface Interface

	// Returns the DI's functions signatures 
	virtual void GetFunctions(TArray<FNiagaraFunctionSignature>& OutFunctions)override;

	// Returns the delegate for the passed function signature.
	virtual void GetVMExternalFunction(const FVMExternalFunctionBindingInfo& BindingInfo, void* InstanceData, FVMExternalFunction& OutFunc) override;

	virtual bool Equals(const UNiagaraDataInterface* Other) const override;

	//----------------------------------------------------------------------------
	// EXPOSED FUNCTIONS

	// Returns the float value at a given sample index in the point cloud
	void GetNumberOfPoints(FVectorVMExternalFunctionContext& Context);
	
	void GetParticleSize(FVectorVMExternalFunctionContext& Context);

	void GetColor(FVectorVMExternalFunctionContext& Context);

	void GetPosition(FVectorVMExternalFunctionContext& Context);
};
