// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"
#include "RogueGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("rr.DamageMultiplier"), 1.0f, TEXT("Global damage modifier for Attribute component"), ECVF_Cheat);

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	Health = 100;
	HealthMax = 100;
	Health = HealthMax;
}

bool UAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthValue(InstigatorActor, -GetHealthMax());
}

bool UAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UAttributeComponent::ApplyHealthValue(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)	// For GOD Mode
	{
		return false;
	}
	if (Delta < 0)
	{
		float damageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= damageMultiplier;
	}
	float oldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = Health - oldHealth;
	OnUpdateHealth.Broadcast(InstigatorActor, this, Health, ActualDelta);

	//Died
	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		ARogueGameModeBase* RGM = GetWorld()->GetAuthGameMode<ARogueGameModeBase>();
		if (RGM)
		{
			RGM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}
	return ActualDelta != 0;
}

bool UAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

float UAttributeComponent::GetHealth() const
{
	return Health;
}

float UAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

UAttributeComponent* UAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UAttributeComponent>(FromActor->GetComponentByClass(UAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool UAttributeComponent::IsActorAlive(AActor* CurrentActor)
{
	UAttributeComponent* AttributeComp = GetAttributes(CurrentActor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
}
