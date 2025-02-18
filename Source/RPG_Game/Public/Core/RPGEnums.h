#pragma once

#include "CoreMinimal.h"
#include "RPGEnums.generated.h"

//Define the selection of the primitive type
UENUM(BlueprintType)
enum class EMeshType : uint8
{
	StaticMesh UMETA(DisplayName = "Static Mesh"),
	SkeletalMesh UMETA(DisplayName = "Skeletal Mesh")
};