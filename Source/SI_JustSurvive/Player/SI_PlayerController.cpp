// Fill out your copyright notice in the Description page of Project Settings.


#include "SI_PlayerController.h"
#include "..//SI_JustSurviveCharacter.h"
#include "..//UI/TowerShopMenu.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "ShopCameraPawn.h"

void ASI_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);


	//If it is a character pawn and our member pawn is null set it to the passed in pawn
	if (MyOwningCharacter == nullptr)
	{
		if (ASI_JustSurviveCharacter* Char = Cast<ASI_JustSurviveCharacter>(aPawn))
		{
			MyOwningCharacter = Char;
		}
	}
}

void ASI_PlayerController::OnUnPossess()
{
	Super::OnUnPossess();

}

void ASI_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);

	// Bind jump events
	InputComponent->BindAction("Jump", IE_Pressed, this, &ASI_PlayerController::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ASI_PlayerController::StopJumping);

	//Bind weapon event
	InputComponent->BindAxis("ChangeWeapon", this, &ASI_PlayerController::ChangeWeapon);
	InputComponent->BindAction("OnClicked", IE_Pressed, this, &ASI_PlayerController::OnMouseClick);
	InputComponent->BindAction("OnClicked", IE_Released, this, &ASI_PlayerController::OnClickReleased);
	InputComponent->BindAction("Reload", IE_Pressed, this, &ASI_PlayerController::Reload);

	//Bind PlayerInteraction
	InputComponent->BindAction("StartInteraction", IE_Pressed, this, &ASI_PlayerController::StartInteraction);
	InputComponent->BindAction("StartInteraction", IE_Released, this, &ASI_PlayerController::StopInteraction);

	// Bind movement events
	InputComponent->BindAxis("MoveForward", this, &ASI_PlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ASI_PlayerController::MoveRight);

}

void ASI_PlayerController::EnterTowerShopMenu()
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "Entering the TowerShop Menu");

	//Stop interacting with the character - due to how it is currently setup
	MyOwningCharacter->StopInteraction(); //TODO: Makle sure this doesn't cause issues while networking

	check (MyTowerHud && "THe hud was a nullptr")

	OnUnPossess();
	FInputModeGameAndUI UUInput;
	SetInputMode(UUInput);
	if (MyTowerHud)
	{
		MyTowerHud->AddToViewport();
	}
	bShowMouseCursor = true;
	TArray<AActor*> CameraPawnArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShopCameraPawn::StaticClass(), CameraPawnArray);

	APawn* newPawn = nullptr;

	for (int i = 0; i < CameraPawnArray.Num(); i++)
	{
		AShopCameraPawn* camPawn = Cast<AShopCameraPawn>(CameraPawnArray[i]);
		if (camPawn)
		{
			if (camPawn->GetIsActiveInShop() == false)
			{
				camPawn->EnteringShop();
				newPawn = Cast<APawn>(camPawn);
				break;
			}
		}
	}

	//TODO: we will have to check if we succesfully entered the shop
	check(newPawn && "New Pawn was Nullptr trying to possess"); //Probably forgot to add CameraPawns to the scene

	newPawn == nullptr ? Possess(MyOwningCharacter) : Possess(newPawn);
}

void ASI_PlayerController::ExitTowerShopMenu()
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "Exiting the TowerShop Menu");

	AShopCameraPawn* camPawn = Cast<AShopCameraPawn>(GetPawn());
	if (camPawn)
	{
		camPawn->ExitingShop();
	}

	OnUnPossess();
	FInputModeGameOnly GameInput;
	SetInputMode(GameInput);

	if (MyTowerHud)
	{
		MyTowerHud->RemoveFromViewport();
	}

	bShowMouseCursor = false;

	MyOwningCharacter->StopInteraction(); //TODO: I think this could be cleaner - Nick

	OnPossess(MyOwningCharacter);
}

void ASI_PlayerController::Jump()
{
	if (ASI_JustSurviveCharacter * Char = Cast<ASI_JustSurviveCharacter>(GetPawn()))
	{
		Char->Jump(); 
	}

}

void ASI_PlayerController::StopJumping()
{
	if (ASI_JustSurviveCharacter * Char = Cast<ASI_JustSurviveCharacter>(GetPawn()))
	{
		Char->StopJumping(); 
	}
}

void ASI_PlayerController::StartInteraction()
{
	if (ASI_JustSurviveCharacter * Char = Cast<ASI_JustSurviveCharacter>(GetPawn()))
	{
		Char->StartInteraction(); 
	}
}

void ASI_PlayerController::StopInteraction()
{
	if (ASI_JustSurviveCharacter * Char = Cast<ASI_JustSurviveCharacter>(GetPawn()))
	{
		Char->StopInteraction();
	}
}

void ASI_PlayerController::MoveForward(float val)
{
	if (ASI_JustSurviveCharacter * Char = Cast<ASI_JustSurviveCharacter>(GetPawn()))
	{
		Char->MoveForward(val); 
	}
}

void ASI_PlayerController::MoveRight(float val)
{
	if (ASI_JustSurviveCharacter * Char = Cast<ASI_JustSurviveCharacter>(GetPawn()))
	{
		Char->MoveRight(val); 
	}
}

void ASI_PlayerController::OnMouseClick()
{
	if (ASI_JustSurviveCharacter * Char = Cast<ASI_JustSurviveCharacter>(GetPawn()))
	{	
		Char->PullTrigger(); 
	
	}

	//if it is a CameraPawn
	//Place active tower?

}

void ASI_PlayerController::OnClickReleased()
{
	if (ASI_JustSurviveCharacter * Char = Cast<ASI_JustSurviveCharacter>(GetPawn()))
	{
		Char->ReleaseTrigger(); 
	}
}

void ASI_PlayerController::Reload()
{
	if (ASI_JustSurviveCharacter * Char = Cast<ASI_JustSurviveCharacter>(GetPawn()))
	{
		Char->Reload(); 
	}
}

void ASI_PlayerController::ChangeWeapon(float val)
{
	if (ASI_JustSurviveCharacter * Char = Cast<ASI_JustSurviveCharacter>(GetPawn()))
	{
		Char->ChangeWeapon(val); 
	}
}

void ASI_PlayerController::BeginPlay()
{
	Super::BeginPlay(); 

	MyTowerHud = CreateWidget<UTowerShopMenu>(this, TSHudTemplate);
	if (MyTowerHud)
	{
		MyTowerHud->SetOwningPlayerController(this);
		MyTowerHud->RemoveFromViewport();
	}
}
