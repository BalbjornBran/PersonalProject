#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

// Macros for cleaner logging
#define FPPINTERACTION_PRINT_FILE (FString(FPaths::GetCleanFilename(TEXT(__FILE__))))
#define FPPINTERACTION_PRINT_FUNC (FString(__FUNCTION__))
#define FPPINTERACTION_PRINT_LINE (FString::FromInt(__LINE__))
#define FPPINTERACTION_LOGS_LINE (FPPINTERACTION_PRINT_FUNC + TEXT(" [") + FPPINTERACTION_PRINT_FILE + TEXT(":") + FPPINTERACTION_PRINT_LINE + TEXT("]"))

DECLARE_LOG_CATEGORY_EXTERN(LogFPP_Interaction, Log, All);

class FFPP_InteractionModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
