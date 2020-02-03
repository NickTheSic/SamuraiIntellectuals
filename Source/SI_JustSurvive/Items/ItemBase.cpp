// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

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

bool AItemBase::Purchase(ASI_JustSurviveCharacter* character)
{
	//TODO: This function will check the player's money, and if there is enough to purchase it will reduce the player's total money. 
	//TODO: Derived classes will call Super for this function to check player's money. 

	return false;
}

