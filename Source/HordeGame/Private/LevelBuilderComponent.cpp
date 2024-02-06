// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelBuilderComponent.h"
#include "ActorSpawnDataStructure.h"
#include <Kismet/GameplayStatics.h>
#include "HordeGame/HordeGameCharacter.h"

// Sets default values for this component's properties
ULevelBuilderComponent::ULevelBuilderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULevelBuilderComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	if (ActorSpawnDatatable)
	{
		FActorSpawnDataStructure* row = ActorSpawnDatatable->FindRow<FActorSpawnDataStructure>("BP_WallTest", "ContextString");
		if (row)
		{
			actorClassReference = row->SpawnableActorClass;

			SpawnActor(FVector(2000, 0, 150.0f), FRotator(0, 0, 0));
			SpawnActor(FVector(0, -2000, 150.0f), FRotator(0, 90, 0));
			SpawnActor(FVector(-2000, 0, 150.0f), FRotator(0, 180, 0));
			SpawnActor(FVector(0, 2000, 150.0f), FRotator(0, 270, 0));
		}
	}
}

void ULevelBuilderComponent::SpawnActor(FVector location, FRotator rotation)
{
	if (actorClassReference == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UActorSpawnComponent::SpawnActor() - Could not spawn actor, actorClassReference is null"));
		return;
	}

	FActorSpawnParameters spawnInfo;
	spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* wall = GetWorld()->SpawnActor<AActor>(actorClassReference, location, rotation, spawnInfo);
	walls.push_back(wall);

}

// Called every frame
void ULevelBuilderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AHordeGameCharacter* playerCharacter = Cast<AHordeGameCharacter>(playerController->GetPawn());

	if (!playerCharacter)
		return;

	// ...
	for (AActor* actor : walls)
	{
		FVector wallPosition = actor->GetActorLocation();
		FVector playerPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

		if (FVector::Dist(wallPosition, playerPosition) > 200.0f)
		{
			FVector movementDirection = playerPosition - wallPosition;

			movementDirection.Normalize();
			movementDirection = movementDirection * DeltaTime * 100.0f;
			movementDirection.Z = 0;
			actor->AddActorWorldOffset(movementDirection, false);
		}
	}
}

void ULevelBuilderComponent::DebugKeyOne()
{
	UE_LOG(LogTemp, Warning, TEXT("LEVEL BUILDER: Debug Key one pressed"));
}

void ULevelBuilderComponent::DebugKeyTwo()
{
	UE_LOG(LogTemp, Warning, TEXT("LEVEL BUILDER: Debug Key two pressed"));
}

void ULevelBuilderComponent::DebugKeyThree()
{
	UE_LOG(LogTemp, Warning, TEXT("LEVEL BUILDER: Debug Key two pressed"));
}
