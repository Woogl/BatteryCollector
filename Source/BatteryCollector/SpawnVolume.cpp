// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnVolume.h"
#include <Kismet/KismetMathLibrary.h>
#include "Pickup.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create the Box Component to represent the spawn volume
	WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn"));
	RootComponent = WhereToSpawn;

	// Set the spawn delay range
	SpawnDelayRengeLow = 1.0f;
	SpawnDelayRengeHigh = 4.5f;
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetRandomPointInVolume()
{
	FVector SpawnOrigin = WhereToSpawn->Bounds.Origin;
	FVector SpawnExtend = WhereToSpawn->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtend);
}

void ASpawnVolume::SetSpawningActive(bool bShouldSpawn)
{
	if (bShouldSpawn)
	{
		// Set the timer on spawn pickups
		SpawnDelay = FMath::FRandRange(SpawnDelayRengeLow, SpawnDelayRengeHigh);
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, SpawnDelay, false);
	}
	else
	{
		// clear the timer
		GetWorldTimerManager().ClearTimer(SpawnTimer);
	}
}

void ASpawnVolume::SpawnPickup()
{
	// If we have set something to spawn:
	if (WhatToSpawn != NULL)
	{
		// Check for valid World:
		UWorld* const World = GetWorld();
		if (World)
		{
			// Set the spawn parameter
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// Get a random location to spawn at
			FVector SpawnLocation = GetRandomPointInVolume();

			// Get a random rotation for the spawned item
			FRotator SpawnRotation;
			SpawnRotation.Yaw = FMath::FRand() * 360.0f;
			SpawnRotation.Pitch = FMath::FRand() * 360.0f;
			SpawnRotation.Roll = FMath::FRand() * 360.0f;

			// Spawn the pickup
			APickup* const SpawnedPickup = World->SpawnActor<APickup>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);

			SpawnDelay = FMath::FRandRange(SpawnDelayRengeLow, SpawnDelayRengeHigh);
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, SpawnDelay, false);
		}
	}
}

