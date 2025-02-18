#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RPGEnums.h"
#include "RPGStructs.generated.h"

USTRUCT(BlueprintType)
struct FItemStruct : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMeshType MeshType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> SkeletalMesh;
};
