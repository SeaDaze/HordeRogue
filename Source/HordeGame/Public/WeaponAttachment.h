// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Components/BoxComponent.h"
#include "WeaponAttachment.generated.h"

/**
 * 
 */
UCLASS()
class HORDEGAME_API AWeaponAttachment : public AStaticMeshActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = WeaponModifiers)
	float additionalDamage = 0.0f;
	AActor* parent;

	void ApplyModifiers();

public:
	AWeaponAttachment();

	void SetAttachParent(AActor* newParent);
	
};
