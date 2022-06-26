// Fill out your copyright notice in the Description page of Project Settings.


#include "Powerup_Credits.h"
#include "RogueState.h"

APowerup_Credits::APowerup_Credits()
{
	CreditsAmount = 80;
}

void APowerup_Credits::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	if (ARogueState* RS = InstigatorPawn->GetPlayerState <ARogueState>())
	{
		RS->AddCredits(CreditsAmount);
		HideAndCooldownPowerup();
	}
}
