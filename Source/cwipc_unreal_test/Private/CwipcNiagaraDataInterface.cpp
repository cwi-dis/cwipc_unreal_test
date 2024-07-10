// Fill out your copyright notice in the Description page of Project Settings.


#include "CwipcNiagaraDataInterface.h"

#include "cwipc_util/api.h"
#include "CoreGlobals.h"

#define LOCTEXT_NAMESPACE "HoudiniNiagaraDataInterface"

static const FName GetNumberOfPointsName("GetNumberOfPoints");

UCwipcNiagaraDataInterface::UCwipcNiagaraDataInterface(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if 0
	Proxy.Reset(new FNDIMousePositionProxy());
#endif
}

void UCwipcNiagaraDataInterface::PostInitProperties()
{
	UE_LOG(LogTemp, Warning, TEXT("xxxjack UCwipcNiagaraDataInterface::PostInitProperties: called"));
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
	UE_LOG(LogTemp, Warning, TEXT("xxxjack UCwipcNiagaraDataInterface::PostLoad: called"));
	Super::PostLoad();
	CwipcPointCloudSourceAsset = NewObject<UCwipcSource>();
	UE_LOG(LogTemp, Warning, TEXT("xxxjack UCwipcNiagaraDataInterface::PostLoad: created PointCloudObject"));
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
}


void UCwipcNiagaraDataInterface::GetVMExternalFunction(const FVMExternalFunctionBindingInfo& BindingInfo, void* InstanceData, FVMExternalFunction& OutFunc)
{

	static const FName NAME_Attribute("Attribute");

	const FVMFunctionSpecifier* AttributeSpecifier = BindingInfo.FindSpecifier(NAME_Attribute);
	bool bAttributeSpecifierRequiredButNotFound = false;

	if (BindingInfo.Name == GetNumberOfPointsName && BindingInfo.GetNumInputs() == 0 && BindingInfo.GetNumOutputs() == 1)
	{
		OutFunc = FVMExternalFunction::CreateUObject(this, &UCwipcNiagaraDataInterface::GetNumberOfPoints);
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
