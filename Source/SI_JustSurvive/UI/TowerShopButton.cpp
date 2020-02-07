// Copyright 2020 Samurai Intellectual - Coded by Nick Lemmon


#include "TowerShopButton.h"

#include "..//Items/ItemBase.h"
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
	check(m_SpawnItem && m_TowerHud && "On click error");
	if (m_SpawnItem && m_TowerHud)
	{
		SetDisplayData();
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
	FVector2D imageSize(t->GetSurfaceWidth(), t->GetSurfaceHeight()); //TODO Get actual size

	//Keep a consistent widget style between the main 3 buttons
	//Normal is for how the button looks
	WidgetStyle.Normal.SetResourceObject(t);
	WidgetStyle.Normal.SetImageSize(imageSize);

	//Hovered for when the mouse is over it
	WidgetStyle.Hovered.SetResourceObject(t);
	WidgetStyle.Hovered.SetImageSize(imageSize);

	//Pressed for when the player clicks it
	WidgetStyle.Pressed.SetResourceObject(t);
	WidgetStyle.Pressed.SetImageSize(imageSize);

	//TODO: Maybe add a Disabled widget style?

}

void UTowerShopButton::SetDisplayData()
{
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
	check(menu && "Menu error");
	if (menu)
	{
		m_TowerHud = menu;
	}
}
