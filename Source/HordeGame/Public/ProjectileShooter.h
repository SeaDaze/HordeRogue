// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HordeGame/HordeGameProjectile.h"
#include "ProjectileShooter.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORDEGAME_API UProjectileShooter : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UProjectileShooter();

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AHordeGameProjectile> projectileClass;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	bool firing = false;
	float shotsPerSecond = 10.0f;
	float shotInterval = 0;
	float lastTimeShot = 0;
	float baseDamage = 50.0f;
	float damage;

	void FireProjectile();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartFire();
	void StopFire();

	void AddDamageModifier(float additionalDamage, float additionalMultiplier);
};
