// Copyright (c) 2025, Balbjorn Bran. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Engine/DataAsset.h"
#include "InteractionConfig.generated.h"

UENUM(BlueprintType)
enum class EInputType : uint8
{
	Press UMETA(DisplayName = "Press"),
	Hold UMETA(DisplayName = "Hold")
};

/**
 * @class UInteractionConfig
 * @brief Configuration data asset for interaction systems.
 *
 * This class is used to configure interaction properties for gameplay elements,
 * specifying input requirements, cooldown settings, and detection features.
 */
UCLASS(BlueprintType)
class FPP_INTERACTION_API UInteractionConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	UInputAction* RequiredInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	EInputType InputType = EInputType::Press;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float CooldownTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bChangeDetectionDistance = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (EditCondition = "bChangeDetectionDistance"))
	float DetectionDistance = 10.0f;
};
