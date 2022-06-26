// Fill out your copyright notice in the Description page of Project Settings.


#include "Powerup_HealthPotion.h"
#include "AttributeComponent.h"
#include "RogueState.h"

APowerup_HealthPotion::APowerup_HealthPotion()
{
	CreditCost = 50;
}

void APowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	UAttributeComponent* AttributeComp = Cast<UAttributeComponent>(InstigatorPawn->GetComponentByClass(UAttributeComponent::StaticClass()));
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		if (ARogueState* RS = InstigatorPawn->GetPlayerState<ARogueState>())
		{
			if (RS->RemoveCredits(CreditCost) && AttributeComp->ApplyHealthValue(this, AttributeComp->GetHealthMax()))
			{
				HideAndCooldownPowerup();
			}
		}
	}
}
