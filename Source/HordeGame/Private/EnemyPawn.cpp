// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPawn.h"

// Sets default values
AEnemyPawn::AEnemyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	playerPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector movementDirection = playerPosition - GetActorLocation();

	movementDirection.Normalize();

	movementDirection = movementDirection * DeltaTime * movementSpeed;
	movementDirection.Z = 0;
	AddActorLocalOffset(movementDirection, true);
}

