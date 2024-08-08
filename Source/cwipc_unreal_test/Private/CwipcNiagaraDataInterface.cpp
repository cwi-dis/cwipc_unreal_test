// Fill out your copyright notice in the Description page of Project Settings.


#include "CwipcNiagaraDataInterface.h"

#include "cwipc_util/api.h"
#include "CoreGlobals.h"

// Define as empty to get debug prints
#define DBG
//#define DBGMORE
// Define is if(0) to not get debug prints
//#define DBG if(0)
#define DBGMORE if(0)

#define LOCTEXT_NAMESPACE "HoudiniNiagaraDataInterface"


static const FName InitializeSourceName("InitializeSource");
static const FName LockPointCloudName("LockPointCloud");
static const FName GetTimeStampName("GetTimeStamp");
static const FName GetNumberOfPointsName("GetNumberOfPoints");
static const FName GetParticleSizeName("GetParticleSize");
static const FName GetColorName("GetColor");
static const FName GetPositionName("GetPosition");

UCwipcNiagaraDataInterface::UCwipcNiagaraDataInterface(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CwipcPointCloudSourceAsset = nullptr;
	DBG UE_LOG(LogTemp, Display, TEXT("UCwipcNiagaraDataInterface[%s]::UCwipcNiagaraDataInterface() called, source=[%s]"), *GetPathNameSafe(this), *GetPathNameSafe(CwipcPointCloudSourceAsset));

#if 0
	Proxy.Reset(new FNDIMousePositionProxy());
#endif
}

void UCwipcNiagaraDataInterface::PostInitProperties()
{
	DBG UE_LOG(LogTemp, Display, TEXT("UCwipcNiagaraDataInterface[%s]::PostInitProperties() called, source=[%s]"), *GetPathNameSafe(this), *GetPathNameSafe(CwipcPointCloudSourceAsset));
	//UE_LOG(LogTemp, Warning, TEXT("xxxjack Cwipc,compiler version = 0x%x "), CWIPC_API_VERSION);
	FString runtimeVersion = cwipc_get_version();
	DBG UE_LOG(LogTemp, Display, TEXT("UCwipcNiagaraDataInterface::PostInitProperties: cwipc runtime version = %s "), *runtimeVersion);
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
	DBG UE_LOG(LogTemp, Display, TEXT("UCwipcNiagaraDataInterface[%s]::PostLoad() on called, source=[%s]"), *GetPathNameSafe(this), *GetPathNameSafe(CwipcPointCloudSourceAsset));
	Super::PostLoad();
	//CwipcPointCloudSourceAsset = nullptr;
	warnedAboutLockBeforeInitialize = false;
	MarkRenderDataDirty();
}

#if WITH_EDITOR

void UCwipcNiagaraDataInterface::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	DBG UE_LOG(LogTemp, Display, TEXT("UCwipcNiagaraDataInterface[%s]::PostEditChangeProperty() called, source=[%s]"), *GetPathNameSafe(this), *GetPathNameSafe(this));
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UCwipcNiagaraDataInterface, CwipcPointCloudSourceAsset))
	{
		Modify();
		if (CwipcPointCloudSourceAsset)
		{
			MarkRenderDataDirty();
		}
	}
	warnedAboutLockBeforeInitialize = false;

}

#endif // WITH_EDITOR

// Returns the signature of all the functions avaialable in the data interface
void UCwipcNiagaraDataInterface::GetFunctions(TArray<FNiagaraFunctionSignature>& OutFunctions)
{
	{
		// InitializeSource
		FNiagaraFunctionSignature Sig;
		Sig.Name = InitializeSourceName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCloud")));	// PointCloud in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetBoolDef(), TEXT("Didrun")));	// Success Out
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetBoolDef(), TEXT("Success")));	// Success Out

		Sig.SetDescription(LOCTEXT("CwipcDataInterface_InitializeSource",
			"Initializes and starts the point cloud source"));

		OutFunctions.Add(Sig);
	}
	{
		// LockPointCloud
		FNiagaraFunctionSignature Sig;
		Sig.Name = LockPointCloudName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCloud")));	// PointCloud in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetBoolDef(), TEXT("Value")));    	// True if a fresh point cloud was gathered

		Sig.SetDescription(LOCTEXT("CwipcDataInterface_LockPointCloud",
			"Gets a fresh point cloud if one is available"));

		OutFunctions.Add(Sig);
	} {
		// GetTimeStamp
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetTimeStampName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCloud")));	// PointCloud in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("Value")));    	// Relative time stamp Out

		Sig.SetDescription(LOCTEXT("CwipcDataInterface_GetTimeStamp",
			"Returns the relative timestamp of the current point cloud"));

		OutFunctions.Add(Sig);
	}
	{
		// GetNumberofPoints
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetNumberOfPointsName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCloud")));	// PointCloud in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("Value")));    	// Number of points Out

		Sig.SetDescription(LOCTEXT("CwipcDataInterface_GetNumberOfPoints",
			"Returns the number of points in the point cloud"));

		OutFunctions.Add(Sig);
	}
	{
		// GetParticleSize
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetParticleSizeName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCloud")));	// PointCloud in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Value")));    	// Number of points Out

		Sig.SetDescription(LOCTEXT("CwipcDataInterface_GetParticleSize",
			"Returns the particle size of points in the point cloud"));

		OutFunctions.Add(Sig);
	}
	{
		// GetPosition
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetPositionName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCloud")));	// PointCloud in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("SampleIndex")));	// SampleIndex In
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetVec3Def(), TEXT("Position")));	// Vector3 Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetPosition",
			"Function returns the position value for a given sample index in the point cloud."));

		OutFunctions.Add(Sig);
	}

	{
		// GetColor
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetColorName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCloud")));	// PointCloud in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("SampleIndex")));	// SampleIndex In
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetColorDef(), TEXT("Color")));	// Color Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetColor",
			"Function returns the color value for a given Sample Index in the point cloud."));

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

	if (BindingInfo.Name == LockPointCloudName && BindingInfo.GetNumInputs() == 0 && BindingInfo.GetNumOutputs() == 1)
	{
		OutFunc = FVMExternalFunction::CreateUObject(this, &UCwipcNiagaraDataInterface::LockPointCloud);
	}
	else if (BindingInfo.Name == GetTimeStampName && BindingInfo.GetNumInputs() == 0 && BindingInfo.GetNumOutputs() == 1)
	{
		OutFunc = FVMExternalFunction::CreateUObject(this, &UCwipcNiagaraDataInterface::GetTimeStamp);
	}
	else if (BindingInfo.Name == GetNumberOfPointsName && BindingInfo.GetNumInputs() == 0 && BindingInfo.GetNumOutputs() == 1)
	{
		OutFunc = FVMExternalFunction::CreateUObject(this, &UCwipcNiagaraDataInterface::GetNumberOfPoints);
	}
	else if (BindingInfo.Name == InitializeSourceName && BindingInfo.GetNumInputs() == 0 && BindingInfo.GetNumOutputs() == 2)
	{
		OutFunc = FVMExternalFunction::CreateUObject(this, &UCwipcNiagaraDataInterface::InitializeSource);
	}
	else if (BindingInfo.Name == GetParticleSizeName && BindingInfo.GetNumInputs() == 0 && BindingInfo.GetNumOutputs() == 1)
	{
		OutFunc = FVMExternalFunction::CreateUObject(this, &UCwipcNiagaraDataInterface::GetParticleSize);
	}
	else if (BindingInfo.Name == GetColorName && BindingInfo.GetNumInputs() == 1 && BindingInfo.GetNumOutputs() == 4)
	{
		NDI_FUNC_BINDER(UCwipcNiagaraDataInterface, GetColor)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetPositionName && BindingInfo.GetNumInputs() == 1 && BindingInfo.GetNumOutputs() == 3)
	{
		//UE_LOG(LogTemp, Warning, TEXT("UCwipcNiagaraDataInterface::GetVMExternalFunction: bound GetPosition"));
		NDI_FUNC_BINDER(UCwipcNiagaraDataInterface, GetPosition)::Bind(this, OutFunc);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UCwipcNiagaraDataInterface[%s]::GetVMExternalFunction: failed to bind name %s"), *GetPathNameSafe(this), *BindingInfo.Name.ToString());

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
	DBG UE_LOG(LogTemp, Display, TEXT("UCwipcNiagaraDataInterface[%s]::CopyToInternal() called, destination=[%s], source=[%s]"), *GetPathNameSafe(this), *GetPathNameSafe(CastedInterface), *GetPathNameSafe(CwipcPointCloudSourceAsset));

	CastedInterface->CwipcPointCloudSourceAsset = CwipcPointCloudSourceAsset;
	CastedInterface->MarkRenderDataDirty();
	CastedInterface->warnedAboutLockBeforeInitialize = false;

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

void UCwipcNiagaraDataInterface::InitializeSource(FVectorVMExternalFunctionContext& Context)
{
	VectorVM::FExternalFuncRegisterHandler<bool> OutDidRun(Context);
	VectorVM::FExternalFuncRegisterHandler<bool> OutSuccess(Context);
	bool success = false;
	if (CwipcPointCloudSourceAsset) {
		success = CwipcPointCloudSourceAsset->InitializeSource();
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("UCwipcNiagaraDataInterface[%s]::InitializeSource: source == NULL"), *GetPathNameSafe(this));
	}
	*OutDidRun.GetDest() = true;
	OutDidRun.Advance();
	*OutSuccess.GetDest() = success;
	OutSuccess.Advance();
	DBG UE_LOG(LogTemp, Display, TEXT("UCwipcNiagaraDataInterface[0x%p %s]::InitializeSource() returns %d"), (void*)this, *GetPathNameSafe(this), (int)success);

}

void UCwipcNiagaraDataInterface::LockPointCloud(FVectorVMExternalFunctionContext& Context)
{
	DBGMORE UE_LOG(LogTemp, Display, TEXT("UCwipcNiagaraDataInterface[%s]::LockPointCloud() called"), *GetPathNameSafe(this));
	VectorVM::FExternalFuncRegisterHandler<bool> IsFresh(Context);
	if (CwipcPointCloudSourceAsset == nullptr) 
	{
		if (!warnedAboutLockBeforeInitialize) 
		{
			UE_LOG(LogTemp, Warning, TEXT("UCwipcNiagaraDataInterface[0x%p %s]::LockPointCloud(): no source, InitializeSource() not called before?"), (void *)this, *GetPathNameSafe(this));
			warnedAboutLockBeforeInitialize = true;
		}
	}
	else 
	{
		// If we did warn before we should warn now that the condition is over
		if (warnedAboutLockBeforeInitialize)
		{
			UE_LOG(LogTemp, Warning, TEXT("UCwipcNiagaraDataInterface[%s]::LockPointCloud(): InitializeSource apparently called, source=[%]"), *GetPathNameSafe(this), *GetPathNameSafe(CwipcPointCloudSourceAsset));
			warnedAboutLockBeforeInitialize = false;
		}
	}
	bool isFresh = CwipcPointCloudSourceAsset ? CwipcPointCloudSourceAsset->LockPointCloud() : false;
	*IsFresh.GetDest() = isFresh;
	IsFresh.Advance();
	DBGMORE UE_LOG(LogTemp, Display, TEXT("UCwipcNiagaraDataInterface[%s]::LockPointCloud() returns %d"), *GetPathNameSafe(this), (int)isFresh);
\
}

void UCwipcNiagaraDataInterface::GetTimeStamp(FVectorVMExternalFunctionContext& Context)
{
	DBGMORE UE_LOG(LogTemp, Display, TEXT("UCwipcNiagaraDataInterface[%s]::GetTimeStamp() called"), *GetPathNameSafe(this));
	VectorVM::FExternalFuncRegisterHandler<int32> OutTimeStamp(Context);
	int32 timestamp = CwipcPointCloudSourceAsset ? CwipcPointCloudSourceAsset->GetTimeStamp() : 0;
	*OutTimeStamp.GetDest() = timestamp;
	OutTimeStamp.Advance();
	DBGMORE UE_LOG(LogTemp, Display, TEXT("UCwipcNiagaraDataInterface[%s]::GetTimeStamp() returns %d"), *GetPathNameSafe(this), timestamp);
}

void UCwipcNiagaraDataInterface::GetNumberOfPoints(FVectorVMExternalFunctionContext& Context)
{
	DBGMORE UE_LOG(LogTemp, Display, TEXT("UCwipcNiagaraDataInterface[%s]::GetNumberOfPoints() called"), *GetPathNameSafe(this));
	VectorVM::FExternalFuncRegisterHandler<int32> OutNumPoints(Context);
	int32 nPoints = CwipcPointCloudSourceAsset ? CwipcPointCloudSourceAsset->GetNumberOfPoints() : 0;
	*OutNumPoints.GetDest() = nPoints;
	OutNumPoints.Advance();
	DBGMORE UE_LOG(LogTemp, Display, TEXT("UCwipcNiagaraDataInterface[%s]::GetNumberOfPoints() returns %d"), *GetPathNameSafe(this), nPoints);
}

void UCwipcNiagaraDataInterface::GetParticleSize(FVectorVMExternalFunctionContext& Context)
{
	DBGMORE UE_LOG(LogTemp, Display, TEXT("UCwipcNiagaraDataInterface[%s]::GetParticleSize() called"), *GetPathNameSafe(this));
	VectorVM::FExternalFuncRegisterHandler<float> OutParticleSize(Context);
	float particleSize = CwipcPointCloudSourceAsset ? CwipcPointCloudSourceAsset->GetParticleSize() : 0;
	*OutParticleSize.GetDest() = particleSize;
	OutParticleSize.Advance();
	DBGMORE UE_LOG(LogTemp, Display, TEXT("UCwipcNiagaraDataInterface[%s]::GetParticleSize() returns %f"), *GetPathNameSafe(this), particleSize);
}

void UCwipcNiagaraDataInterface::GetColor(FVectorVMExternalFunctionContext& Context)
{
	DBGMORE UE_LOG(LogTemp, Display, TEXT("UCwipcNiagaraDataInterface[%s]::GetColor() called"), *GetPathNameSafe(this));
	VectorVM::FExternalFuncInputHandler<int32> SampleIndexParam(Context);

	VectorVM::FExternalFuncRegisterHandler<float> OutSampleR(Context);
	VectorVM::FExternalFuncRegisterHandler<float> OutSampleG(Context);
	VectorVM::FExternalFuncRegisterHandler<float> OutSampleB(Context);
	VectorVM::FExternalFuncRegisterHandler<float> OutSampleA(Context);

	if (CwipcPointCloudSourceAsset == nullptr) {
		return;
	}
	int32 nPoints = CwipcPointCloudSourceAsset ? CwipcPointCloudSourceAsset->GetNumberOfPoints() : 1;

	int32 numParticles = Context.GetNumInstances();
	for (int32 i = 0; i < numParticles; ++i)
	{
		int32 idx = SampleIndexParam.Get();

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
		*OutSampleA.GetDest() = 0.5;
		SampleIndexParam.Advance();
		OutSampleR.Advance();
		OutSampleG.Advance();
		OutSampleB.Advance();
		OutSampleA.Advance();
		//UE_LOG(LogTemp, Warning, TEXT("GetColor(%d) -> (%f, %f, %f)"), idx, r, g, b);
	}
	DBGMORE UE_LOG(LogTemp, Display, TEXT("UCwipcNiagaraDataInterface[%s]::GetColor() nParticles=%d, nPoints=%d"), *GetPathNameSafe(this), numParticles, nPoints);

}

void UCwipcNiagaraDataInterface::GetPosition(FVectorVMExternalFunctionContext& Context)
{
	DBGMORE UE_LOG(LogTemp, Display, TEXT("UCwipcNiagaraDataInterface[%s]::GetPosition() called"), *GetPathNameSafe(this));
	VectorVM::FExternalFuncInputHandler<int32> SampleIndexParam(Context);

	VectorVM::FExternalFuncRegisterHandler<float> OutSampleX(Context);
	VectorVM::FExternalFuncRegisterHandler<float> OutSampleY(Context);
	VectorVM::FExternalFuncRegisterHandler<float> OutSampleZ(Context);

	if (CwipcPointCloudSourceAsset == nullptr) {
		return;
	}
	int32 nPoints = CwipcPointCloudSourceAsset ? CwipcPointCloudSourceAsset->GetNumberOfPoints() : 1;

	int32 numParticles = Context.GetNumInstances();
	for (int32 i = 0; i < numParticles; ++i)
	{
		int32 idx = SampleIndexParam.Get();
		cwipc_point* pt = CwipcPointCloudSourceAsset->GetPoint(idx);
		if (pt == nullptr) {
			return;
		}
		*OutSampleX.GetDest() = pt->x * 100.0;
		*OutSampleY.GetDest() = pt->z * -100.0;
		*OutSampleZ.GetDest() = pt->y * 100.0;
		SampleIndexParam.Advance();
		OutSampleX.Advance();
		OutSampleY.Advance();
		OutSampleZ.Advance();
		//UE_LOG(LogTemp, Warning, TEXT("GetPosition(%d) -> (%f, %f, %f)"), idx, pt->x, pt->y, pt->z);
	}
	DBGMORE UE_LOG(LogTemp, Display, TEXT("UCwipcNiagaraDataInterface[%s]::GetPosition() nParticles=%d, nPoints=%d"), *GetPathNameSafe(this), numParticles, nPoints);
}
