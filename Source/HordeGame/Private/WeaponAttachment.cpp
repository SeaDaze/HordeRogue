// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponAttachment.h"
#include "ProjectileShooter.h"

AWeaponAttachment::AWeaponAttachment()
{
	//boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	//boxComponent->SetCollisionProfileName(FName("WeaponAttachment"));
}

void AWeaponAttachment::SetAttachParent(AActor* newParent)
{
	parent = newParent;
	ApplyModifiers();
}

void AWeaponAttachment::ApplyModifiers()
{
	if (parent)
	{
		UProjectileShooter* projectileShooterComponent = parent->GetComponentByClass<UProjectileShooter>();
		projectileShooterComponent->AddDamageModifier(additionalDamage, 1.0f);
	}
	
}
