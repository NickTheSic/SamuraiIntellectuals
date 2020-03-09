// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "../Player/SI_PlayerController.h"
#include "../Player/SI_PlayerState.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AItemBase::Purchase(ASI_PlayerController* character)
{
	//Derived classes will call Super for this function to check player's money. 

	ASI_PlayerState* ps = character->GetPlayerState<ASI_PlayerState>();

	if (ps)
	{
		//TODO: ItemBase Purchase Function
		//If has enough money return true and steal money
		//Else return false

		if (ps->GetCurrentMoney() >= ItemShopData.m_Cost)
		{
			//The player has enough money so we purchase the item and return true
			ps->SubtractCurrentMoney(ItemShopData.m_Cost);
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

void AItemBase::SellItem(ASI_PlayerController* character)
{
	//TODO: Note: It seemed easy to add.  It doesn't have to ever be used
	ASI_PlayerState* ps = character->GetPlayerState<ASI_PlayerState>();

	if (ps)
	{
		//Add money eqaul to 75% of the items value
		ps->AddCurrentMoney(ItemShopData.m_Cost * 0.75);
	}
}

FShopData AItemBase::GetShopData()
{
	return ItemShopData;
}

void AItemBase::SetShopData(FShopData newData)
{
	ItemShopData = newData;
}

void AItemBase::SetIsInShop(bool isInShop)
{
	ItemShopData.bIsInShop = isInShop;
}

