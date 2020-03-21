// Fill out your copyright notice in the Description page of Project Settings.


#include "SI_PlayerController.h"
#include "..//SI_JustSurviveCharacter.h"
#include "..//UI/TowerShopMenu.h"
#include "ShopCameraPawn.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "ShopCameraPawn.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"

ASI_PlayerController::ASI_PlayerController()
{
	SetReplicates(true);
}

void ASI_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	//if (IsLocalController())
	{

		check(aPawn != nullptr && "The pawn was null");

		//If it is a character pawn and our member pawn is null set it to the passed in pawn
		if (MyOwningCharacter == nullptr)
		{
			if (ASI_JustSurviveCharacter* Char = Cast<ASI_JustSurviveCharacter>(aPawn))
			{
				MyOwningCharacter = Char;
			}
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

	InputComponent->BindAxis("ControllerMenuUp", this, &ASI_PlayerController::ControllerYForMouseY);
	InputComponent->BindAxis("ControllerMenuRight", this, &ASI_PlayerController::ControllerXForMouseX);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ASI_PlayerController::ControllerForMouseClick);
	InputComponent->BindAction("Jump", IE_Released, this, &ASI_PlayerController::ControllerForMouseUp);

}


void ASI_PlayerController::EnterTowerShopMenu()
{
	if (IsLocalPlayerController())
	{
		//Hopefully gets the pawn
		if (MyOwningCharacter == nullptr) MyOwningCharacter = Cast<ASI_JustSurviveCharacter>(GetPawn());

		TArray<AActor*> CameraPawnArray;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShopCameraPawn::StaticClass(), CameraPawnArray);
		APawn* newPawn = nullptr;
		AShopCameraPawn* camPawn = nullptr;

		for (int i = 0; i < CameraPawnArray.Num(); i++)
		{
			camPawn = Cast<AShopCameraPawn>(CameraPawnArray[i]);
			if (camPawn)
			{
				if (camPawn->GetIsActiveInShop() == false)
				{
					//camPawn->ServerEnteringShop();
					newPawn = Cast<APawn>(camPawn);
					break;
				}
			}
		}

		//TODO: we will have to check if we succesfully entered the shop
		check(newPawn && "New Pawn was Nullptr trying to possess"); //Probably forgot to add CameraPawns to the scene

		//Entering Shop Failed
		if (newPawn == nullptr) {
			check(false && "Entering Shop Failed"); 
			return;
		}

		if (MyOwningCharacter != nullptr)
			MyOwningCharacter->StopInteraction(); //TODO: Makle sure this doesn't cause issues while networking

		check(MyTowerHud && "The hud was a nullptr")

		OnUnPossess();
		FInputModeGameAndUI UUInput;
		SetInputMode(UUInput);

		if (MyTowerHud != nullptr)
		{
			if (MyTowerHud->IsInViewport() == false)
				MyTowerHud->AddToPlayerScreen();
			bShowMouseCursor = true;
		}

		OnPossess(newPawn);
		
		camPawn->SetOwner(this);
		
		//camPawn->cEnteringShop();
		ServerCallEnterShop(camPawn);
		
	}
}

void ASI_PlayerController::ServerPlaceTower_Implementation(AShopCameraPawn* camPawn, TSubclassOf<ATowerBase> tower, FTransform transform, ATowerBase* placeableTower)
{
	camPawn->ServerPlaceObject(tower, transform, placeableTower);
}

void ASI_PlayerController::ServerCallEnterShop_Implementation(AShopCameraPawn* camPawn)
{
		camPawn->EnteringShop();
}

void ASI_PlayerController::ServerExitShop_Implementation(AShopCameraPawn* camPawn)
{
		camPawn->ExitingShop();
}

void ASI_PlayerController::ExitTowerShopMenu()
{
	if (IsLocalPlayerController())
	{
		AShopCameraPawn* camPawn = Cast<AShopCameraPawn>(GetPawn());
		if (camPawn)
		{
			//camPawn->ExitingShop();
			ServerExitShop(camPawn);
			camPawn->SetOwner(nullptr);
		}

		OnUnPossess();
		FInputModeGameOnly GameInput;
		SetInputMode(GameInput);

		if (MyTowerHud && MyTowerHud->IsInViewport())
		{
			MyTowerHud->RemoveFromViewport();
			bShowMouseCursor = false;
		}

		if (MyOwningCharacter != nullptr)
			MyOwningCharacter->StopInteraction(); //TODO: I think this could be cleaner - Nick

		OnPossess(MyOwningCharacter);
	}
}

void ASI_PlayerController::InitiateGameOver()
{

	StopJumping();
	StopInteraction();
	OnClickReleased();

	FInputModeUIOnly UIMode;
	SetInputMode(UIMode);
	if (w_GameOverMenu)
	{
		GameOverMenu = CreateWidget<UUserWidget>(this, w_GameOverMenu);

		if (GameOverMenu)
		{
			GameOverMenu->AddToViewport();
		}
		bShowMouseCursor = true;
	}
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

	if (AShopCameraPawn* CamPawn = Cast<AShopCameraPawn>(GetPawn()))
	{
		CamPawn->MoveUp(val);
	}
}

void ASI_PlayerController::MoveRight(float val)
{
	if (ASI_JustSurviveCharacter * Char = Cast<ASI_JustSurviveCharacter>(GetPawn()))
	{
		Char->MoveRight(val); 
	}
	if (AShopCameraPawn* CamPawn = Cast<AShopCameraPawn>(GetPawn()))
	{
		CamPawn->MoveRight(val);
	}
}

void ASI_PlayerController::OnMouseClick()
{
	if (ASI_JustSurviveCharacter * Char = Cast<ASI_JustSurviveCharacter>(GetPawn()))
	{	
		Char->PullTrigger(); 
	}

	if (AShopCameraPawn* camPawn = Cast<AShopCameraPawn>(GetPawn()))
	{
		camPawn->OnClickPlaceObject();
	}

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
	if (IsLocalPlayerController())
	{
		MyTowerHud = CreateWidget<UTowerShopMenu>(this, TSHudTemplate);
		if (MyTowerHud)
		{
			MyTowerHud->SetOwningPlayerController(this);
			MyTowerHud->RemoveFromViewport();
		}
	}
}

void ASI_PlayerController::ControllerXForMouseX(float val)
{
	//Only if the val iusn't 0 and we are in the camera shop
	if (val != 0 && Cast<AShopCameraPawn>(GetPawn()))
	{
		float x, y;
		GetMousePosition(x, y);
		x += val;
		SetMouseLocation(x, y);
	}
}

void ASI_PlayerController::ControllerYForMouseY(float val)
{
	//Only if the val iusn't 0 and we are in the camera shop
	if (val != 0 && Cast<AShopCameraPawn>(GetPawn()))
	{
		float x, y;
		GetMousePosition(x, y);
		y += val;
		SetMouseLocation(x, y);
	}
}

void ASI_PlayerController::ControllerForMouseClick()
{
	if (Cast<AShopCameraPawn>(GetPawn()))
	{
		FViewportClient* client = GEngine->GameViewport->Viewport->GetClient();
		FKey mouseLMB = EKeys::LeftMouseButton;
		client->InputKey(GEngine->GameViewport->Viewport, 0, mouseLMB, EInputEvent::IE_Pressed);
	
	
		FSlateApplication& SlateApp = FSlateApplication::Get();
		
		FPointerEvent mouseDown(
			0, 
			SlateApp.GetCursorPos(), 
			SlateApp.GetLastCursorPos(), 
			SlateApp.GetPressedMouseButtons(), 
			EKeys::LeftMouseButton, 
			0, 
			SlateApp.GetPlatformApplication()->GetModifierKeys()
		);
		
		TSharedPtr<FGenericWindow> window;
		SlateApp.ProcessMouseButtonDownEvent(window, mouseDown);
	}	
}

void ASI_PlayerController::ControllerForMouseUp()
{
	if (Cast<AShopCameraPawn>(GetPawn()))
	{
		//trigger the mouse click release event
		FViewportClient* Client = GEngine->GameViewport->Viewport->GetClient();
		FKey MouseLMB = EKeys::LeftMouseButton;
		Client->InputKey(GEngine->GameViewport->Viewport, 0, MouseLMB, EInputEvent::IE_Released);
		
		//trigger the UI mouse click
		FSlateApplication& SlateApp = FSlateApplication::Get();
		
		FPointerEvent MouseUpEvent(
			0,
			SlateApp.CursorPointerIndex,
			SlateApp.GetCursorPos(),
			SlateApp.GetLastCursorPos(),
			SlateApp.GetPressedMouseButtons(),
			EKeys::LeftMouseButton,
			0,
			SlateApp.GetPlatformApplication()->GetModifierKeys()
		);
		
		TSharedPtr<FGenericWindow> GenWindow;
		SlateApp.ProcessMouseButtonUpEvent(MouseUpEvent);
	}
}

void ASI_PlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASI_PlayerController, MyOwningCharacter);

}
