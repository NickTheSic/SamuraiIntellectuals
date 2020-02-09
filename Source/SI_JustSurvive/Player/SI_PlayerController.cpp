// Fill out your copyright notice in the Description page of Project Settings.


#include "SI_PlayerController.h"
#include "..//SI_JustSurviveCharacter.h"
#include "..//UI/TowerShopMenu.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"

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

}

void ASI_PlayerController::EnterTowerShopMenu()
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "Entering the TowerShop Menu");

	OnUnPossess();
	//FInputModeGameAndUIOnly UIInput;
	//SetInputMode(UUInput);
	//MyPlayerHud->AddToViewport();
	//bShowMouseCursor = true;
	//TArray<AActor*> CameraPawnArray;
	//UGamePlayStatics::GetAllActorsOfClass(GetWorld(), ACameraPawn::StaticClass(), CameraPawnArray);
	//APawn* newPawn = Cast<ACameraPawn>(CameraPawnArray[0]);

	//Possess(newPawn);
}

void ASI_PlayerController::ExitTowerShopMenu()
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "Exiting the TowerShop Menu");

	OnUnPossess();
	//FInputModeGameOnly GameInput;
	//SetInputMode(GameInput);
	//MyPlayerHud->RemoveFromViewport();
	//bShowMouseCursor = false;
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
	
	}

	//if it is a CameraPawn
	//Place active tower?

}

void ASI_PlayerController::BeginPlay()
{
	Super::BeginPlay(); 

	//MyTowerHud = CreateWidget<UTowerShopMenu>(this, TSHudTemplate);
	//MyTowerHud->SetOwningPlayerController(this);
	//MyPlayerHud->RemoveFromViewport();
}
