// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraDataInterface.h"
#include "CwipcNiagaraDataInterface.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, Category = "Cwipc Niagara", meta = (DisplayName = "Cwipc Point Cloud Info"))
class CWIPC_UNREAL_TEST_API UCwipcNiagaraDataInterface : public UNiagaraDataInterface
{
	GENERATED_UCLASS_BODY()
	BEGIN_SHADER_PARAMETER_STRUCT(FShaderParameters, )
	SHADER_PARAMETER(FVector4f, DummyTestMousePosition)
	END_SHADER_PARAMETER_STRUCT()

public:
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

};