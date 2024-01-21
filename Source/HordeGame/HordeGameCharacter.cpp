// Copyright Epic Games, Inc. All Rights Reserved.

#include "HordeGameCharacter.h"
#include "HordeGameProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectileShooter.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AHordeGameCharacter

AHordeGameCharacter::AHordeGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

void AHordeGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Add Input Mapping Context
	playerController = Cast<APlayerController>(Controller);
	if (playerController != nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	TArray<AActor*, FDefaultAllocator> actors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Weapon", actors);
	weaponActor = actors[0];
	if (weaponActor == nullptr)
		return;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Found weapon actor!"));
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, true);
	weaponActor->AttachToComponent(FirstPersonCameraComponent, AttachmentRules);

	TArray<AActor*, FDefaultAllocator> attachedActors;
	weaponActor->GetAttachedActors(attachedActors);
	UE_LOG(LogTemp, Warning, TEXT("Attached actors=%i"), attachedActors.Num());
}

void AHordeGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHordeGameCharacter::Move);
		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHordeGameCharacter::Look);

		// Weapon Upgrade
		EnhancedInputComponent->BindAction(openWeaponMenuAction, ETriggerEvent::Started, this, &AHordeGameCharacter::OnOpenWeaponMenu);
		EnhancedInputComponent->BindAction(toggleRotate, ETriggerEvent::Triggered, this, &AHordeGameCharacter::ToggleRotate);
		EnhancedInputComponent->BindAction(placeWeaponUpgrade, ETriggerEvent::Triggered, this, &AHordeGameCharacter::PlaceWeaponUpgrade);
		EnhancedInputComponent->BindAction(fireAction, ETriggerEvent::Triggered, this, &AHordeGameCharacter::Fire);
		EnhancedInputComponent->BindAction(rotateAttachmentLeft, ETriggerEvent::Triggered, this, &AHordeGameCharacter::RotateAttachmentLeft);
		EnhancedInputComponent->BindAction(rotateAttachmentRight, ETriggerEvent::Triggered, this, &AHordeGameCharacter::RotateAttachmentRight);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AHordeGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleWeaponUpgrade();
	HandleWeaponRotation();
}

void AHordeGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AHordeGameCharacter::Look(const FInputActionValue& Value)
{
	if (upgradingWeapon)
		return;

	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AHordeGameCharacter::CreateWeaponAttachment()
{
	if (!upgradingWeapon)
		return;
	if (weaponUpgradeActor != nullptr)
	{
		weaponUpgradeActor->Destroy();
		weaponUpgradeActor = nullptr;
	}

	if (ActorSpawnDatatable)
	{
		FActorSpawnDataStructure* row = ActorSpawnDatatable->FindRow<FActorSpawnDataStructure>(weaponUpgradeSpawnableRowName, "ContextString");
		if (row)
		{
			weaponUpgradeOverlapCounter = 0;
			FActorSpawnParameters spawnInfo;
			spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			weaponUpgradeActor = GetWorld()->SpawnActor<AActor>(row->SpawnableActorClass, upgradeLocation, upgradeRotation, spawnInfo);

			if (weaponUpgradeActor != nullptr)
			{
				UStaticMeshComponent* weaponUpgradeMesh = weaponUpgradeActor->GetComponentByClass<UStaticMeshComponent>();
				if (weaponUpgradeMesh)
				{
					weaponAttachmentCollisionProfileName = weaponUpgradeMesh->GetCollisionProfileName();
					weaponUpgradeMesh->SetCollisionProfileName(TEXT("WeaponPlacement"));
					weaponUpgradeMesh->SetMaterial(0, attachmentValidMaterial);
					weaponUpgradeMesh->OnComponentBeginOverlap.AddDynamic(this, &AHordeGameCharacter::AttachmentBeginOverlap);
					weaponUpgradeMesh->OnComponentEndOverlap.AddDynamic(this, &AHordeGameCharacter::AttachmentEndOverlap);
				}
				
			}
		}
	}
}
void AHordeGameCharacter::Fire()
{
	if (upgradingWeapon)
		return;
	TArray<AActor*, FDefaultAllocator> attachedActors;
	weaponActor->GetAttachedActors(attachedActors);

	if (firingWeapon)
	{
		//UE_LOG(LogTemp, Warning, TEXT("AHordeGameCharacter: (BASE) STOP Firing weapon"));
		firingWeapon = false;
		for (auto actor : attachedActors)
		{

			UProjectileShooter* projectileComponent = actor->GetComponentByClass<UProjectileShooter>();
			if (projectileComponent)
			{
				projectileComponent->StopFire();
			}
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("AHordeGameCharacter: (BASE) START Firing weapon"));
		firingWeapon = true;
		for (auto actor : attachedActors)
		{
			UProjectileShooter* projectileComponent = actor->GetComponentByClass<UProjectileShooter>();
			if (projectileComponent)
			{
				projectileComponent->StartFire();
			}
		}
	}
	
}
void AHordeGameCharacter::RotateAttachmentLeft()
{
	RotateAttachment(-100.0f * GetWorld()->GetDeltaSeconds());
}
void AHordeGameCharacter::RotateAttachmentRight()
{
	RotateAttachment(100.0f * GetWorld()->GetDeltaSeconds());
}
void AHordeGameCharacter::RotateAttachment(float delta)
{
	if (!upgradingWeapon)
		return;
	if (weaponUpgradeActor == nullptr)
		return;
	UE_LOG(LogTemp, Warning, TEXT("RotateAttachmentRight: delta=%f"), delta);
	FRotator deltaRotation(0.0f, delta, 0.0f);
	upgradeRotationOffset += deltaRotation;
}
void AHordeGameCharacter::DebugKeyOne()
{
	weaponUpgradeSpawnableRowName = "BP_PoisonCanister";
	CreateWeaponAttachment();
	UE_LOG(LogTemp, Warning, TEXT("Debug Key one pressed"));
}

void AHordeGameCharacter::DebugKeyTwo()
{
	weaponUpgradeSpawnableRowName = "BP_WeaponBarrel";
	CreateWeaponAttachment();
	UE_LOG(LogTemp, Warning, TEXT("Debug Key two pressed"));
}

void AHordeGameCharacter::DebugKeyThree()
{
	weaponUpgradeSpawnableRowName = "BP_AttachmentConnector";
	CreateWeaponAttachment();
	UE_LOG(LogTemp, Warning, TEXT("Debug Key two pressed"));
}

void AHordeGameCharacter::OnOpenWeaponMenu()
{	
	if (upgradingWeapon)
	{
		upgradingWeapon = false;
		FInputModeGameOnly inputMode;
		playerController->SetInputMode(inputMode);
		playerController->SetShowMouseCursor(false);
		weaponActor->SetActorLocation(weaponHeldLocation + GetActorLocation(), false);
		weaponActor->SetActorRotation(weaponHeldRotation + GetActorRotation());
	}
	else
	{
		upgradingWeapon = true;
		FInputModeGameAndUI inputMode;
		inputMode.SetHideCursorDuringCapture(false);
		playerController->SetInputMode(inputMode);
		playerController->SetShowMouseCursor(true);

		weaponHeldRotation = weaponActor->GetActorRotation() - GetActorRotation();
		weaponHeldLocation = weaponActor->GetActorLocation() - GetActorLocation();

		FVector ownLocation = FirstPersonCameraComponent->GetComponentLocation();
		FVector forwardDistance = FirstPersonCameraComponent->GetForwardVector() * FVector(50.0f, 50.0f, 50.0f);
		weaponActor->SetActorLocation(ownLocation + forwardDistance, false, nullptr, ETeleportType::None);
	}
}

void AHordeGameCharacter::ToggleRotate()
{
	if (rotatingWeapon)
	{
		rotatingWeapon = false;
	}
	else
	{
		rotatingWeapon = true;
		playerController->GetMousePosition(mouseStartX, mouseStartY);
		weaponStartRotation = weaponActor->GetActorRotation();
	}
}

void AHordeGameCharacter::PlaceWeaponUpgrade()
{
	if (!upgradingWeapon)
		return;
	if (weaponUpgradeOverlapCounter > 0)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Place Weapon Upgrade"));

	if (ActorSpawnDatatable)
	{
		FActorSpawnDataStructure* row = ActorSpawnDatatable->FindRow<FActorSpawnDataStructure>(weaponUpgradeSpawnableRowName, "ContextString");
		if (row)
		{
			FActorSpawnParameters spawnInfo;
			spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AActor* attachmentActor = GetWorld()->SpawnActor<AActor>(row->SpawnableActorClass, upgradeLocation, upgradeRotation, spawnInfo);
			attachmentActor->AddActorLocalRotation(upgradeRotationOffset);

			FAttachmentTransformRules attachmentRules(EAttachmentRule::KeepWorld, false);
			attachmentActor->AttachToActor(weaponActor, attachmentRules);

			AWeaponAttachment* attachment = Cast<AWeaponAttachment>(attachmentActor);
			if (attachment)
			{
				if (hitBuildingBlock.GetActor())
				{
					attachment->SetAttachParent(hitBuildingBlock.GetActor());
				}
				UStaticMeshComponent* attachmentMesh = attachment->GetComponentByClass<UStaticMeshComponent>();
				if (attachmentMesh)
				{
					attachmentMesh->SetCollisionProfileName(weaponAttachmentCollisionProfileName);
				}
			}
			
			if (weaponUpgradeActor != nullptr)
			{
				weaponUpgradeActor->Destroy();
				weaponUpgradeActor = nullptr;
			}

			//TArray<AActor*, FDefaultAllocator> attachedActors;
			//weaponActor->GetAttachedActors(attachedActors);
			//UE_LOG(LogTemp, Warning, TEXT("Attached actors=%i"), attachedActors.Num());
		}
	}
}

void AHordeGameCharacter::HandleWeaponRotation()
{
	if (!upgradingWeapon)
		return;
	if (!rotatingWeapon)
		return;
	float mouseX;
	float mouseY;
	playerController->GetMousePosition(mouseX, mouseY);
	FRotator newRotation = weaponStartRotation;
	newRotation.Yaw += (mouseStartX - mouseX);
	newRotation.Roll += (mouseStartY - mouseY);
	weaponActor->SetActorRotation(newRotation);
}

void AHordeGameCharacter::HandleWeaponUpgrade()
{
	if (!upgradingWeapon)
		return;
	if (weaponUpgradeActor == nullptr)
		return;

	// Get world position of mouse
	FVector worldPosition;
	FVector worldDirection;
	playerController->DeprojectMousePositionToWorld(worldPosition, worldDirection);
	FVector lineEnd;
	lineEnd = worldPosition + (worldDirection * 10000.0f);

	// Line trace to floor plane only
	
	FCollisionQueryParams TraceParams(FName(TEXT("CameraRay")), false, nullptr);
	GetWorld()->LineTraceSingleByObjectType(
		OUT hitBuildingBlock,
		worldPosition,
		lineEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_GameTraceChannel3),
		TraceParams
	);

	FQuat rootQuat = weaponActor->GetActorRotation().Quaternion();
	FVector up = rootQuat.GetUpVector();
	FVector rotationAxis = FVector::CrossProduct(up, hitBuildingBlock.ImpactNormal);
	rotationAxis.Normalize();
	float dot = FVector::DotProduct(up, hitBuildingBlock.ImpactNormal);
	float rotationAngle = acosf(dot);
	FQuat quat = FQuat(rotationAxis, rotationAngle);
	FQuat newQuat = quat * rootQuat;

	upgradeRotation = newQuat.Rotator();
	upgradeLocation = (hitBuildingBlock.Location + (hitBuildingBlock.ImpactNormal * 0.1f));
	//UE_LOG(LogTemp, Warning, TEXT("HandleWeaponUpgrade, Hit.ImpactNormal=%f, %f, %f"), Hit.ImpactNormal.X, Hit.ImpactNormal.Y, Hit.ImpactNormal.Z);
	weaponUpgradeActor->SetActorRotation(upgradeRotation);
	weaponUpgradeActor->AddActorLocalRotation(upgradeRotationOffset);
	weaponUpgradeActor->SetActorLocation(upgradeLocation);
}

void AHordeGameCharacter::AttachmentBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (weaponUpgradeOverlapCounter == 0)
	{
		weaponUpgradeActor->GetComponentByClass<UStaticMeshComponent>()->SetMaterial(0, attachmentInvalidMaterial);
	}
	weaponUpgradeOverlapCounter++;
	//UE_LOG(LogTemp, Warning, TEXT("AttachmentBeginOverlap, counter=%i"), weaponUpgradeOverlapCounter);
}

void AHordeGameCharacter::AttachmentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	weaponUpgradeOverlapCounter--;
	weaponUpgradeOverlapCounter = weaponUpgradeOverlapCounter < 0 ? 0 : weaponUpgradeOverlapCounter;
	if (weaponUpgradeOverlapCounter == 0)
	{
		weaponUpgradeActor->GetComponentByClass<UStaticMeshComponent>()->SetMaterial(0, attachmentValidMaterial);
	}
	//UE_LOG(LogTemp, Warning, TEXT("AttachmentEndOverlap, counter=%i"), weaponUpgradeOverlapCounter);
}