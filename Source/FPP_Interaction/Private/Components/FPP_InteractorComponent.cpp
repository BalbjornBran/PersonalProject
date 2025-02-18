// Copyright (c) 2025, Balbjorn Bran. All rights reserved.

#include "Components/FPP_InteractorComponent.h"
#include "GameFramework/Pawn.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "FPP_Interaction.h"
#include "GeneralLibrary/Public/UtilsLib.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "Components/FPP_InteractableComponent.h"



// Sets default values for this component's properties
UFPP_InteractorComponent::UFPP_InteractorComponent()
	: OwningPawn(nullptr), PlayerCamera(nullptr) // Initialize members safely
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}



// Called when the game starts
void UFPP_InteractorComponent::BeginPlay()
{
	Super::BeginPlay();

	// Call the Initialize function
	Initialize();

	OwningPawn = Cast<APawn>(GetOwner());
	if (!OwningPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("UFPP_InteractorComponent: OwningPawn not found!"));
		return;
	}
	BindInputActions();
}


// Called every frame
void UFPP_InteractorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


/**
 * Activates the interactor component and enables focus detection.
 * @param bReset If true, the component will reset before activation.
 */
void UFPP_InteractorComponent::Activate(bool bReset)
{
	Super::Activate(bReset);
	
	ToggleFocusDetection(true);
}


/**
 * Deactivates the interactor component and disables focus detection functionality.
 */
void UFPP_InteractorComponent::Deactivate()
{
	Super::Deactivate();

	ToggleFocusDetection(false);
}


/**
 * Toggles the functionality of detecting interactable objects by enabling or disabling focus detection.
 * @param activate If true, enables periodic focus detection. If false, stops focus detection.
 */
void UFPP_InteractorComponent::ToggleFocusDetection(bool activate)
{
	//Somehow when compiling or modifying blueprints in editor, the Activate and Deactivate functions from component gets called a lot.
	//This is a quick skip for that.
	if (!GetWorld() || !GetWorld()->IsGameWorld())
	{
		if (bActivateDebugLogs)
		{
			UE_LOG(LogFPP_Interaction, Warning, TEXT("FocusDetection will not run because the world is not in play mode."));	
		}
		return;
	}
	
	if (activate)
	{
		if (GetWorld() == nullptr || !GetWorld()->IsGameWorld())
		{
			UE_LOG(LogFPP_Interaction, Error, TEXT("GetWorld() is nullptr. %s"), *FPPINTERACTION_LOGS_LINE);
			return;
		}
		// Set a timer to trigger the FocusDetection function every DetectionFrequency seconds
		GetWorld()->GetTimerManager().SetTimer(
		FocusTimerHandle, /* Handle for the timer */
		this, /* Target component */
        &UFPP_InteractorComponent::FocusDetection, /* Function to execute */
		DetectionFrequency, /* Interval in seconds */
		true /* Enable loop */
		);
	}
	else
	{
		// Stop the timer
		GetWorld()->GetTimerManager().ClearTimer(FocusTimerHandle);
	}
}


/**
 * Checks if the specified actor has an interactable component.
 * @param Actor The actor to check for an interactable component.
 * @return A pointer to the UFPP_InteractableComponent if found, otherwise nullptr.
 */
UFPP_InteractableComponent* UFPP_InteractorComponent::HasInteractableComponent(const AActor* Actor)
{
	return Actor ? Actor->FindComponentByClass<UFPP_InteractableComponent>() : nullptr;
}


/**
 * Binds input actions associated with interaction to the owning character's input component.
 * Ensures that the component is properly associated with an owning pawn and character,
 * and binds each interaction action to its corresponding handler.
 * Logs errors if the owning pawn or input component is invalid.
 */
void UFPP_InteractorComponent::BindInputActions()
{
	if (!OwningPawn)
	{
		UE_LOG(LogFPP_Interaction, Error, TEXT("OwningPawn is null in UFPP_InteractorComponent!"));
		return;
	}
	ACharacter* OwningCharacter = Cast<ACharacter>(OwningPawn);
	if (!OwningCharacter)
	{
		UE_LOG(LogFPP_Interaction, Warning, TEXT("OwningPawn is not a valid ACharacter in UFPP_InteractorComponent!"));
		return;
	}

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(OwningCharacter->InputComponent);
	if (!EnhancedInput)
	{
		UE_LOG(LogFPP_Interaction, Warning, TEXT("EnhancedInputComponent not found on the Owner Character!"));
		return;
	}

	for (TObjectPtr<UInputAction> InputAction : InteractionActions)
	{
		if (InputAction)
		{
			EnhancedInput->BindAction(InputAction, ETriggerEvent::Triggered, this, &UFPP_InteractorComponent::HandleTriggerInputAction);
			EnhancedInput->BindAction(InputAction, ETriggerEvent::Ongoing, this, &UFPP_InteractorComponent::HandleOnGoingInputAction);
			EnhancedInput->BindAction(InputAction, ETriggerEvent::Canceled, this, &UFPP_InteractorComponent::HandleStopInputAction);
			EnhancedInput->BindAction(InputAction, ETriggerEvent::Completed, this, &UFPP_InteractorComponent::HandleStopInputAction);
		}
	}

	if (bActivateDebugLogs)
	{
		UE_LOG(LogFPP_Interaction, Log, TEXT("InputActions successfully bound in UFPP_InteractorComponent!"));	
	}
	
}


/**
 * Handles input action events and determines if interaction is possible.
 * Logs whether the interaction conditions are met.
 * @param instance The input action instance triggering this function.
 */
void UFPP_InteractorComponent::HandleTriggerInputAction(const FInputActionInstance& instance)
{
	if (bActivateDebugLogs)
	{
		UE_LOG(LogFPP_Interaction, Log, TEXT("InputAction triggered in UFPP_InteractorComponent!"));
	}
	
	if (CanInteract(FocusedHit))
	{
		if (bActivateDebugLogs)
		{
			UE_LOG(LogFPP_Interaction, Log, TEXT("CanInteract is true!"));
		}
		
		if (UFPP_InteractableComponent* InteractableComponent = HasInteractableComponent(FocusedHit.GetActor()))
		{
			InteractableComponent->Interact(FocusedHit, this, instance.GetSourceAction());
		}
	}
}

void UFPP_InteractorComponent::HandleStopInputAction(const FInputActionInstance& instance)
{
}

void UFPP_InteractorComponent::HandleOnGoingInputAction(const FInputActionInstance& instance)
{
}


/**
 * Determines if the interactor component can interact with the object currently being focused on.
 * @param CurrentHit The current hit result from a detection trace, representing the object under focus.
 * @return True if the component is focusing on an object, allowing interaction; false otherwise.
 */
bool UFPP_InteractorComponent::CanInteract_Implementation(const FHitResult& CurrentHit) const
{
	return this->IsFocusing();
}


/**
 * Initializes the interactor component by setting up references to the owning Pawn
 * and the player's camera component, if applicable. Logs the initialization process
 * and validates the ownership of the component.
 * - Retrieves a reference to the owning Pawn of the component.
 * - Verifies if the owner is player-controlled.
 * - Finds and stores a reference to the camera component, if available.
 * - Logs errors if the owning Pawn is invalid or not player-controlled.
 */
void UFPP_InteractorComponent::Initialize()
{
	if (bActivateDebugLogs)
	{
		UE_LOG(LogFPP_Interaction, Log, TEXT("Initialize function called. %s"), *FPPINTERACTION_LOGS_LINE);
	}
	
	// Get a reference to the owning Pawn
	OwningPawn = Cast<APawn>(GetOwner());

	// Check if the owner is controlled by a player
	if (OwningPawn && OwningPawn->IsPlayerControlled())
	{
		if (bActivateDebugLogs)
		{
			UE_LOG(LogFPP_Interaction, Log, TEXT("OwningPawn set successfully to %s. %s"), *OwningPawn->GetName(), *FPPINTERACTION_LOGS_LINE);	
		}
		
		// Attempt to get the camera component (if applicable)
		PlayerCamera = OwningPawn->FindComponentByClass<UCameraComponent>();
	}
	else
	{
		UE_LOG(LogFPP_Interaction, Error, TEXT("Failed to set OwningPawn! %s"), *FPPINTERACTION_LOGS_LINE);
	}
}


/**
 * Updates the detected objects from a list of hit results, identifies interactable components,
 * and sets focus on a new interactable object if it is not already focused.
 * @param HitResults An array of hit results derived from object detection traces.
 */
void UFPP_InteractorComponent::UpdateDetectedObject(TArray<FHitResult> HitResults)
{
	for (const FHitResult& HitResult : HitResults)
	{
		// Get the detected actor from the hit result
		const AActor* Actor = HitResult.GetActor();
		if (!Actor)
		{
			continue; // Skip if the actor is null
		}

		// Try to find the interactable component on the detected actor
		UFPP_InteractableComponent* Component = HasInteractableComponent(Actor);
		if (!Component)
		{
			continue; // Skip if no interactable component is found
		}

		if (Actor != FocusedHit.GetActor())
		{
			ClearFocusedObject();
			Component->InFocus(true);
			FocusedHit = HitResult;
		}
	}	
}


/**
 * Clears the currently focused object by resetting the focus state of its interactable component, if any.
 * This method ensures the interactable component is notified of losing focus before clearing the stored hit result.
 */
void UFPP_InteractorComponent::ClearFocusedObject()
{
	if (UFPP_InteractableComponent* Component = HasInteractableComponent(FocusedHit.GetActor()))
	{
		Component->InFocus(false);
	}
	FocusedHit.Reset();
}


/**
 * Performs detection of interactable objects within a specified range and direction.
 * This function uses a sphere trace originating from the player's camera to identify potential objects to interact with.
 * If an object is detected, it updates the focus state; otherwise, it clears the current focus.
 */
void UFPP_InteractorComponent::FocusDetection()
{
	if (bActivateDebugLogs)
	{
		UE_LOG(LogFPP_Interaction, Log, TEXT("FocusDetection function called. %s"), *FPPINTERACTION_LOGS_LINE);	
	}
	//Tracing from the Camara of the player.
	TArray<FHitResult> HitResults;
	bool bIsFocusing = UUtilsLib::TraceFromActor(
		OwningPawn,
		ETraceType::Sphere,
		ETraceDirection::Forward,
		ETraceStartPoint::Camera,
		StartOffset,
		DetectionDistance,
		DetectionSensibility,
		DebugMode,
		DetectionChannel,
		HitResults
		);

	if (bIsFocusing)
	{
		UpdateDetectedObject(HitResults);
	}
	else
	{
		ClearFocusedObject();
	}
}








