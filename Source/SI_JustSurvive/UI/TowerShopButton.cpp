// Copyright 2020 Samurai Intellectual - Coded by Nick Lemmon


#include "TowerShopButton.h"

#include "..//Items/ItemBase.h"
#include "../Player/SI_PlayerController.h"
#include "../Player/ShopCameraPawn.h"
#include "Engine/Texture.h"
#include "TowerShopMenu.h"

UTowerShopButton::UTowerShopButton()
{
	//Set the on click function to the function we are creating
	OnClicked.AddDynamic(this, &UTowerShopButton::OnButtonClick);
}

void UTowerShopButton::OnButtonClick()
{
	//Make sure that we have an item and a hud to display to
	check(m_SpawnItem && m_TowerHud && "On click error"); //TODO: Make sure this check is the right way
	if (m_SpawnItem && m_TowerHud)
	{
		SetDisplayData();
		GiveSpawnItemToPlayer();
	}
}

void UTowerShopButton::GiveSpawnItemToPlayer()
{
	check(m_TowerHud->GetOwningPlayer() && "The Owning player was null"); //TODO: Test this is the correct way to check

	//No need to cast it as a ASI_PlayerController since I am not yet using a specific function?
	if (ASI_PlayerController* pc = Cast<ASI_PlayerController>(m_TowerHud->GetOwningPlayer()))
	{

		check(pc->GetPawn() && "The pc Pawn was null"); //TODO Check that this is the right way to check
		if (AShopCameraPawn* shopPawn = Cast<AShopCameraPawn>(pc->GetPawn()))
		{

			check(m_SpawnItem && "The spawn item when you clicked wasn't set"); //TODO Check this is the right way
			if (m_SpawnItem)
			{
				shopPawn->SetPlaceableObject(m_SpawnItem);
			}

		}

	}
}

void UTowerShopButton::SetObjectToCreate(class AItemBase* newItem)
{
	check(newItem && "New item error"); 

	if (newItem)
	{
		m_SpawnItem = newItem;
		SetupWidgetStyle();
	}
}

void UTowerShopButton::SetupWidgetStyle()
{
	FShopData displayDat = m_SpawnItem->GetShopData();

	UTexture* t = displayDat.m_Texture;

	//TODO(Nick):  Fix the texture display on the buttons

	//Make sure we have a texture before we try to set it
	if (t != nullptr)
	{
		FVector2D imageSize(t->GetSurfaceWidth(), t->GetSurfaceHeight()); //TODO(Nick) Get actual size
		imageSize /= 10; //I am hopefully shrinking the size of the texture by a reasonable amount TODO:  

		//Normal is for how the button looks
		WidgetStyle.Normal.SetResourceObject(t);
		WidgetStyle.Normal.SetImageSize(imageSize);

		//Hovered for when the mouse is over it
		WidgetStyle.Hovered.SetResourceObject(t);
		WidgetStyle.Hovered.SetImageSize(imageSize);

		//Pressed for when the player clicks it
		WidgetStyle.Pressed.SetResourceObject(t);
		WidgetStyle.Pressed.SetImageSize(imageSize);

		//TODO(Nick): Maybe add a Disabled widget style?
	}
}

void UTowerShopButton::SetDisplayData()
{
	check(m_TowerHud && m_SpawnItem && "The if statement wont trigger");
	//If we have a hud call the function to display text
	if (m_TowerHud && m_SpawnItem)
	{
		FShopData displayDat = m_SpawnItem->GetShopData();
		m_TowerHud->SetDisplayText(displayDat);
	}
}

void UTowerShopButton::SetOwningHud(UTowerShopMenu* menu)
{
	//Make sure menu isn't a nullptr
	check(menu && "Menu is nullptr");

	if (menu)
	{
		m_TowerHud = menu;
	}
}
