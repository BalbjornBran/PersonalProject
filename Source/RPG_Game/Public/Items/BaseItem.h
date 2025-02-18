// Copyright (c) 2025, Balbjorn Bran. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Core/RPGStructs.h"
#include "BaseItem.generated.h"

UCLASS()
class RPG_GAME_API ABaseItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseItem();

	// Called whenever the actor is updated in the editor or during spawning
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Reference to the DataTable storing the item data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Setup")
	UDataTable* ItemDataTable;

	// Name of the row in the DataTable to query
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Setup")
	FName RowName;

	// Mesh used by the item (it can be either StaticMesh or SkeletalMesh)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

private:
	// Function to initialize the item
	void ItemInitialization();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
