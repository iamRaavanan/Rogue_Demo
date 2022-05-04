// Fill out your copyright notice in the Description page of Project Settings.


#include "Powerup_HealthPotion.h"
#include "AttributeComponent.h"

APowerup_HealthPotion::APowerup_HealthPotion()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);
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
		if (AttributeComp->ApplyHealthValue(AttributeComp->GetHealthMax()))
		{
			HideAndCooldownPowerup();
		}
	}
}
