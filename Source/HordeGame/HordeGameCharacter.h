// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include "WeaponAttachment.h"
#include "HordeGameCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AHordeGameCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	void OnOpenWeaponMenu();
	void ToggleRotate();
	void PlaceWeaponUpgrade();
	void HandleWeaponRotation();
	void HandleWeaponUpgrade();
	void CreateWeaponAttachment();
	void Fire();
	void RotateAttachmentLeft();
	void RotateAttachmentRight();
	void RotateAttachment(float delta);

	UFUNCTION()
	void AttachmentBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void AttachmentEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Open Weapon Menu Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* openWeaponMenuAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* toggleRotate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* placeWeaponUpgrade;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* fireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* rotateAttachmentLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* rotateAttachmentRight;


	UPROPERTY(EditAnywhere, Category = Weapon)
	class UDataTable* ActorSpawnDatatable;

	UPROPERTY(EditAnywhere, Category = Materials)
	UMaterial* attachmentValidMaterial;

	UPROPERTY(EditAnywhere, Category = Materials)
	UMaterial* attachmentInvalidMaterial;


	APlayerController* playerController;

	bool rotatingWeapon = false;
	bool upgradingWeapon = false;
	bool firingWeapon = false;
	AActor* weaponActor;
	AActor* weaponUpgradeActor;
	int weaponUpgradeOverlapCounter = 0;
	FName weaponUpgradeSpawnableRowName;

	float mouseStartX;
	float mouseStartY;
	FRotator weaponStartRotation;
	FRotator upgradeRotation;
	FRotator upgradeRotationOffset;
	FVector upgradeLocation;

	FRotator weaponHeldRotation;
	FVector weaponHeldLocation;
	FName weaponAttachmentCollisionProfileName;

	FHitResult hitBuildingBlock;
protected:
	virtual void BeginPlay();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:
	AHordeGameCharacter();

	UFUNCTION(BlueprintCallable, Category = Debug)
	void DebugKeyOne();
	UFUNCTION(BlueprintCallable, Category = Debug)
	void DebugKeyTwo();
	UFUNCTION(BlueprintCallable, Category = Debug)
	void DebugKeyThree();

	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
};

