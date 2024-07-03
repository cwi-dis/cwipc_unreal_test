// Fill out your copyright notice in the Description page of Project Settings.


#include "CwipcNiagaraDataInterface.h"

#include "CoreGlobals.h"


void UCwipcNiagaraDataInterface::PostInitProperties()
{
	UE_LOG(LogTemp, Warning, TEXT("xxxjack UCwipcNiagaraDataInterface::PostInitProperties: called"));
	Super::PostInitProperties();
#if 0
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		ENiagaraTypeRegistryFlags RegistryFlags = ENiagaraTypeRegistryFlags::AllowUserVariable
			| ENiagaraTypeRegistryFlags::AllowSystemVariable
			| ENiagaraTypeRegistryFlags::AllowEmitterVariable
			| ENiagaraTypeRegistryFlags::AllowParameter;

		FNiagaraTypeRegistry::Register(FNiagaraTypeDefinition(GetClass()), RegistryFlags);

		RegistryFlags |= ENiagaraTypeRegistryFlags::AllowPayload;
		FNiagaraTypeRegistry::Register(FHoudiniEvent::StaticStruct(), RegistryFlags);
	}
#endif
}

void UCwipcNiagaraDataInterface::PostLoad()
{
	UE_LOG(LogTemp, Warning, TEXT("xxxjack UCwipcNiagaraDataInterface::PostLoad: called"));
	Super::PostLoad();
#if 0
	MarkRenderDataDirty();
#endif
}

#if WITH_EDITOR

void UCwipcNiagaraDataInterface::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
#if 0
	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UCwipcNiagaraDataInterface, HoudiniPointCacheAsset))
	{
		Modify();
		if (HoudiniPointCacheAsset)
		{
			MarkRenderDataDirty();
		}
	}
#endif
}

#endif // WITH_EDITOR


// Returns the signature of all the functions avaialable in the data interface
void UCwipcNiagaraDataInterface::GetFunctions(TArray<FNiagaraFunctionSignature>& OutFunctions)
{
#if 0
	{
		// GetFloatValue
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetFloatValueName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));		// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("SampleIndex")));		// SampleIndex In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("AttributeIndex")));	// AttributeIndex In
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Value")));    	// Float Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetFloatValue",
			"Returns the float value in the point cache for a given Sample Index and Attribute Index.\n"));

		OutFunctions.Add(Sig);
	}

	{
		// GetFloatValueByString
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetFloatValueByStringName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));	// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("SampleIndex")));	// SampleIndex In
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Value")));	// Float Out

		Sig.FunctionSpecifiers.Add(FName("Attribute"));

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetFloatValueByString",
			"Returns the float value in the point cache for a given Sample Index and Attribute name.\n"));

		OutFunctions.Add(Sig);
	}

	{
		// GetVectorValue
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetVectorValueName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));		// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("SampleIndex")));		// SampleIndex In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("AttributeIndex")));	// AttributeIndex In
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetVec3Def(), TEXT("Value")));			// Vector3 Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetVectorValue",
			"Returns a Vector3 in the point cache for a given Sample Index and Attribute Index.\nThe returned Vector is converted from Houdini's coordinate system to Unreal's."));

		OutFunctions.Add(Sig);
	}

	{
		// GetVectorValueByString
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetVectorValueByStringName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));	// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("SampleIndex")));	// SampleIndex In
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetVec3Def(), TEXT("Value")));		// Vector3 Out

		Sig.FunctionSpecifiers.Add(FName("Attribute"));

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetVectorValueByString",
			"Returns a Vector3 in the point cache for a given Sample Index and Attribute name.\nThe returned Vector is converted from Houdini's coordinate system to Unreal's."));

		OutFunctions.Add(Sig);
	}

	{
		// GetVectorValueEx
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetVectorValueExName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));	// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("SampleIndex")));	// SampleIndex In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("AttributeIndex"))); // AttributeIndex In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetBoolDef(), TEXT("DoSwap")));		// DoSwap in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetBoolDef(), TEXT("DoScale")));	// DoScale in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetVec3Def(), TEXT("Value")));		// Vector3 Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetVectorValueEx",
			"Returns a Vector3 in the point cache for a given Sample Index and Attribute Index.\nThe DoSwap parameter indicates if the vector should be converted from Houdini*s coordinate system to Unreal's.\nThe DoScale parameter decides if the Vector value should be converted from meters (Houdini) to centimeters (Unreal)."));

		OutFunctions.Add(Sig);
	}

	{
		// GetVectorValueExByString
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetVectorValueExByStringName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));	// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("SampleIndex")));	// SampleIndex In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetBoolDef(), TEXT("DoSwap")));		// DoSwap in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetBoolDef(), TEXT("DoScale")));	// DoScale in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetVec3Def(), TEXT("Value")));		// Vector3 Out

		Sig.FunctionSpecifiers.Add(FName("Attribute"));

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetVectorValueExByString",
			"Returns a Vector3 in the point cache for a given Sample Index and Attribute name.\nThe DoSwap parameter indicates if the vector should be converted from Houdini*s coordinate system to Unreal's.\nThe DoScale parameter decides if the Vector value should be converted from meters (Houdini) to centimeters (Unreal)."));

		OutFunctions.Add(Sig);
	}

	{
		// GetVector4Value
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetVector4ValueName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));		// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("SampleIndex")));		// SampleIndex In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("AttributeIndex")));	// AttributeIndex In
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetVec4Def(), TEXT("Value")));			// Vector4 Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetVectorValue",
			"Returns a Vector4 in the point cache for a given Sample Index and Attribute Index.\nThe returned Vector is converted from Houdini's coordinate system to Unreal's."));

		OutFunctions.Add(Sig);
	}

	{
		// GetVector4ValueByString
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetVector4ValueByStringName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));	// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("SampleIndex")));	// SampleIndex In
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetVec4Def(), TEXT("Value")));		// Vector4 Out

		Sig.FunctionSpecifiers.Add(FName("Attribute"));

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetVectorValueByString",
			"Returns a Vector4 in the point cache for a given Sample Index and Attribute name.\nThe returned Vector is converted from Houdini's coordinate system to Unreal's."));

		OutFunctions.Add(Sig);
	}

	{
		// GetQuatValueEx
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetQuatValueName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));	// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("SampleIndex")));	// SampleIndex In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("AttributeIndex"))); // AttributeIndex In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetBoolDef(), TEXT("DoHoudiniToUnrealConversion")));		// DoHoudiniToUnrealConversion in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetQuatDef(), TEXT("Value")));		// Quat Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetQuatValue",
			"Returns a Quat in the point cache for a given Sample Index and Attribute Index.\nThe DoHoudiniToUnrealConversion parameter indicates if the vector should be converted from Houdini*s coordinate system to Unreal's."));

		OutFunctions.Add(Sig);
	}

	{
		// GetQuatValueByString
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetQuatValueByStringName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));	// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("SampleIndex")));	// SampleIndex In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetBoolDef(), TEXT("DoHoudiniToUnrealConversion")));		// DoHoudiniToUnrealConversion in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetQuatDef(), TEXT("Value")));		// Quat Out

		Sig.FunctionSpecifiers.Add(FName("Attribute"));

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetQuatValueByString",
			"Returns a Quat in the point cache for a given Sample Index and Attribute name.\nThe DoHoudiniToUnrealConversion parameter indicates if the quat should be converted from Houdini*s coordinate system to Unreal's."));

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
		// GetPositionAndTime
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetPositionAndTimeName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));	// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("SampleIndex")));	// SampleIndex In
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetVec3Def(), TEXT("Position")));	// Vector3 Out
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Time")));		// float Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetPositionAndTime",
			"Helper function returning the position and time values for a given Sample Index in the point cache.\nThe returned Position vector is converted from Houdini's coordinate system to Unreal's."));

		OutFunctions.Add(Sig);
	}

	{
		// GetNormal
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetNormalName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));	// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("SampleIndex")));	// SampleIndex In
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetVec3Def(), TEXT("Normal")));	// Vector3 Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetNormal",
			"Helper function returning the normal value for a given Sample Index in the point cache.\nThe returned Normal vector is converted from Houdini's coordinate system to Unreal's."));

		OutFunctions.Add(Sig);
	}

	{
		// GetTime
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetTimeName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));	// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("SampleIndex")));	// SampleIndex In
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Time")));		// Float Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetTime",
			"Helper function returning the time value for a given Sample Index in the point cache.\n"));

		OutFunctions.Add(Sig);
	}

	{
		// GetVelocity
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetVelocityName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));	// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("SampleIndex")));	// SampleIndex In
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetVec3Def(), TEXT("Velocity")));	// Vector3 Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetVelocity",
			"Helper function returning the velocity value for a given Sample Index in the point cache.\nThe returned velocity vector is converted from Houdini's coordinate system to Unreal's."));

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

	{
		// GetImpulse
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetImpulseName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));	// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("SampleIndex")));	// SampleIndex In
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Impulse")));	// Float Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetImpulse",
			"Helper function returning the impulse value for a given Sample Index in the point cache.\n"));

		OutFunctions.Add(Sig);
	}

	{
		// GetNumberOfPoints
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetNumberOfPointsName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));			// PointCache in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("NumberOfPoints")));  // Int Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetNumberOfPoints",
			"Returns the number of points (with different id values) in the point cache.\n"));

		OutFunctions.Add(Sig);
	}

	{
		// GetNumberOfSamples
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetNumberOfSamplesName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));			// PointCache in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("NumberOfSamples")));	// Int Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetNumberOfSamples",
			"Returns the number of samples in the point cache."));

		OutFunctions.Add(Sig);
	}

	{
		// GetNumberOfAttributes
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetNumberOfAttributesName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));				// PointCache in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("NumberOfAttributes")));		// Int Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetNumberOfAttributes",
			"Returns the number of attributes in the point cache."));

		OutFunctions.Add(Sig);
	}

	{
		// GetLastSampleIndexAtTime
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetLastSampleIndexAtTimeName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));			// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Time")));				// Time in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("LastSampleIndex")));	    // Int Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetLastSampleIndexAtTime",
			"Returns the index of the last sample in the point cache that has a time value lesser or equal to the Time parameter."));

		OutFunctions.Add(Sig);
	}

	{
		//GetPointIDsToSpawnAtTime
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetPointIDsToSpawnAtTimeName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));		// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Time")));		    // Time in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("LastSpawnTime"))); // Emitter state In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("LastSpawnTimeRequest")));	// Emitter state In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("LastSpawnedPointID")));	// Emitter state In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetBoolDef(), TEXT("ResetSpawnState")));	// Emitter state In

		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("MinID")));			// Int Out
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("MaxID")));			// Int Out
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("Count")));		    // Int Out
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("LastSpawnTime"))); // Emitter state Out
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("LastSpawnTimeRequest")));	// Emitter state Out
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("LastSpawnedPointID")));	// Emitter state Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetPointIDsToSpawnAtTime",
			"Returns the count and point IDs of the points that should spawn for a given time value."));

		OutFunctions.Add(Sig);
	}

	{
		// GetSampleIndexesForPointAtTime
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetSampleIndexesForPointAtTimeName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));				// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("PointID")));					// Point Number In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Time")));					// Time in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("PreviousSampleIndex")));	// Int Out
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("NextSampleIndex")));		// Int Out
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("PrevWeight")));			// Float Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetSampleIndexesForPointAtTime",
			"Returns the sample indexes for a given point at a given time.\nThe previous index, next index and weight can then be used to Lerp between values."));

		OutFunctions.Add(Sig);
	}

	{
		// GetPointPositionAtTime
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetPointPositionAtTimeName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));		// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("PointID")));			// Point Number In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Time")));		    // Time in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetVec3Def(), TEXT("Position")));		// Vector3 Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetPointPositionAtTime",
			"Helper function returning the linearly interpolated position for a given point at a given time.\nThe returned Position vector is converted from Houdini's coordinate system to Unreal's."));

		OutFunctions.Add(Sig);
	}

	{
		// GetPointValueAtTime
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetPointValueAtTimeName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));		// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("PointID")));			// Point Number In		
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("AttributeIndex")));	 // AttributeIndex In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Time")));		    // Time in		
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Value")));		// Float Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetPointValueAtTime",
			"Returns the linearly interpolated value in the specified attribute for a given point at a given time."));

		OutFunctions.Add(Sig);
	}

	{
		// GetPointValueAtTimeByString
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetPointValueAtTimeByStringName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));		// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("PointID")));			// Point Number In		
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Time")));		    // Time in		
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Value")));		// Float Out

		Sig.FunctionSpecifiers.Add(FName("Attribute"));

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetPointValueAtTimeByString",
			"Returns the linearly interpolated value in the specified attribute (by name) for a given point at a given time."));

		OutFunctions.Add(Sig);
	}

	{
		// GetPointVectorValueAtTime
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetPointVectorValueAtTimeName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));		// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("PointID")));			// Point ID In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("AttributeIndex")));	// AttributeIndex In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Time")));		    // Time in		
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetVec3Def(), TEXT("Value")));			// Vector3 Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetPointVectorValueAtTime",
			"Helper function returning the linearly interpolated Vector value in the specified Attribute Index for a given point at a given time.\nThe returned Vector is converted from Houdini's coordinate system to Unreal's."));

		OutFunctions.Add(Sig);
	}

	{
		// GetPointVectorValueAtTimeByString
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetPointVectorValueAtTimeByStringName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));		// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("PointID")));			// Point ID In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Time")));		    // Time in		
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetVec3Def(), TEXT("Value")));			// Vector3 Out

		Sig.FunctionSpecifiers.Add(FName("Attribute"));

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetPointVectorValueAtTimeByString",
			"Helper function returning the linearly interpolated Vector value in the specified attribute (by name) for a given point at a given time.\nThe returned Vector is converted from Houdini's coordinate system to Unreal's."));

		OutFunctions.Add(Sig);
	}

	{
		// GetPointVector4ValueAtTime
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetPointVector4ValueAtTimeName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));		// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("PointID")));			// Point ID In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("AttributeIndex")));	// AttributeIndex In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Time")));		    // Time in		
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetVec4Def(), TEXT("Value")));			// Vector4 Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetPointVector4ValueAtTime",
			"Helper function returning the linearly interpolated Vector4 value in the specified Attribute Index for a given point at a given time.\nThe returned Vector is converted from Houdini's coordinate system to Unreal's."));

		OutFunctions.Add(Sig);
	}

	{
		// GetPointVectorValueAtTimeByString
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetPointVector4ValueAtTimeByStringName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));		// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("PointID")));			// Point ID In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Time")));		    // Time in		
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetVec4Def(), TEXT("Value")));			// Vector4 Out

		Sig.FunctionSpecifiers.Add(FName("Attribute"));

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetPointVector4ValueAtTimeByString",
			"Helper function returning the linearly interpolated Vector4 value in the specified attribute (by name) for a given point at a given time.\nThe returned Vector is converted from Houdini's coordinate system to Unreal's."));

		OutFunctions.Add(Sig);
	}

	{
		// GetPointVectorValueAtTimeEx
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetPointVectorValueAtTimeExName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));		// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("PointID")));			// Point ID In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("AttributeIndex")));	 // AttributeIndex In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Time")));		    // Time in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetBoolDef(), TEXT("DoSwap")));			// DoSwap in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetBoolDef(), TEXT("DoScale")));		// DoScale in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetVec3Def(), TEXT("Value")));			// Vector3 Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetPointVectorValueAtTime",
			"Helper function returning the linearly interpolated Vector value in the specified attribute index for a given point at a given time.\nThe DoSwap parameter indicates if the vector should be converted from Houdini*s coordinate system to Unreal's.\nThe DoScale parameter decides if the Vector value should be converted from meters (Houdini) to centimeters (Unreal)."));

		OutFunctions.Add(Sig);
	}

	{
		// GetPointVectorValueAtTimeExByString
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetPointVectorValueAtTimeExByStringName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));		// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("PointID")));			// Point ID In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Time")));		    // Time in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetBoolDef(), TEXT("DoSwap")));			// DoSwap in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetBoolDef(), TEXT("DoScale")));		// DoScale in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetVec3Def(), TEXT("Value")));			// Vector3 Out

		Sig.FunctionSpecifiers.Add(FName("Attribute"));

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetPointVectorValueAtTimeByString",
			"Helper function returning the linearly interpolated Vector value in the specified attribute (by name) for a given point at a given time.\nThe DoSwap parameter indicates if the vector should be converted from Houdini*s coordinate system to Unreal's.\nThe DoScale parameter decides if the Vector value should be converted from meters (Houdini) to centimeters (Unreal)."));

		OutFunctions.Add(Sig);
	}

	{
		// GetPointQuatValueAtTime
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetPointQuatValueAtTimeName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));		// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("PointID")));			// Point ID In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("AttributeIndex")));	 // AttributeIndex In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Time")));		    // Time in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetBoolDef(), TEXT("DoHoudiniToUnrealConversion")));		// DoHoudiniToUnrealConversion in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetQuatDef(), TEXT("Value")));			// Quat Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetPointQuatValueAtTime",
			"Helper function returning the linearly interpolated Quat value in the specified attribute index for a given point at a given time.\nThe DoHoudiniToUnrealConversion parameter indicates if the vector4 should be converted from Houdini*s coordinate system to Unreal's.\n"));

		OutFunctions.Add(Sig);
	}

	{
		// GetPointQuatValueAtTimeByString
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetPointQuatValueAtTimeByStringName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));		// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("PointID")));			// Point ID In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Time")));		    // Time in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetBoolDef(), TEXT("DoHoudiniToUnrealConversion")));		// DoHoudiniToUnrealConversion in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetQuatDef(), TEXT("Value")));			// Quat Out

		Sig.FunctionSpecifiers.Add(FName("Attribute"));

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetPointQuatValueAtTimeByString",
			"Helper function returning the linearly interpolated Quat value in the specified attribute (by name) for a given point at a given time.\nThe DoHoudiniToUnrealConversion parameter indicates if the vector should be converted from Houdini*s coordinate system to Unreal's.\n"));

		OutFunctions.Add(Sig);
	}

	{
		// GetPointLife
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetPointLifeName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));		// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("PointID")));			// Point Number In		
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Life")));			// Float Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetPointLife",
			"Helper function returning the life value for a given point when spawned.\nThe life value is either calculated from the alive attribute or is the life attribute at spawn time."));

		OutFunctions.Add(Sig);
	}

	{
		// GetPointLifeAtTime
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetPointLifeAtTimeName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));		// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("PointID")));			// Point Number In		
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Time")));		    // Time in		
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Life")));			// Float Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetPointLifeAtTime",
			"Helper function returning the remaining life for a given point in the point cache at a given time."));

		OutFunctions.Add(Sig);
	}

	{
		// GetPointType
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetPointTypeName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));		// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("PointID")));			// Point Number In		
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("Type")));			// Int Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetPointType",
			"Helper function returning the type value for a given point when spawned.\n"));

		OutFunctions.Add(Sig);
	}

	{
		// GetPointNormalAtTime
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetPointNormalAtTimeName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));		// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("PointID")));			// Point Number In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Time")));		    // Time in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetVec3Def(), TEXT("Normal")));		// Vector3 Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetPointNormalAtTime",
			"Helper function returning the linearly interpolated normal for a given point at a given time.\nThe returned vector is converted from Houdini's coordinate system to Unreal's."));

		OutFunctions.Add(Sig);
	}

	{
		// GetPointColorAtTime
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetPointColorAtTimeName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));		// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("PointID")));			// Point Number In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Time")));		    // Time in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetVec3Def(), TEXT("Color")));		    // Vector3 Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetPointColorAtTime",
			"Helper function returning the linearly interpolated color for a given point at a given time."));

		OutFunctions.Add(Sig);
	}

	{
		// GetPointAlphaAtTime
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetPointAlphaAtTimeName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));		// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("PointID")));			// Point Number In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Time")));		    // Time in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Alpha")));		// Float Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetPointAlphaAtTime",
			"Helper function returning the linearly interpolated alpha for a given point at a given time."));

		OutFunctions.Add(Sig);
	}

	{
		// GetPointVelocityAtTime
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetPointVelocityAtTimeName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));		// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("PointID")));			// Point Number In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Time")));		    // Time in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetVec3Def(), TEXT("Velocity")));	    // Vector3 Out

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetPointVelocityAtTime",
			"Helper function returning the linearly interpolated velocity for a given point at a given time.\nThe returned vector is converted from Houdini's coordinate system to Unreal's."));

		OutFunctions.Add(Sig);
	}

	{
		// GetPointImpulseAtTime
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetPointImpulseAtTimeName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));		// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("PointID")));			// Point Number In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Time")));		    // Time in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Impulse")));		// Impulse

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetPointImpulseAtTime",
			"Helper function returning the linearly interpolated impulse for a given point at a given time."));

		OutFunctions.Add(Sig);
	}

	{
		// GetPointTypeAtTime
		FNiagaraFunctionSignature Sig;
		Sig.Name = GetPointTypeAtTimeName;
		Sig.bMemberFunction = true;
		Sig.bRequiresContext = false;
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition(GetClass()), TEXT("PointCache")));		// PointCache in
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("PointID")));			// Point Number In
		Sig.Inputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetFloatDef(), TEXT("Time")));		    // Time in
		Sig.Outputs.Add(FNiagaraVariable(FNiagaraTypeDefinition::GetIntDef(), TEXT("Type")));		    // Type

		Sig.SetDescription(LOCTEXT("DataInterfaceHoudini_GetPointTypeAtTime",
			"Helper function returning the integer type for a given point at a given time."));

		OutFunctions.Add(Sig);
	}
#endif // 0
}


void UCwipcNiagaraDataInterface::GetVMExternalFunction(const FVMExternalFunctionBindingInfo& BindingInfo, void* InstanceData, FVMExternalFunction& OutFunc)
{
#if 0
	static const FName NAME_Attribute("Attribute");

	const FVMFunctionSpecifier* AttributeSpecifier = BindingInfo.FindSpecifier(NAME_Attribute);
	bool bAttributeSpecifierRequiredButNotFound = false;

	if (BindingInfo.Name == GetFloatValueName && BindingInfo.GetNumInputs() == 2 && BindingInfo.GetNumOutputs() == 1)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetFloatValue)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetFloatValueByStringName && BindingInfo.GetNumInputs() == 1 && BindingInfo.GetNumOutputs() == 1)
	{
		if (AttributeSpecifier)
		{
			NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetFloatValueByString)::Bind(this, OutFunc, AttributeSpecifier->Value.ToString());
		}
		else
		{
			bAttributeSpecifierRequiredButNotFound = true;
		}
	}
	else if (BindingInfo.Name == GetVectorValueName && BindingInfo.GetNumInputs() == 2 && BindingInfo.GetNumOutputs() == 3)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetVectorValue)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetVectorValueByStringName && BindingInfo.GetNumInputs() == 1 && BindingInfo.GetNumOutputs() == 3)
	{
		if (AttributeSpecifier)
		{
			NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetVectorValueByString)::Bind(this, OutFunc, AttributeSpecifier->Value.ToString());
		}
		else
		{
			bAttributeSpecifierRequiredButNotFound = true;
		}
	}
	else if (BindingInfo.Name == GetVectorValueExName && BindingInfo.GetNumInputs() == 4 && BindingInfo.GetNumOutputs() == 3)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetVectorValueEx)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetVectorValueExByStringName && BindingInfo.GetNumInputs() == 3 && BindingInfo.GetNumOutputs() == 3)
	{
		if (AttributeSpecifier)
		{
			NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetVectorValueExByString)::Bind(this, OutFunc, AttributeSpecifier->Value.ToString());
		}
		else
		{
			bAttributeSpecifierRequiredButNotFound = true;
		}
	}
	else if (BindingInfo.Name == GetVector4ValueName && BindingInfo.GetNumInputs() == 2 && BindingInfo.GetNumOutputs() == 4)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetVector4Value)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetVector4ValueByStringName && BindingInfo.GetNumInputs() == 1 && BindingInfo.GetNumOutputs() == 4)
	{
		if (AttributeSpecifier)
		{
			NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetVector4ValueByString)::Bind(this, OutFunc, AttributeSpecifier->Value.ToString());
		}
		else
		{
			bAttributeSpecifierRequiredButNotFound = true;
		}
	}
	else if (BindingInfo.Name == GetQuatValueName && BindingInfo.GetNumInputs() == 3 && BindingInfo.GetNumOutputs() == 4)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetQuatValue)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetQuatValueByStringName && BindingInfo.GetNumInputs() == 2 && BindingInfo.GetNumOutputs() == 4)
	{
		if (AttributeSpecifier)
		{
			NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetQuatValueByString)::Bind(this, OutFunc, AttributeSpecifier->Value.ToString());
		}
		else
		{
			bAttributeSpecifierRequiredButNotFound = true;
		}
	}
	else if (BindingInfo.Name == GetPositionName && BindingInfo.GetNumInputs() == 1 && BindingInfo.GetNumOutputs() == 3)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetPosition)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetNormalName && BindingInfo.GetNumInputs() == 1 && BindingInfo.GetNumOutputs() == 3)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetNormal)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetTimeName && BindingInfo.GetNumInputs() == 1 && BindingInfo.GetNumOutputs() == 1)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetTime)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetVelocityName && BindingInfo.GetNumInputs() == 1 && BindingInfo.GetNumOutputs() == 3)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetVelocity)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetColorName && BindingInfo.GetNumInputs() == 1 && BindingInfo.GetNumOutputs() == 4)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetColor)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetImpulseName && BindingInfo.GetNumInputs() == 1 && BindingInfo.GetNumOutputs() == 1)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetImpulse)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetPositionAndTimeName && BindingInfo.GetNumInputs() == 1 && BindingInfo.GetNumOutputs() == 4)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetPositionAndTime)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetNumberOfPointsName && BindingInfo.GetNumInputs() == 0 && BindingInfo.GetNumOutputs() == 1)
	{
		OutFunc = FVMExternalFunction::CreateUObject(this, &UNiagaraDataInterfaceHoudini::GetNumberOfPoints);
	}
	else if (BindingInfo.Name == GetNumberOfSamplesName && BindingInfo.GetNumInputs() == 0 && BindingInfo.GetNumOutputs() == 1)
	{
		OutFunc = FVMExternalFunction::CreateUObject(this, &UNiagaraDataInterfaceHoudini::GetNumberOfSamples);
	}
	else if (BindingInfo.Name == GetNumberOfAttributesName && BindingInfo.GetNumInputs() == 0 && BindingInfo.GetNumOutputs() == 1)
	{
		OutFunc = FVMExternalFunction::CreateUObject(this, &UNiagaraDataInterfaceHoudini::GetNumberOfAttributes);
	}
	else if (BindingInfo.Name == GetLastSampleIndexAtTimeName && BindingInfo.GetNumInputs() == 1 && BindingInfo.GetNumOutputs() == 1)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetLastSampleIndexAtTime)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetPointIDsToSpawnAtTimeName && BindingInfo.GetNumInputs() == 5 && BindingInfo.GetNumOutputs() == 6)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetPointIDsToSpawnAtTime)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetSampleIndexesForPointAtTimeName && BindingInfo.GetNumInputs() == 2 && BindingInfo.GetNumOutputs() == 3)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetSampleIndexesForPointAtTime)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetPointPositionAtTimeName && BindingInfo.GetNumInputs() == 2 && BindingInfo.GetNumOutputs() == 3)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetPointPositionAtTime)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetPointValueAtTimeName && BindingInfo.GetNumInputs() == 3 && BindingInfo.GetNumOutputs() == 1)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetPointValueAtTime)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetPointValueAtTimeByStringName && BindingInfo.GetNumInputs() == 2 && BindingInfo.GetNumOutputs() == 1)
	{
		if (AttributeSpecifier)
		{
			NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetPointValueAtTimeByString)::Bind(this, OutFunc, AttributeSpecifier->Value.ToString());
		}
		else
		{
			bAttributeSpecifierRequiredButNotFound = true;
		}
	}
	else if (BindingInfo.Name == GetPointVectorValueAtTimeName && BindingInfo.GetNumInputs() == 3 && BindingInfo.GetNumOutputs() == 3)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetPointVectorValueAtTime)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetPointVectorValueAtTimeByStringName && BindingInfo.GetNumInputs() == 2 && BindingInfo.GetNumOutputs() == 3)
	{
		if (AttributeSpecifier)
		{
			NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetPointVectorValueAtTimeByString)::Bind(this, OutFunc, AttributeSpecifier->Value.ToString());
		}
		else
		{
			bAttributeSpecifierRequiredButNotFound = true;
		}
	}
	else if (BindingInfo.Name == GetPointVector4ValueAtTimeName && BindingInfo.GetNumInputs() == 3 && BindingInfo.GetNumOutputs() == 4)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetPointVector4ValueAtTime)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetPointVector4ValueAtTimeByStringName && BindingInfo.GetNumInputs() == 2 && BindingInfo.GetNumOutputs() == 4)
	{
		if (AttributeSpecifier)
		{
			NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetPointVector4ValueAtTimeByString)::Bind(this, OutFunc, AttributeSpecifier->Value.ToString());
		}
		else
		{
			bAttributeSpecifierRequiredButNotFound = true;
		}
	}
	else if (BindingInfo.Name == GetPointVectorValueAtTimeExName && BindingInfo.GetNumInputs() == 5 && BindingInfo.GetNumOutputs() == 3)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetPointVectorValueAtTimeEx)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetPointVectorValueAtTimeExByStringName && BindingInfo.GetNumInputs() == 4 && BindingInfo.GetNumOutputs() == 3)
	{
		if (AttributeSpecifier)
		{
			NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetPointVectorValueAtTimeExByString)::Bind(this, OutFunc, AttributeSpecifier->Value.ToString());
		}
		else
		{
			bAttributeSpecifierRequiredButNotFound = true;
		}
	}
	else if (BindingInfo.Name == GetPointQuatValueAtTimeName && BindingInfo.GetNumInputs() == 4 && BindingInfo.GetNumOutputs() == 4)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetPointQuatValueAtTime)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetPointQuatValueAtTimeByStringName && BindingInfo.GetNumInputs() == 3 && BindingInfo.GetNumOutputs() == 4)
	{
		if (AttributeSpecifier)
		{
			NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetPointQuatValueAtTimeByString)::Bind(this, OutFunc, AttributeSpecifier->Value.ToString());
		}
		else
		{
			bAttributeSpecifierRequiredButNotFound = true;
		}
	}
	else if (BindingInfo.Name == GetPointLifeName && BindingInfo.GetNumInputs() == 1 && BindingInfo.GetNumOutputs() == 1)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetPointLife)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetPointLifeAtTimeName && BindingInfo.GetNumInputs() == 2 && BindingInfo.GetNumOutputs() == 1)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetPointLifeAtTime)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetPointTypeName && BindingInfo.GetNumInputs() == 1 && BindingInfo.GetNumOutputs() == 1)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetPointType)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetPointNormalAtTimeName && BindingInfo.GetNumInputs() == 2 && BindingInfo.GetNumOutputs() == 3)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetPointNormalAtTime)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetPointColorAtTimeName && BindingInfo.GetNumInputs() == 2 && BindingInfo.GetNumOutputs() == 3)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetPointColorAtTime)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetPointAlphaAtTimeName && BindingInfo.GetNumInputs() == 2 && BindingInfo.GetNumOutputs() == 1)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetPointAlphaAtTime)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetPointVelocityAtTimeName && BindingInfo.GetNumInputs() == 2 && BindingInfo.GetNumOutputs() == 3)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetPointVelocityAtTime)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetPointImpulseAtTimeName && BindingInfo.GetNumInputs() == 2 && BindingInfo.GetNumOutputs() == 1)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetPointImpulseAtTime)::Bind(this, OutFunc);
	}
	else if (BindingInfo.Name == GetPointTypeAtTimeName && BindingInfo.GetNumInputs() == 2 && BindingInfo.GetNumOutputs() == 1)
	{
		NDI_FUNC_BINDER(UNiagaraDataInterfaceHoudini, GetPointTypeAtTime)::Bind(this, OutFunc);
	}
	else
	{
		UE_LOG(LogHoudiniNiagara, Error,
			TEXT("Could not find data interface function:\n\tName: %s\n\tInputs: %i\n\tOutputs: %i"),
			*BindingInfo.Name.ToString(), BindingInfo.GetNumInputs(), BindingInfo.GetNumOutputs());
		OutFunc = FVMExternalFunction();
	}

	if (bAttributeSpecifierRequiredButNotFound)
	{
		// Attribute specifier was required but was not found
		UE_LOG(
			LogHoudiniNiagara, Error,
			TEXT("Could not find specifier '%s' on function:\n\tName: %s\n\tInputs: %i\n\tOutputs: %i"),
			*NAME_Attribute.ToString(), *BindingInfo.Name.ToString(), BindingInfo.GetNumInputs(), BindingInfo.GetNumOutputs()
		);
	}
#endif // 0
}


bool UCwipcNiagaraDataInterface::Equals(const UNiagaraDataInterface* Other) const
{
	if (!Super::Equals(Other))
		return false;
#if 0
	const UNiagaraDataInterfaceHoudini* OtherHN = CastChecked<UNiagaraDataInterfaceHoudini>(Other);

	if (OtherHN != nullptr && OtherHN->HoudiniPointCacheAsset != nullptr && HoudiniPointCacheAsset)
	{
		// Just make sure the two interfaces point to the same file
		return OtherHN->HoudiniPointCacheAsset->FileName.Equals(HoudiniPointCacheAsset->FileName);
	}
#endif
	return false;
}

