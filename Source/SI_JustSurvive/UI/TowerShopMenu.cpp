// Copyright 2020 Samurai Intellectual - Coded by Nick Lemmon


#include "TowerShopMenu.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "TowerShopButton.h"

#include "../Player/SI_PlayerController.h" //TODO: Change this to the official player controller

//A Safe set text macro to make sure the Text object isn't null before we try to set its text
#define SetObjectText(t, s); if (t) { FText text; t->SetText(text.FromString(s));}

void UTowerShopMenu::NativeConstruct()
{
	Super::NativeConstruct();

	check(ButtonTemplate);

	if (!m_ExitButton->OnClicked.IsBound())
	{
		m_ExitButton->OnClicked.AddDynamic(this, &UTowerShopMenu::ExitMenu);
	}

	if (m_TowerList->GetChildrenCount() == 0)
	{
		InitializeShopList();
	}
	
	else	
	{
		UpdateShopList();
	}

	ClearDisplayText();
}

void UTowerShopMenu::NativeDestruct()
{
	Super::NativeDestruct();
	//No need to clear thetower shop list anymore
	ClearDisplayText();
}

void UTowerShopMenu::SetOwningPlayerController(ASI_PlayerController* pc)
{
	check(pc && "Player Controller Was null");

	if (pc)
	{
		SetOwningPlayer(pc);
		m_OwningPC = pc;
	}
}

void UTowerShopMenu::SetDisplayText(FShopData& data)
{
	//Set the text of our text objects. Will change as needed
	SetObjectText(m_NameText, data.m_ItemName.ToString());
	SetObjectText(m_CostText, FString::FromInt(data.m_Cost));
}

void UTowerShopMenu::ClearDisplayText()
{
	SetObjectText(m_NameText, "");
	SetObjectText(m_CostText, "");
}

void UTowerShopMenu::ExitMenu()
{
	if (m_OwningPC)
	{
		m_OwningPC->ExitTowerShopMenu();
	}
}

void UTowerShopMenu::InitializeShopList()
{
	//Ths creates the first list

	unsigned numberOfItems = m_ItemsToDisplay.Num();

	if (ButtonTemplate && numberOfItems != 0)
	{
		for (int i = 0; i != numberOfItems; i++)
		{
			//Loop through the items inside the Items to Display array
			//Create a button for each item and set the buttons info
			UTowerShopButton* tsButton = WidgetTree->ConstructWidget<UTowerShopButton>(ButtonTemplate); //TODO: Should we add an FName?
			AItemBase* item = Cast<AItemBase>(m_ItemsToDisplay[i]->GetDefaultObject());
			tsButton->SetObjectToCreate(item);
			tsButton->SetOwningHud(this);

			//Add the button to the TowerList scroll box object
			m_TowerList->AddChild(tsButton);
		}
	}
}

void UTowerShopMenu::UpdateShopList()
{
	//This will update every time the shop is open.

	//TODO:: Loop through and Check the cost of the item the button has.
	//If the player has less money then the button style is disabled
	//If not it is the regular style
}
