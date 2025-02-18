#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

// Macros for cleaner logging
#define GENLIB_PRINT_FILE (FString(FPaths::GetCleanFilename(TEXT(__FILE__))))
#define GENLIB_PRINT_FUNC (FString(__FUNCTION__))
#define GENLIB_PRINT_LINE (FString::FromInt(__LINE__))
#define GENLIB_LOGS_LINE (GENLIB_PRINT_FUNC + TEXT(" [") + GENLIB_PRINT_FILE + TEXT(":") + GENLIB_PRINT_LINE + TEXT("]"))

class FGeneralLibraryModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
