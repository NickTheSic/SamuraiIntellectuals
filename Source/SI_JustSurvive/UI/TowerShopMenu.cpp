// Copyright 2020 Samurai Intellectual - Coded by Nick Lemmon


#include "TowerShopMenu.h"

#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "TowerShopButton.h"

#include "GameFramework/PlayerController.h" //TODO: Change this to the official player controller

//A Safe set text macro to make sure the Text object isn't null before we try to set its text
#define SetObjectText(t, s); if (t) { FText text; t->SetText(text.FromString(s));}

void UTowerShopMenu::NativeConstruct()
{
	Super::NativeConstruct();

	check(ButtonTemplate);

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

void UTowerShopMenu::NativeDestruct()
{
	//Clear childern of the TowerList -> Due to a duplication glitch. This was the easiest fix I could think of
	m_TowerList->ClearChildren();

	Super::NativeDestruct();
}

void UTowerShopMenu::SetOwningPlayerController(APlayerController* pc)
{
	check(pc && "Player Controller issue");
	if (pc)
	{
		m_OwningPC = pc;
	}
}

void UTowerShopMenu::SetDisplayText(FShopData& data)
{
	//Set the text of our text objects. Will change as needed
	SetObjectText(m_NameText, data.m_ItemName.ToString());
	SetObjectText(m_CostText, FString::FromInt(data.m_Cost));
}
