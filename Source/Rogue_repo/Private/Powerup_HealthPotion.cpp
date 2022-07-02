// Fill out your copyright notice in the Description page of Project Settings.


#include "Powerup_HealthPotion.h"
#include "AttributeComponent.h"
#include "RogueState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

APowerup_HealthPotion::APowerup_HealthPotion()
{
	CreditCost = 50;
}

FText APowerup_HealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	UAttributeComponent* AttributeComp = UAttributeComponent::GetAttributes(InstigatorPawn);
	if (AttributeComp && AttributeComp->IsFullHealth())
	{
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already had full health.");
	}
	return FText::Format(LOCTEXT("HealthPotion_InteractMessage", "Cost {0} Credits. Restores maximum health."), CreditCost);
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
#undef LOCTEXT_NAMESPACE