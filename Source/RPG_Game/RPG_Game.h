// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

//Log Categories
DECLARE_LOG_CATEGORY_EXTERN(ItemLog, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(DetectionLog, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(RPGLog, Log, All);

//DebugMacros
#define RPG_PRINT_FUNC (FString(__FUNCTION__))
#define RPG_PRINT_LINE (FString::FromInt(__LINE__))
#define RPG_LOGS_LINE (RPG_PRINT_FUNC + TEXT(" [") + RPG_PRINT_LINE + TEXT("]"))