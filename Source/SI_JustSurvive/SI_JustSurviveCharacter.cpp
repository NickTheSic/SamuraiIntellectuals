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
#include "DrawDebugHelpers.h"
#include "Components/PawnNoiseEmitterComponent.h"

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

	// Create a gun mesh component
	//FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	//FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	//FP_Gun->bCastDynamicShadow = false;
	//FP_Gun->CastShadow = false;
	//// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	//FP_Gun->SetupAttachment(RootComponent);
	//FP_Gun->SetHiddenInGame(true);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;

	//Noise Emitter for Tower's PawnSensingComponent
	NoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("Noise Emitter"));
	m_InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("Inventory"); 
}

void ASI_JustSurviveCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* HitActor = nullptr;

	HitActor = GetPickableActor_LineTraceSingleByChannel(TraceCollisionParams.CollisionChannel);

	if (HitActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, "Hit - " + HitActor->GetActorLabel());

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

				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Weapon"));
			}
		}
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

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	//FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASI_JustSurviveCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASI_JustSurviveCharacter::OnFire);

	//Bind weapon event
	PlayerInputComponent->BindAxis("ChangeWeapon", this, &ASI_JustSurviveCharacter::ChangeWeapon); 
	PlayerInputComponent->BindAction("PullTrigger", IE_Pressed, this, &ASI_JustSurviveCharacter::PullTrigger); 
	PlayerInputComponent->BindAction("ReleaseTrigger", IE_Released, this, &ASI_JustSurviveCharacter::ReleaseTrigger); 
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ASI_JustSurviveCharacter::Reload); 

	//Bind PlayerInteraction
	PlayerInputComponent->BindAction("StartInteraction", IE_Pressed, this, &ASI_JustSurviveCharacter::StartInteraction);
	PlayerInputComponent->BindAction("StopInteraction", IE_Released, this, &ASI_JustSurviveCharacter::StopInteraction);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ASI_JustSurviveCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ASI_JustSurviveCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASI_JustSurviveCharacter::MoveRight);

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
	MakeNoise(1.0f, this, GetActorLocation());

	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			if (bUsingMotionControllers)
			{
				const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
				const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
				World->SpawnActor<ASI_JustSurviveProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
			else
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

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

void ASI_JustSurviveCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ASI_JustSurviveCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ASI_JustSurviveCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
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

bool ASI_JustSurviveCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ASI_JustSurviveCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &ASI_JustSurviveCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ASI_JustSurviveCharacter::TouchUpdate);
		return true;
	}
	
	return false;
}

void ASI_JustSurviveCharacter::StartInteraction()
{
	bIsInteracting = true; 
}

void ASI_JustSurviveCharacter::StopInteraction()
{
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