#pragma once

#include "CoreMinimal.h"

// Enum to define the starting point of a trace
UENUM(BlueprintType)
enum class ETraceStartPoint : uint8
{
	PlayerCenter UMETA(DisplayName = "Player Center"),
	Camera UMETA(DisplayName = "Camera")
};

// Enum to define the direction of a trace
UENUM(BlueprintType)
enum class ETraceDirection : uint8
{
	Forward UMETA(DisplayName = "Forward"),
	Backward UMETA(DisplayName = "Backward"),
	Righthand UMETA(DisplayName = "Right Hand"),
	Lefthand UMETA(DisplayName = "Left Hand"),
	Upward UMETA(DisplayName = "Upward"),
	Downward UMETA(DisplayName = "Downward")	
};

//Enum to define the type of the trace
UENUM(BlueprintType)
enum class ETraceType : uint8
{
	Line UMETA(DisplayName = "Line"),
	Sphere UMETA(DisplayName = "Sphere"),
	Capsule UMETA(DisplayName = "Capsule")
};