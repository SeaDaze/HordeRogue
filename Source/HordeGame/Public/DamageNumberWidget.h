// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageNumberWidget.generated.h"

/**
 * 
 */
UCLASS()
class HORDEGAME_API UDamageNumberWidget : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float damageAmount = 0;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnDamageUpdated(float damage, FVector damageLocation);
};
