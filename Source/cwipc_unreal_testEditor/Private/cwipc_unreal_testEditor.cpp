#include "cwipc_unreal_testEditor.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"

IMPLEMENT_GAME_MODULE(FCwipcUnrealTestEditorModule, CwipcUnrealTestEditor);

DEFINE_LOG_CATEGORY(CwipcUnrealTestEditor)

#define LOCTEXT_NAMESPACE "CwipcUnrealTestEditor"

void FCwipcUnrealTestEditorModule::StartupModule()
{
    UE_LOG(CwipcUnrealTestEditor, Warning, TEXT("CwipcUnrealTestEditor: Log Started"));
}

void FCwipcUnrealTestEditorModule::ShutdownModule()
{
    UE_LOG(CwipcUnrealTestEditor, Warning, TEXT("CwipcUnrealTestEditor: Log Ended"));
}

#undef LOCTEXT_NAMESPACE