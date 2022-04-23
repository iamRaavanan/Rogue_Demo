// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	Health = 100;
}


bool UAttributeComponent::ApplyHealthValue(float Delta)
{
	Health += Delta;
	OnUpdateHealth.Broadcast(nullptr, this, Health, Delta);
	return true;
}
