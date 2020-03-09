// Copyright 2020 Samurai Intellectuals


#include "SI_PlayerState.h"
#include "Engine.h"

float ASI_PlayerState::GetMaxHealth()
{
	return m_MaxHealth;
}

float ASI_PlayerState::GetCurrentHealth()
{
	return m_CurrentHealth;
}

int ASI_PlayerState::GetCurrentMoney()
{
	return m_CurrentMoney;
}

void ASI_PlayerState::AddCurrentMoney(int newMoneyIn)
{
	check(newMoneyIn > 0 && "The value passed into this was negative");

	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Blue, TEXT("You added money"));

	//Make sure the money is positive
	if (newMoneyIn < 0)
	{
		newMoneyIn *= -1;
	}

	m_CurrentMoney += newMoneyIn;
}

void ASI_PlayerState::SubtractCurrentMoney(int newMoneyPaid)
{
	check(newMoneyPaid > 0 && "The value passed into this was negative");

	//Make sure the money is positive
	if (newMoneyPaid < 0)
	{
		newMoneyPaid *= -1;
	}

	m_CurrentMoney -= newMoneyPaid;
}

bool ASI_PlayerState::GetPlayerIsInMenu()
{
	return bPlayerIsInMenu;
}

void ASI_PlayerState::SetPlayerIsInMenu(bool toggle)
{
	bPlayerIsInMenu = toggle;
}