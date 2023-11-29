// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorSpawnDataStructure.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct HORDEGAME_API FActorSpawnDataStructure : public FTableRowBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Example)
	FName EntryId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Example)
	TSubclassOf<AActor> SpawnableActorClass;

	FActorSpawnDataStructure();
	~FActorSpawnDataStructure();
};
