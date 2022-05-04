// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	Health = 100;
	HealthMax = 100;
	Health = HealthMax;
}


bool UAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UAttributeComponent::ApplyHealthValue(float Delta)
{
	float oldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = Health - oldHealth;
	OnUpdateHealth.Broadcast(nullptr, this, Health, ActualDelta);

	return ActualDelta != 0;
}

bool UAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

float UAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}
