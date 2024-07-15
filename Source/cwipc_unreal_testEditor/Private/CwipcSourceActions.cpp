/*
* Copyright (c) <2018> Side Effects Software Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*/
#include "CwipcSourceActions.h"

#include "ToolMenus.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "EditorReimportHandler.h"
#include "HAL/FileManager.h"
#include "CwipcSource.h"

//#include "CwipcSourceEditorToolkit.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"


FCwipcSourceActions::FCwipcSourceActions()
{ }

#if 0
bool FCwipcSourceActions::CanFilter()
{
    return true;
}
#endif

#if 0
void FCwipcSourceActions::GetActions(const TArray<UObject*>& InObjects, FToolMenuSection& Section)
{
	// xxxjack this is where the magic goes to add our "Add Cwipc Source" command goes, I think.
	FAssetTypeActions_Base::GetActions(InObjects, Section);

    auto CwipcSources = GetTypedWeakObjectPtrs<UCwipcSource>(InObjects);

    Section.AddMenuEntry(
		"ReimportHoudiniPointCacheLabel",
		LOCTEXT("ReimportHoudiniPointCacheLabel", "Reimport"),
		LOCTEXT("ReimportHoudiniPointCacheTooltip", "Reimport the selected Houdini Point Cache file(s)."),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "ContentBrowser.AssetActions.ReimportAsset"),
		FUIAction(
			FExecuteAction::CreateSP(this, &FCwipcSourceActions::ExecuteReimport, CwipcSources),
			FCanExecuteAction::CreateSP(this, &FCwipcSourceActions::CanExecuteReimport, CwipcSources)
		)
    );

    Section.AddMenuEntry(
		"OpenHoudiniPointCacheLabel",
		LOCTEXT("OpenHoudiniPointCacheLabel", "Open in Text Editor"),
		LOCTEXT("OpenHoudiniPointCacheTooltip", "Open the selected Houdini Point Cache file(s) in a Text Editor."),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "ContentBrowser.AssetActions.OpenInExternalEditor"),
		FUIAction(
			FExecuteAction::CreateSP(this, &FCwipcSourceActions::ExecuteOpenInEditor, CwipcSources),
			FCanExecuteAction::CreateSP(this, &FCwipcSourceActions::CanExecuteOpenInEditor, CwipcSources)
		)
    );

	Section.AddMenuEntry(
		"FindHoudiniPointCacheInExplorer",
		LOCTEXT("FindHoudiniPointCacheInExplorer", "Find Source File"),
		LOCTEXT("FindHoudiniPointCacheInExplorerTooltip", "Opens explorer at the location of this asset's source file."),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "ContentBrowser.AssetActions.OpenInExternalEditor"),
		FUIAction(
			FExecuteAction::CreateSP(this, &FCwipcSourceActions::ExecuteFindInExplorer, CwipcSources),
			FCanExecuteAction::CreateSP(this, &FCwipcSourceActions::CanExecuteFindInExplorer, CwipcSources)
		)
	);
}
#endif


uint32 FCwipcSourceActions::GetCategories()
{
    return EAssetTypeCategories::Misc;
}


FText FCwipcSourceActions::GetName() const
{
    return INVTEXT("Cwipc Point Cloud Source Asset");
}


UClass* FCwipcSourceActions::GetSupportedClass() const
{
    return UCwipcSource::StaticClass();
}

#if 0
FText FCwipcSourceActions::GetAssetDescription(const FAssetData& AssetData) const
{
	/*
	UCwipcSource* PointCacheAsset = Cast<UCwipcSource>(AssetData.GetAsset());
	if ( !PointCacheAsset )
		return FText::GetEmpty();

	FString StrDescription;
	StrDescription += TEXT("Number of Rows: ") + FString::FromInt(PointCacheAsset->NumberOfRows) + TEXT("\n");
	StrDescription += TEXT("Number of Columns: ") + FString::FromInt(PointCacheAsset->NumberOfColumns) + TEXT("\n");
	StrDescription += TEXT("Number of Points: ") + FString::FromInt(PointCacheAsset->NumberOfPoints) + TEXT("\n");

	return FText::FromString(StrDescription);
	*/
	return FText::GetEmpty();
}
#endif


FColor FCwipcSourceActions::GetTypeColor() const
{
    return FColor::Cyan;
}

#if 0
bool FCwipcSourceActions::HasActions(const TArray<UObject*>& InObjects) const
{
    return true;
}
#endif

#if 0
class UThumbnailInfo* FCwipcSourceActions::GetThumbnailInfo(UObject* Asset) const
{
    /*
    UCwipcSource* HoudiniPointCache = CastChecked<UCwipcSource>(Asset);
    UThumbnailInfo* ThumbnailInfo = HoudiniPointCache->ThumbnailInfo;
    if (ThumbnailInfo == NULL)
    {
	ThumbnailInfo = NewObject<USceneThumbnailInfo>(HoudiniPointCache, NAME_None, RF_Transactional);
	HoudiniPointCache->ThumbnailInfo = ThumbnailInfo;
    }

    return ThumbnailInfo;
    */

    return nullptr;
}
#endif

#if 0
bool FCwipcSourceActions::CanExecuteReimport(const TArray<TWeakObjectPtr<UCwipcSource>> Objects) const
{
    for ( auto ObjIt = Objects.CreateConstIterator(); ObjIt; ++ObjIt )
    {
		auto Object = (*ObjIt).Get();
		if (!Object)
			continue;

		// If one object is valid, we can execute the action
		return true;
    }

    return false;
}
#endif

#if 0
void FCwipcSourceActions::ExecuteReimport(const TArray<TWeakObjectPtr<UCwipcSource>> Objects) const
{
    for ( auto ObjIt = Objects.CreateConstIterator(); ObjIt; ++ObjIt )
    {
		auto Object = (*ObjIt).Get();
		if (!Object)
			continue;

		FReimportManager::Instance()->Reimport(Object, true);
    }
}
#endif

#if 0
bool 
FCwipcSourceActions::CanExecuteOpenInEditor(const TArray<TWeakObjectPtr<UCwipcSource>> Objects) const
{
    for (auto ObjIt = Objects.CreateConstIterator(); ObjIt; ++ObjIt)
    {
		auto Object = ( *ObjIt ).Get();
		if (!Object)
			continue;

		const FString SourceFilePath = Object->FileName;
		if (!SourceFilePath.Len() || IFileManager::Get().FileSize(*SourceFilePath) == INDEX_NONE)
			continue;

		// If one object is valid, we can execute the action
		return true;
    }

    return false;
}
#endif

#if 0
void
FCwipcSourceActions::ExecuteOpenInEditor(const TArray<TWeakObjectPtr<UCwipcSource>> Objects) const
{
    for ( auto ObjIt = Objects.CreateConstIterator(); ObjIt; ++ObjIt )
    {
		auto Object = ( *ObjIt ).Get();
		if (!Object)
			continue;

		const FString SourceFilePath = Object->FileName;
		if (!SourceFilePath.Len() || IFileManager::Get().FileSize(*SourceFilePath) == INDEX_NONE)
			continue;

		FPlatformProcess::LaunchFileInDefaultExternalApplication(*SourceFilePath, NULL, ELaunchVerb::Open);
    }
}
#endif

#if 0
bool
FCwipcSourceActions::CanExecuteFindInExplorer(const TArray<TWeakObjectPtr<UCwipcSource>> Objects) const
{
	for ( auto ObjIt = Objects.CreateConstIterator(); ObjIt; ++ObjIt )
	{
		auto Object = (*ObjIt).Get();
		if (!Object)
			continue;

		const FString SourceFilePath = Object->FileName;
		if (!SourceFilePath.Len() || IFileManager::Get().FileSize(*SourceFilePath) == INDEX_NONE)
			continue;

		// If one object is valid, we can execute the action
		return true;
	}
	return false;
}
#endif

#if 0
void
FCwipcSourceActions::ExecuteFindInExplorer(const TArray<TWeakObjectPtr<UCwipcSource>> Objects) const
{
	for ( auto ObjIt = Objects.CreateConstIterator(); ObjIt; ++ObjIt )
	{
		auto Object = (*ObjIt).Get();
		if (!Object)
			continue;

		const FString SourceFilePath = Object->FileName;
		if ( SourceFilePath.Len() && IFileManager::Get().FileSize(*SourceFilePath) != INDEX_NONE )
			return FPlatformProcess::ExploreFolder(*SourceFilePath);
	}
}
#endif

#undef LOCTEXT_NAMESPACE
