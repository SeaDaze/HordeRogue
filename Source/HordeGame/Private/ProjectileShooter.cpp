// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileShooter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UProjectileShooter::UProjectileShooter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	damage = baseDamage;
}

// Called when the game starts
void UProjectileShooter::BeginPlay()
{
	Super::BeginPlay();

	// ...
	shotInterval = 1 / shotsPerSecond;
}


// Called every frame
void UProjectileShooter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (firing)
	{
		float currentTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
		if (currentTime >= (lastTimeShot + shotInterval))
		{
			FireProjectile();
			lastTimeShot = currentTime;
		}
	}
}

void UProjectileShooter::FireProjectile()
{
	// Try and fire a projectile
	if (projectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			USceneComponent* projectileSpawnPoint = (USceneComponent*)GetOwner()->GetComponentsByTag(USceneComponent::StaticClass(), FName("ProjectileSpawn"))[0];

			const FRotator SpawnRotation = projectileSpawnPoint->GetComponentRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = projectileSpawnPoint->GetComponentLocation();

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// Spawn the projectile at the muzzle
			AHordeGameProjectile* projectileActor = World->SpawnActor<AHordeGameProjectile>(projectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			projectileActor->SetDamage(damage);
		}
	}
}

void UProjectileShooter::StartFire()
{
	firing = true;
}

void UProjectileShooter::StopFire()
{
	firing = false;
}

void UProjectileShooter::AddDamageModifier(float additionalDamage, float additionalMultiplier = 1.0f)
{
	damage = (damage + additionalDamage) * additionalMultiplier;
}