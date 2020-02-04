// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "../Items/WeaponBase.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::NextWeapon()
{
	for (int i = 0; i < GetWeaponCount(); i++)
	{
		if (m_CurrentWeapon == m_WeaponArray[i])
		{
			if (i < GetWeaponCount() - 1)
			{
				m_CurrentWeapon->Unequip();
				m_CurrentWeapon = m_WeaponArray[i + 1];
				m_CurrentWeapon->Equip();
				break;
			}

			else
			{
				m_CurrentWeapon->Unequip();
				m_CurrentWeapon = m_WeaponArray[0];
				m_CurrentWeapon->Equip();
				break;
			}
		}
	}
}

void UInventoryComponent::PreviousWeapon()
{
	for (int i = 0; i < GetWeaponCount(); i++)
	{
		if (m_CurrentWeapon == m_WeaponArray[i])
		{
			if (i > 0)
			{
				m_CurrentWeapon->Unequip();
				m_CurrentWeapon = m_WeaponArray[i - 1];
				m_CurrentWeapon->Equip();
				break;
			}
			else
			{
				m_CurrentWeapon->Unequip();
				m_CurrentWeapon = m_WeaponArray[GetWeaponCount() - 1];
				m_CurrentWeapon->Equip();
				break;
			}
		}
	}
}

void UInventoryComponent::SetCurrentWeapon(AWeaponBase* weapon)
{
	m_CurrentWeapon = weapon;
}

AWeaponBase* UInventoryComponent::GetCurrentWeapon()
{
	return m_CurrentWeapon;
}

int UInventoryComponent::GetWeaponCount()
{
	return m_WeaponArray.Num();
}

void UInventoryComponent::AddWeaponToInventory(AWeaponBase* weapon)
{
	if (weapon != nullptr)
	{
		m_WeaponArray.AddUnique(weapon);
		SetCurrentWeapon(weapon);
	}
}

