// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorSpawnDataStructure.h"
#include "ActorSpawnComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORDEGAME_API UActorSpawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorSpawnComponent();

	UPROPERTY(EditAnywhere)
	class UDataTable* ActorSpawnDatatable;

	UPROPERTY(EditAnywhere, Category = "Pawn Variables")
	int spawnCount = 100;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void RegisterActorDeath(AActor* deadActor);

private:

	TSubclassOf<AActor> actorClassReference;
	void SpawnActor(FVector location, FRotator rotation);
};
