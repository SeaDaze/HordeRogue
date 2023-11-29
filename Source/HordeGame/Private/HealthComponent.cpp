// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	currentHealth = maxHealth;
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::ApplyDamage(float damage)
{
	currentHealth = FMath::Clamp(currentHealth - damage, 0, maxHealth);
	if (currentHealth == 0)
	{
		if (actorSpawnComponent != nullptr)
		{
			actorSpawnComponent->RegisterActorDeath(GetOwner());
		}
		GetOwner()->Destroy();
	}
}

void UHealthComponent::InitialiseDeathNotification(UActorSpawnComponent* actorSpawnComponentReference)
{
	UE_LOG(LogTemp, Log, TEXT("UHealthComponent::InitialiseDeathNotification() - Initialised death notification"));
	actorSpawnComponent = actorSpawnComponentReference;
}