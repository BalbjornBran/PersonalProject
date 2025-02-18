// Copyright (c) 2025, Balbjorn Bran. All rights reserved.

#include "Items/BaseItem.h"
#include "RPG_Game/RPG_Game.h"

// Constructor
ABaseItem::ABaseItem()
{
	// Enable ticking every frame for this actor
	PrimaryActorTick.bCanEverTick = true;

	// Create mesh components
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));

	// Attach meshes to RootComponent
	RootComponent = StaticMeshComponent;
	SkeletalMeshComponent->SetupAttachment(RootComponent);
}

void ABaseItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Attempt to initialize the mesh from the DataTable
	ItemInitialization();
}

// Called when the game starts
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();

}

// Function to process the DataTable and assign the appropriate mesh
void ABaseItem::ItemInitialization()
{
	if (!ItemDataTable) // Check if the DataTable is assigned
	{
		UE_LOG(ItemLog, Warning, TEXT("ItemDataTable is not assigned in %s"), *GetName());
		return;
	}

	// Try to find the row in the DataTable using RowName
	const FItemStruct* ItemData = ItemDataTable->FindRow<FItemStruct>(RowName, TEXT("ABaseItem::ItemInitialization"));

	if (!ItemData) // Check if the row exists
	{
		UE_LOG(ItemLog, Error, TEXT("Row %s not found in the assigned DataTable."), *RowName.ToString());
		return;
	}

	// Determine which Mesh to use based on MeshType
	if (ItemData->MeshType == EMeshType::StaticMesh)
	{
		// Assign the StaticMesh
		if (ItemData->StaticMesh)
		{
			StaticMeshComponent->SetStaticMesh(ItemData->StaticMesh);
			StaticMeshComponent->SetVisibility(true);
			SkeletalMeshComponent->SetVisibility(false);
			UE_LOG(ItemLog, Log, TEXT("A StaticMesh was assigned in %s"), *GetName());
		}
		else
		{
			UE_LOG(ItemLog, Warning, TEXT("StaticMesh is not assigned in row %s"), *RowName.ToString());
		}
	}
	else if (ItemData->MeshType == EMeshType::SkeletalMesh)
	{
		// Assign the SkeletalMesh
		if (ItemData->SkeletalMesh)
		{
			SkeletalMeshComponent->SetSkeletalMesh(ItemData->SkeletalMesh);
			SkeletalMeshComponent->SetVisibility(true);
			StaticMeshComponent->SetVisibility(false);
			UE_LOG(ItemLog, Log, TEXT("A SkeletalMesh was assigned in %s"), *GetName());
		}
		else
		{
			UE_LOG(ItemLog, Warning, TEXT("SkeletalMesh is not assigned in row %s"), *RowName.ToString());
		}
	}
}

void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

