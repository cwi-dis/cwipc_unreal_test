// Fill out your copyright notice in the Description page of Project Settings.

#include "CwipcSourceFactory.h"
#include "AssetTypeCategories.h"

UCwipcSourceFactoryNew::UCwipcSourceFactoryNew(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UE_LOG(LogTemp, Display, TEXT("UCwipcSourceFactoryNew::UCwipcSourceFactoryNew() on 0x%p called"), (void*)this);
	// This factory is responsible for manufacturing HoudiniEngine assets.
	SupportedClass = UCwipcSource::StaticClass();

	// This factory does not manufacture new objects from scratch.
	bCreateNew = true;

	// This factory will open the editor for each new object.
	bEditAfterNew = true;

	// This factory will import objects from files.
	bEditorImport = false;

	// Factory does not import objects from text.
	bText = false;
}


UObject*
UCwipcSourceFactoryNew::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UCwipcSource* NewCwipcSourceObject = NewObject<UCwipcSource>(InParent, Class, Name, Flags | RF_Transactional);
	UE_LOG(LogTemp, Display, TEXT("UCwipcSourceFactoryNew::FactoryCreateNew() on 0x%p called, return 0x%p"), (void*)this, (void *)NewCwipcSourceObject);
	return NewCwipcSourceObject;
}

bool UCwipcSourceFactoryNew::ShouldShowInNewMenu() const
{
	return true;
}

uint32 UCwipcSourceFactoryNew::GetMenuCategories() const
{
	return EAssetTypeCategories::Misc;
}

FText UCwipcSourceFactoryNew::GetDisplayName() const
{
	return INVTEXT("Cwipc Point Cloud Source - Synthetic");
}
