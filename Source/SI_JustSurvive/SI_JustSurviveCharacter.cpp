// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SI_JustSurviveCharacter.h"
#include "SI_JustSurviveProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Components/InventoryComponent.h"
#include "Items/WeaponBase.h"
#include "Interactable/TowerTerminal.h"
#include "DrawDebugHelpers.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Player/SI_PlayerController.h"
#include "Engine.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ASI_JustSurviveCharacter

ASI_JustSurviveCharacter::ASI_JustSurviveCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	//Noise Emitter for Tower's PawnSensingComponent
	NoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("Noise Emitter"));
	m_InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("Inventory"); 

	Mesh1P->SetIsReplicated(true); 

	SetReplicates(true);
	SetReplicateMovement(true); 
}

void ASI_JustSurviveCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* HitActor = nullptr;

	HitActor = GetPickableActor_LineTraceSingleByChannel(TraceCollisionParams.CollisionChannel);

	if (HitActor)
	{
		//Thought I might have needed to add this. SiceGetActorLabel is Editor only
#if WITH_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, "Hit - " + HitActor->GetActorLabel());
#endif
		if (Cast<AWeaponBase>(HitActor))
		{
			if (bIsInteracting)
			{
				AWeaponBase* tempWeapon = Cast<AWeaponBase>(HitActor);
				//m_BombActor = Cast<ABombActor>(HitActor);

				if (m_InventoryComponent->GetWeaponCount() > 0)
					m_InventoryComponent->GetCurrentWeapon()->Unequip();

				m_InventoryComponent->AddWeaponToInventory(tempWeapon);

				/*m_InventoryComponent->m_WeaponArray.AddUnique(tempWeapon);
				m_InventoryComponent->SetCurrentWeapon(tempWeapon);*/
				m_InventoryComponent->GetCurrentWeapon()->PickUp(this);
				bHasGun = true;

				GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Blue, TEXT("Weapon"));
			}
		}

		if (Cast<ATowerTerminal>(HitActor))
		{
			if (bIsInteracting)
			{
				if (ASI_PlayerController * pc = Cast< ASI_PlayerController>(GetController()))
				{
					pc->EnterTowerShopMenu();
				}
			}
		}
	}
	else
	{
		//TODO: Make E to interact text dissapear
		//Reset any sort of raytrace thing?
	}

	if (!bHasGun)
		Mesh1P->SetHiddenInGame(true);
	else if (bHasGun)
		Mesh1P->SetHiddenInGame(false);
}

void ASI_JustSurviveCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASI_JustSurviveCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASI_JustSurviveCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASI_JustSurviveCharacter::LookUpAtRate);
}

void ASI_JustSurviveCharacter::OnFire()
{
	//MakeNoise(1.0f, this, GetActorLocation());

	// try and fire a projectile
	if (ProjectileClass != NULL)
	{

		if (bHasGun)
		{
			m_InventoryComponent->GetCurrentWeapon()->Shoot();

			// try and play the sound if specified
			if (FireSound != NULL)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			// try and play a firing animation if specified

			if (FireAnimation != NULL)
			{
				// Get the animation object for the arms mesh
				UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
				if (AnimInstance != NULL)
				{
					AnimInstance->Montage_Play(FireAnimation, 1.f);
				}
			}
		}
	}
}

void ASI_JustSurviveCharacter::MoveForward(float Value)
{
	MakeNoise(1.0f, this, GetActorLocation());

	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ASI_JustSurviveCharacter::MoveRight(float Value)
{
	MakeNoise(1.0f, this, GetActorLocation());

	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ASI_JustSurviveCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASI_JustSurviveCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASI_JustSurviveCharacter::StartInteraction()
{
	bIsInteracting = true; 
}

void ASI_JustSurviveCharacter::StopInteraction()
{
	if (this != nullptr) //I was getting a nullptr so I added this - Nick
		bIsInteracting = false; 
}

void ASI_JustSurviveCharacter::ChangeWeapon(float val)
{
	if (val > 0)
		m_InventoryComponent->NextWeapon();

	else if (val < 0)
		m_InventoryComponent->PreviousWeapon();
}

AActor* ASI_JustSurviveCharacter::GetPickableActor_LineTraceSingleByChannel(ECollisionChannel CollisionChannel)
{
	if (Controller && Controller->IsLocalPlayerController())
	{
		FVector StartTrace;
		FVector Direction;
		FVector EndTrace;

		SetupRay(StartTrace, Direction, EndTrace);

		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);
		TraceParams.bTraceComplex = true;
		TraceParams.bReturnPhysicalMaterial = true;

		FHitResult Hit(ForceInit);
		UWorld* World = GetWorld();
		World->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, CollisionChannel, TraceParams); // simple trace function  ECC_PhysicsBody
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Green, false, 1, 0, 1.f);
		return Hit.GetActor();
	}
	return nullptr;
}

void ASI_JustSurviveCharacter::SetupRay(FVector& StartTrace, FVector& Direction, FVector& EndTrace)
{
	FVector CamLoc;
	FRotator CamRot;

	Controller->GetPlayerViewPoint(CamLoc, CamRot); // Get the camera position and rotation
	CamLoc = GetActorLocation();

	StartTrace = CamLoc; // trace start is the camera location
	Direction = CamRot.Vector();
	EndTrace = StartTrace + Direction * 300;
}

void ASI_JustSurviveCharacter::PullTrigger()
{
	if (bHasGun)
	{
		m_InventoryComponent->GetCurrentWeapon()->PullTrigger();
	}
}

void ASI_JustSurviveCharacter::ReleaseTrigger()
{
	if (bHasGun)
	{
		m_InventoryComponent->GetCurrentWeapon()->ReleaseTrigger();
	}
}

void ASI_JustSurviveCharacter::Reload()
{
	if (bHasGun && m_InventoryComponent->GetCurrentWeapon() != nullptr)
	{
		m_InventoryComponent->GetCurrentWeapon()->Reload(); 
	}
}