
#pragma once

#include "CoreMinimal.h"

#include "Engine.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "UnrealEd.h"

DECLARE_LOG_CATEGORY_EXTERN(CwipcUnrealTestEditor, All, All)

class FCwipcUnrealTestEditorModule: public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

};