// Fill out your copyright notice in the Description page of Project Settings.


#include "CwipcNiagaraDataInterface.h"

#include "cwipc_util/api.h"
#include "CoreGlobals.h"

#define LOCTEXT_NAMESPACE "HoudiniNiagaraDataInterface"

static const FName GetNumberOfPointsName("GetNumberOfPoints");
static const FName GetColorName("GetColor");
static const FName GetPositionName("GetPosition");

UCwipcNiagaraDataInterface::UCwipcNiagaraDataInterface(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if 0
	Proxy.Reset(new FNDIMousePositionProxy());
#endif
}

void UCwipcNiagaraDataInterface::PostInitProperties()
{
	UE_LOG(LogTemp, Warning, TEXT("xxxjack UCwipcNiagaraDataInterface::PostInitProperties: called, this=0x%x"), (uint64_t)this);
	UE_LOG(LogTemp, Warning, TEXT("xxxjack Cwipc,compiler version = 0x%x "), CWIPC_API_VERSION);
	FString runtimeVersion = cwipc_get_version();
	UE_LOG(LogTemp, Warning, TEXT("xxxjack Cwipc,runtime version = %s "), *runtimeVersion);
	Super::PostInitProperties();
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		ENiagaraTypeRegistryFlags RegistryFlags = ENiagaraTypeRegistryFlags::AllowAnyVariable
			| ENiagaraTypeRegistryFlags::AllowParameter;

		FNiagaraTypeRegistry::Register(FNiagaraTypeDefinition(GetClass()), RegistryFlags);
#if 0
		RegistryFlags |= ENiagaraTypeRegistryFlags::AllowPayload;
		FNiagaraTypeRegistry::Register(FHoudiniEvent::StaticStruct(), RegistryFlags);
#endif
	}

}

void UCwipcNiagaraDataInterface::PostLoad()
{
	Super::PostLoad();
	CwipcPointCloudSourceAsset = nullptr;
	MarkRenderDataDirty();
}

#if WITH_EDITOR

void UCwipcNiagaraDataInterface::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UCwipcNiagaraDataInterface, CwipcPointCloudSourceAsset))
	{
		Modify();
		if (CwipcPointCloudSourceAsset)
		{
			MarkRenderDataDirty();
		}
	}

}

#endif // WITH_EDITOR

// Returns the signature of all the functions avaialable in the data interface
void UCwipcNiagaraDataInterface::GetFunctions(TArray<FNiagaraFunctionSignature>& OutFunctions)
{

	{
		// GetNumberofPoints
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetNumberOfPointsName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCloud")));	// PointCache in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("Value")));    	// Number of points Out

		Sig.SetDescription(LOCTEXT("CwipcDataInterface_GetNumberOfPoints",
			"Returns the number of points in the point cloud"));

		OutFunctions.Add(Sig);
	}

	{
		// GetPosition
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetPositionName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));	// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("SampleIndex")));	// SampleIndex In
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetVec3Def(), TEXT("Position")));	// Vector3 Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetPosition",
			"Helper function returning the position value for a given sample index in the point cache file.\nThe returned Position vector is converted from Houdini's coordinate system to Unreal's."));

		OutFunctions.Add(Sig);
	}

	{
		// GetColor
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetColorName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));	// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("SampleIndex")));	// SampleIndex In
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetColorDef(), TEXT("Color")));	// Color Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetColor",
			"Helper function returning the color value for a given Sample Index in the point cache."));

		OutFunctions.Add(Sig);
	}
}

DEFINE_NDI_DIRECT_FUNC_BINDER(UCwipcNiagaraDataInterface, GetPosition);
DEFINE_NDI_DIRECT_FUNC_BINDER(UCwipcNiagaraDataInterface, GetColor);

void UCwipcNiagaraDataInterface::GetVMExternalFunction(const FVMExternalFunctionBindingInfo& BindingInfo, void* InstanceData, FVMExternalFunction& OutFunc)
{

	static const FName NAME_Attribute("Attribute");

	const FVMFunctionSpecifier* AttributeSpecifier = BindingInfo.FindSpecifier(NAME_Attribute);
	bool bAttributeSpecifierRequiredButNotFound = false;

	if (BindingInfo.Name == GetNumberOfPointsName && BindingInfo.GetNumInputs() == 0 && BindingInfo.GetNumOutputs() == 1)
	{
		OutFunc = FVMExternalFunction::CreateUObject(this, &UCwipcNiagaraDataInterface::GetNumberOfPoints);
	}
	else if (BindingInfo.Name == GetColorName && BindingInfo.GetNumInputs() == 1 && BindingInfo.GetNumOutputs() == 4)
	{
		NDI_FUNC_BINDER(UCwipcNiagaraDataInterface, GetColor)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetPositionName && BindingInfo.GetNumInputs() == 1 && BindingInfo.GetNumOutputs() == 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCwipcNiagaraDataInterface::GetVMExternalFunction: bound GetPosition"));
		NDI_FUNC_BINDER(UCwipcNiagaraDataInterface, GetPosition)::Bind(this, OutFunc);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UCwipcNiagaraDataInterface::GetVMExternalFunction: failed to bind %s"), *BindingInfo.Name.ToString());

		OutFunc = FVMExternalFunction();
	}
}

bool UCwipcNiagaraDataInterface::CopyToInternal(UNiagaraDataInterface* Destination) const
{
	if (!Super::CopyToInternal(Destination))
		return false;

	UCwipcNiagaraDataInterface* CastedInterface = CastChecked<UCwipcNiagaraDataInterface>(Destination);
	if (!CastedInterface)
		return false;

	CastedInterface->CwipcPointCloudSourceAsset = CwipcPointCloudSourceAsset;
	CastedInterface->MarkRenderDataDirty();

	return true;
}

bool UCwipcNiagaraDataInterface::Equals(const UNiagaraDataInterface* Other) const
{
	if (!Super::Equals(Other))
		return false;

	const UCwipcNiagaraDataInterface* OtherHN = CastChecked<UCwipcNiagaraDataInterface>(Other);

	if (OtherHN != nullptr && OtherHN->CwipcPointCloudSourceAsset != nullptr &&CwipcPointCloudSourceAsset)
	{
		return OtherHN->CwipcPointCloudSourceAsset == CwipcPointCloudSourceAsset;
	}

	return false;
}

void UCwipcNiagaraDataInterface::GetNumberOfPoints(FVectorVMExternalFunctionContext& Context)
{
	VectorVM::FExternalFuncRegisterHandler<int32> OutNumPoints(Context);
	*OutNumPoints.GetDest() = CwipcPointCloudSourceAsset ? CwipcPointCloudSourceAsset->GetNumberOfPoints() : 0;
	OutNumPoints.Advance();
}

void UCwipcNiagaraDataInterface::GetColor(FVectorVMExternalFunctionContext& Context)
{
	VectorVM::FExternalFuncInputHandler<int32> SampleIndexParam(Context);

	VectorVM::FExternalFuncRegisterHandler<float> OutSampleR(Context);
	VectorVM::FExternalFuncRegisterHandler<float> OutSampleG(Context);
	VectorVM::FExternalFuncRegisterHandler<float> OutSampleB(Context);
	VectorVM::FExternalFuncRegisterHandler<float> OutSampleA(Context);

	if (CwipcPointCloudSourceAsset == nullptr) {
		return;
	}
	int idx = SampleIndexParam.Get();
	cwipc_point* pt = CwipcPointCloudSourceAsset->GetPoint(idx);
	if (pt == nullptr) {
		return;
	}
	*OutSampleA.GetDest() = 1.0;
	float r = pt->r / 255.0;
	float g = pt->g / 255.0;
	float b = pt->b / 255.0;
	*OutSampleR.GetDest() = r;
	*OutSampleG.GetDest() = g;
	*OutSampleB.GetDest() = b;
	SampleIndexParam.Advance();
	OutSampleR.Advance();
	OutSampleG.Advance();
	OutSampleB.Advance();
	OutSampleA.Advance();
	UE_LOG(LogTemp, Warning, TEXT("GetColor(%d) -> (%f, %f, %f)"), idx, r, g, b);

}

void UCwipcNiagaraDataInterface::GetPosition(FVectorVMExternalFunctionContext& Context)
{
	VectorVM::FExternalFuncInputHandler<int32> SampleIndexParam(Context);

	VectorVM::FExternalFuncRegisterHandler<float> OutSampleX(Context);
	VectorVM::FExternalFuncRegisterHandler<float> OutSampleY(Context);
	VectorVM::FExternalFuncRegisterHandler<float> OutSampleZ(Context);

	if (CwipcPointCloudSourceAsset == nullptr) {
		return;
	}
	int idx = SampleIndexParam.Get();
	cwipc_point* pt = CwipcPointCloudSourceAsset->GetPoint(idx);
	if (pt == nullptr) {
		return;
	}
	*OutSampleX.GetDest() = pt->x;
	*OutSampleY.GetDest() = pt->y;
	*OutSampleZ.GetDest() = pt->z;
	SampleIndexParam.Advance();
	OutSampleX.Advance();
	OutSampleY.Advance();
	OutSampleZ.Advance();
	UE_LOG(LogTemp, Warning, TEXT("GetPosition(%d) -> (%f, %f, %f)"), idx, pt->x, pt->y, pt->z);
}

void UCwipcNiagaraDataInterface::GetPointIDsToSpawnAtTime(FVectorVMExternalFunctionContext& Context)
{
}
