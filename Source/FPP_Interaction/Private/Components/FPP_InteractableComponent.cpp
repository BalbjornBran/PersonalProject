// Copyright (c) 2025, Balbjorn Bran. All rights reserved.


#include "Components/FPP_InteractableComponent.h"
#include "FPP_Interaction.h"

// Sets default values for this component's properties
UFPP_InteractableComponent::UFPP_InteractableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UFPP_InteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


/**
 * Updates the focus state of the interactable component.
 *
 * @param bFocused A boolean value indicating whether the component is in focus (true)
 *                 or out of focus (false). When true, the component should display visual
 *                 or contextual cues indicating focus. Otherwise, it should hide those cues.
 */
void UFPP_InteractableComponent::InFocus(const bool bFocused)
{
	if (bFocused)
	{
		if (bActivateDebugLogs){UE_LOG(LogFPP_Interaction, Log, TEXT("TODO: Add widget calls to show focus. %s"), *FPPINTERACTION_LOGS_LINE)}
	}
	else
	{
		if (bActivateDebugLogs){UE_LOG(LogFPP_Interaction, Log, TEXT("TODO: Add widget calls to remove focus.. %s"), *FPPINTERACTION_LOGS_LINE)}
	}

	OnFocus.Broadcast(bFocused);
}

void UFPP_InteractableComponent::Interact(FHitResult HitResult, UFPP_InteractorComponent* InteractorComponent,
	const UInputAction* InputAction)
{
	BpInteracted(HitResult, InteractorComponent, InputAction);
}

