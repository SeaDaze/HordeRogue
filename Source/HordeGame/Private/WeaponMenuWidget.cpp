// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponMenuWidget.h"
#include "Components/Button.h"

//void UWeaponMenuWidget::NativeConstruct()
//{
//    Super::NativeConstruct();
//
//    // Bind delegates here.
//}

bool UWeaponMenuWidget::Initialize()
{
	Super::Initialize();

	addUpgrade->OnClicked.AddDynamic(this, &UWeaponMenuWidget::AddUpgradeClicked);
	return true;
}

void UWeaponMenuWidget::AddUpgradeClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Add Upgrade button clicked"));
}
