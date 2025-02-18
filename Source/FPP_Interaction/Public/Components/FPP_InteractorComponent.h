// Copyright (c) 2025, Balbjorn Bran. All rights reserved.

#pragma once

/**
 * This component is intended to be added to a Pawn that requires detection of interactable objects,
 * If the owner is player, through player inputs to interact with objects in the game.
 * TODO: If owner is AIControlled
 * The interactable objects must implement the UFPPInteractableComponent to be compatible
 * with this interaction system.
 */

#include "CoreMinimal.h"
#include "FPP_Interaction.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "InputAction.h"
#include "FPP_InteractorComponent.generated.h"



class UFPP_InteractableComponent;
class UCameraComponent;

UCLASS( ClassGroup=(Interaction), Blueprintable, meta=(BlueprintSpawnableComponent) )
class FPP_INTERACTION_API UFPP_InteractorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFPP_InteractorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	/*
	 * Variables
	 */

	// Frequency (in seconds) to perform object detection. Default set to 0.1f (100ms)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup|Detection", meta=(ClampMin = "0.01", UIMin = "0.01"))
	float DetectionFrequency = 0.1f;

	// Reference to the owning Pawn of this component
	UPROPERTY(BlueprintReadOnly, Category=Owner, meta=(AllowPrivateAccess = "true"))
	APawn* OwningPawn;

	//Channel of the detection
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup|Detection")
	TEnumAsByte<ECollisionChannel> DetectionChannel = ECC_Visibility;

	// Maximum detection distance, default set to 200 units
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup|Detection",meta=(ClampMin = "10.0", UIMin = "10.0"))
	float DetectionDistance = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup|Detection",meta=(ClampMin = "1.0", UIMin = "1.0"))
	float DetectionSensibility = 20.0f;
	
	// Start offset of the trace from the camara
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup|Detection")
	FVector StartOffset = FVector(0.0f, 0.0f, 0.0f);

	//Activate the Debug of the traces
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	TEnumAsByte<EDrawDebugTrace::Type> DebugMode = EDrawDebugTrace::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool bActivateDebugLogs = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup|Inputs")
	TSet<TObjectPtr<class UInputAction>> InteractionActions;
	
	// Stores the result from a detection trace
	FHitResult FocusedHit;
	
private:
	//Timer handle to activate or deactivate.
	FTimerHandle FocusTimerHandle;

	//Reference of the camera
	UCameraComponent* PlayerCamera;

	
	/*
	 * Functions
	 */

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Overriding of the Activate and Deactivate to add custom functionality in the component
	virtual void Activate(bool bReset = false) override;
	virtual void Deactivate() override;

	// Toggles the function of the detection of interactables
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ToggleFocusDetection(bool activate);

	// Checks if the player is currently focused on an object
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	FORCEINLINE bool IsFocusing() const { return FocusedHit.bBlockingHit; }

	// helper function to find an interactable component on an actor
	UFPP_InteractableComponent* HasInteractableComponent(const AActor* Actor);	

	void BindInputActions();

	void HandleTriggerInputAction(const FInputActionInstance& instance);
	void HandleStopInputAction(const FInputActionInstance& instance);
	void HandleOnGoingInputAction(const FInputActionInstance& instance);

protected:

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Utilities")
    bool CanInteract(const FHitResult& CurrentHit) const;

	virtual bool CanInteract_Implementation(const FHitResult& CurrentHit) const;
	
private:
	// Initializes the component (e.g., sets up initial data or state)
	void Initialize();

	// Handles the detection logic for interactable objects
	void FocusDetection();
	
	// This function updates the currently detected object(s) based on the provided hit results from a detection trace
	void UpdateDetectedObject(TArray<FHitResult> HitResults);
	
	// Clears the currently focused object and resets related states
	void ClearFocusedObject();
};


