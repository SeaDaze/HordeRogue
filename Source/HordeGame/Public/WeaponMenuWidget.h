// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class HORDEGAME_API UWeaponMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
	//virtual void NativeConstruct() override;

	virtual bool Initialize();

	UPROPERTY(meta = (BindWidget))
	class UButton* addUpgrade;

	UFUNCTION()
	void AddUpgradeClicked();
};
