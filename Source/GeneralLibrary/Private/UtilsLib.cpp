// Copyright (c) 2025, Balbjorn Bran. All rights reserved.


#include "UtilsLib.h"

#include "GeneralLibrary.h"
#include "Camera/CameraComponent.h"

// Define the custom log category for this class
DEFINE_LOG_CATEGORY(LogUtilLib);

bool UUtilsLib::TraceFromActor(AActor* Actor, ETraceType TraceType, ETraceDirection TraceDirection,
	ETraceStartPoint StartFrom, FVector StartOffset, float TraceDistance, float Size,
	EDrawDebugTrace::Type DrawDebugType, ECollisionChannel TraceChannel, TArray<FHitResult>& OutHitResults)
	{
		// Validate Actor parameter
		if (!Actor)
		{
			UE_LOG(LogUtilLib, Error, TEXT("TraceFromActor: Actor is nullptr. %s"), *GENLIB_LOGS_LINE); 
			return false;
		}

		// Get the World from the actor
		UWorld* World = Actor->GetWorld();
		if (!World)
		{
			UE_LOG(LogUtilLib, Error, TEXT("TraceFromActor: Unable to retrieve World context from Actor %s. %s"), *Actor->GetName(), *GENLIB_LOGS_LINE);
			return false;
		}

		// Trace start and end
		FVector TraceStart, TraceEnd;
		FVector DirectionVector;

		// Determine the starting point
		// Determine the starting point based on StartFrom enum
		if (StartFrom == ETraceStartPoint::PlayerCenter)
		{
			TraceStart = Actor->GetActorLocation() + StartOffset;
			// Calculate TraceEnd based on TraceDirection enum
			
			switch (static_cast<int>(TraceDirection))
			{
			case static_cast<int>(ETraceDirection::Forward):
				DirectionVector = Actor->GetActorForwardVector();
				break;
			case static_cast<int>(ETraceDirection::Backward):
				DirectionVector = -Actor->GetActorForwardVector();
				break;
			case static_cast<int>(ETraceDirection::Righthand):
				DirectionVector = Actor->GetActorRightVector();
				break;
			case static_cast<int>(ETraceDirection::Lefthand):
				DirectionVector = -Actor->GetActorRightVector();
				break;
			case static_cast<int>(ETraceDirection::Upward):
				DirectionVector = Actor->GetActorUpVector();
				break;
			case static_cast<int>(ETraceDirection::Downward):
				DirectionVector = -Actor->GetActorUpVector();
				break;
			default:
				UE_LOG(LogUtilLib, Warning, TEXT("TraceFromActor: Invalid TraceDirection. Defaulting to Forward."));
				DirectionVector = Actor->GetActorForwardVector();
				break;
			}
		}
		else if (StartFrom == ETraceStartPoint::Camera)
		{
			UCameraComponent* CameraComponent = Actor->FindComponentByClass<UCameraComponent>();
			if (CameraComponent)
			{
				TraceStart = CameraComponent->GetComponentLocation() + StartOffset;
				// Calculate TraceEnd based on TraceDirection enum
				switch (static_cast<int>(TraceDirection))
				{
				case static_cast<int>(ETraceDirection::Forward):
					DirectionVector = CameraComponent->GetForwardVector();
					break;
				case static_cast<int>(ETraceDirection::Backward):
					DirectionVector = -CameraComponent->GetForwardVector();
					break;
				case static_cast<int>(ETraceDirection::Righthand):
					DirectionVector = CameraComponent->GetRightVector();
					break;
				case static_cast<int>(ETraceDirection::Lefthand):
					DirectionVector = -CameraComponent->GetRightVector();
					break;
				case static_cast<int>(ETraceDirection::Upward):
					DirectionVector = CameraComponent->GetUpVector();
					break;
				case static_cast<int>(ETraceDirection::Downward):
					DirectionVector = -CameraComponent->GetUpVector();
					break;
				default:
					UE_LOG(LogUtilLib, Warning, TEXT("TraceFromActor: Invalid TraceDirection. Defaulting to Forward."));
					DirectionVector = CameraComponent->GetForwardVector();
					break;
				}
			}
			else
			{
				UE_LOG(LogUtilLib, Error, TEXT("TraceFromActor: Actor %s does not have a CameraComponent."), *Actor->GetName());
				return false;
			}
		}

		TraceEnd = TraceStart + (DirectionVector * TraceDistance);

		
		// Collision Query Parameters
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Actor);

		// Perform the trace based on the TraceType enum
		bool bHit = false;
		
		switch (static_cast<int>(TraceType))
		{
		case static_cast<int>(ETraceType::Line):
			bHit = World->LineTraceMultiByChannel(OutHitResults, TraceStart, TraceEnd, TraceChannel, QueryParams);
			break;

		case static_cast<int>(ETraceType::Sphere):
			bHit = World->SweepMultiByChannel(
				OutHitResults,
				TraceStart,
				TraceEnd,
				FQuat::Identity,
				TraceChannel,
				FCollisionShape::MakeSphere(Size), // Size is the radius of the sphere
				QueryParams
			);
			break;

		case static_cast<int>(ETraceType::Capsule):
			bHit = World->SweepMultiByChannel(
				OutHitResults,
				TraceStart,
				TraceEnd,
				FQuat::Identity,
				TraceChannel,
				FCollisionShape::MakeCapsule(Size, TraceDistance * 0.5f), // Size is the radius, TraceDistance * 0.5f is height
				QueryParams
			);
			break;

		default:
			bHit = World->LineTraceMultiByChannel(OutHitResults, TraceStart, TraceEnd, TraceChannel, QueryParams);
			break;
		}

		// Debug visualization based on DrawDebugType
		if (DrawDebugType != EDrawDebugTrace::None)
		{
			FColor TraceColor = bHit ? FColor::Red : FColor::Green;
			bool bPersistent = (DrawDebugType == EDrawDebugTrace::Persistent);

			// Draw starting position and trace line
			DrawDebugLine(World, TraceStart, TraceEnd, TraceColor, bPersistent, 1.0f, 0, 1.0f);

			if (TraceType == ETraceType::Sphere  )
			{
				DrawDebugSphere(World, TraceStart, Size, 12, FColor::Blue, bPersistent, 1.0f);
				DrawDebugSphere(World, TraceEnd, Size, 12, FColor::Blue, bPersistent, 1.0f);
			}
			else if (TraceType == ETraceType::Capsule)
			{
				DrawDebugCapsule(World, TraceStart, TraceDistance * 0.5f, Size, FQuat::Identity, FColor::Blue, bPersistent, 1.0f);
				DrawDebugCapsule(World, TraceEnd, TraceDistance * 0.5f, Size, FQuat::Identity, FColor::Blue, bPersistent, 1.0f);
			}

			// Highlight hit points
			for (const FHitResult& Hit : OutHitResults)
			{
				DrawDebugSphere(World, Hit.ImpactPoint, Size * 0.5f, 12, FColor::Yellow, bPersistent, 1.0f);
			}
		}

		return bHit && OutHitResults.Num() > 0;
	}
