#include "cwipc_unreal_testEditor.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"
#include "CwipcSourceActions.h"

IMPLEMENT_GAME_MODULE(FCwipcUnrealTestEditorModule, CwipcUnrealTestEditor);

DEFINE_LOG_CATEGORY(CwipcUnrealTestEditor)

#define LOCTEXT_NAMESPACE "CwipcUnrealTestEditor"

void FCwipcUnrealTestEditorModule::StartupModule()
{
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    auto GameAssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Cwipc")), LOCTEXT("CwipcCategory", "Cwipc"));
    AssetTools.RegisterAssetTypeActions(MakeShareable(new FCwipcSourceActions(GameAssetCategory)));
    UE_LOG(CwipcUnrealTestEditor, Warning, TEXT("CwipcUnrealTestEditor: Log Started"));
}

void FCwipcUnrealTestEditorModule::ShutdownModule()
{
    UE_LOG(CwipcUnrealTestEditor, Warning, TEXT("CwipcUnrealTestEditor: Log Ended"));
}

#undef LOCTEXT_NAMESPACE