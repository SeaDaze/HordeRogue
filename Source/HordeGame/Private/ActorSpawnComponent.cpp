// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorSpawnComponent.h"
#include "HealthComponent.h"

// Sets default values for this component's properties
UActorSpawnComponent::UActorSpawnComponent()
{
}


// Called when the game starts
void UActorSpawnComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	if (ActorSpawnDatatable)
	{
		FActorSpawnDataStructure* row = ActorSpawnDatatable->FindRow<FActorSpawnDataStructure>("BP_Enemy", "ContextString");
		if (row)
		{
			actorClassReference = row->SpawnableActorClass;

			for (int i = 0; i < spawnCount; i++)
			{
				float angle = i * (360 / spawnCount);
				float x = 2000.0f * FMath::Sin(angle);
				float y = 2000.0f * FMath::Cos(angle);
				SpawnActor(FVector(x, y, 90.0f), FRotator(0, 0, 0));
			}
		}
	}
}

void UActorSpawnComponent::SpawnActor(FVector location, FRotator rotation)
{
	if (actorClassReference == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UActorSpawnComponent::SpawnActor() - Could not spawn actor, actorClassReference is null"));
		return;
	}

	FActorSpawnParameters spawnInfo;
	spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* enemy = GetWorld()->SpawnActor<AActor>(actorClassReference, location, rotation, spawnInfo);
	enemy->FindComponentByClass<UHealthComponent>()->InitialiseDeathNotification(this);
}

void UActorSpawnComponent::RegisterActorDeath(AActor* deadActor)
{
	UE_LOG(LogTemp, Log, TEXT("UActorSpawnComponent::RegisterActorDeath() - Spawning new actor..."));
	FVector playerPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	float angle = FMath::RandRange(0, 360);
	float x = 2000.0f * FMath::Sin(angle);
	float y = 2000.0f * FMath::Cos(angle);
	SpawnActor(FVector(x + playerPosition.X, y + playerPosition.Y, 90.0f), FRotator(0, 0, 0));
}