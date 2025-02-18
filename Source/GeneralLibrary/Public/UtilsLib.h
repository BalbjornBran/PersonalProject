// Copyright (c) 2025, Balbjorn Bran. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilsEnums.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UtilsLib.generated.h"

/**
 * 
 */

// Declare a custom log category for this class
DECLARE_LOG_CATEGORY_EXTERN(LogUtilLib, Log, All);

UCLASS()
class GENERALLIBRARY_API UUtilsLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Tracing")
	static bool TraceFromActor(
		AActor* Actor,
		ETraceType TraceType,
		ETraceDirection TraceDirection,
		ETraceStartPoint StartFrom,
		FVector StartOffset,
		float TraceDistance,
		float Size,
		EDrawDebugTrace::Type DrawDebugType,
		ECollisionChannel TraceChannel,
		TArray<FHitResult>& OutHitResults
	);
	
	
};
