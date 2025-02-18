// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPG_GamePickUpComponent.h"

URPG_GamePickUpComponent::URPG_GamePickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void URPG_GamePickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &URPG_GamePickUpComponent::OnSphereBeginOverlap);
}

void URPG_GamePickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	ARPG_GameCharacter* Character = Cast<ARPG_GameCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
