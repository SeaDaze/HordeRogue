// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"

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

void UHealthComponent::ApplyDamage(float damage, FVector damageLocation)
{
	currentHealth = FMath::Clamp(currentHealth - damage, 0, maxHealth);
	//UE_LOG(LogTemp, Warning, TEXT("ApplyDamage: Received Damage=%f"), damage);
	if (damageNumbersWidgetClass)
	{
		damageNumbersWidget = Cast<UDamageNumberWidget>(CreateWidget(GetWorld(), damageNumbersWidgetClass));
		damageNumbersWidget->AddToViewport();
		if (damageNumbersWidget)
		{
			damageNumbersWidget->OnDamageUpdated(damage, damageLocation);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("BeginPlay: damageNumbersWidget is invalid"));
		}
	}
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
	//UE_LOG(LogTemp, Log, TEXT("UHealthComponent::InitialiseDeathNotification() - Initialised death notification"));
	actorSpawnComponent = actorSpawnComponentReference;
}