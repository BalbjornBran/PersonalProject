// Copyright (c) 2025, Balbjorn Bran. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FPP_InteractorComponent.h"
#include "Config/InteractionConfig.h"
#include "FPP_InteractableComponent.generated.h"

/**
 * Declares a multicast delegate that can be used to handle multiple bound event listeners.
 * This delegate type facilitates the broadcasting of events to any number of subscribed functions.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFocusChanged, bool, bIsFocused);

/**
 * Represents an interactable component that can be used to define interaction behavior for actors in a game.
 * This component includes configurable interaction data and provides mechanisms for managing interaction focus states.
 */
UCLASS( ClassGroup=(Interaction), Blueprintable ,meta=(BlueprintSpawnableComponent) )
class FPP_INTERACTION_API UFPP_InteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFPP_InteractableComponent();

protected:

	/**
	 * Handles initialization logic when the interactable component begins play.
	 * This function is called automatically when the owning actor or component is initialized
	 * and serves as an opportunity to set up required runtime behavior or states for the component.
	 */
	virtual void BeginPlay() override;

	UPROPERTY(blueprintReadWrite, EditAnywhere, Category = "Debug")
	bool bActivateDebugLogs = false;

public:

	/**
	 * Updates the focus state of the interactable component.
	 * Broadcasts an event when the focus state changes.
	 *
	 * @param bFocused A boolean indicating whether the component is currently in focus (true) or not (false).
	 */
	UFUNCTION(BlueprintCallable)
	void InFocus(const bool bFocused);
	
	/**
	 * Handles interaction with an interactable component when triggered by an interactor component and a specific input action.
	 * This function logs the provided interactor component information and the input action name for debugging or gameplay purposes.
	 *
	 * @param HitResult The hit result containing information about the interaction, including location and hit object.
	 * @param InteractorComponent A pointer to the interactor component initiating the interaction.
	 * @param InputAction A pointer to the input action associated with the interaction trigger.
	 */
	void Interact(FHitResult HitResult, UFPP_InteractorComponent* InteractorComponent, const UInputAction* InputAction);

	/**
	 * Gets the current focus state of the interactable component.
	 *
	 * @return A boolean indicating whether the component is currently in focus (true) or not (false).
	 */
	//UFUNCTION(BlueprintPure)
	//bool IsInFocus() const;
	
	/** Delegate to broadcast when the focus state changes */
	UPROPERTY(BlueprintAssignable, Category = "Components|Interaction")
	FOnFocusChanged OnFocus;

	/** Configuration data defining interaction behavior for this component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	UInteractionConfig* InteractionConfig;

	//Temporary for prototyping in blueprints
	UFUNCTION(BlueprintNativeEvent, Category = "Components|Interaction")
	void BpInteracted (FHitResult HitResult, UFPP_InteractorComponent* InteractorComponent, const UInputAction* InputAction);

	virtual void BpInteracted_Implementation(FHitResult HitResult, UFPP_InteractorComponent* InteractorComponent, const UInputAction* InputAction) { }
};
