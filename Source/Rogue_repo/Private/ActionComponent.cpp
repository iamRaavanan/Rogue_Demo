// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionComponent.h"
#include "Action.h"

// Sets default values for this component's properties
UActionComponent::UActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UAction> ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
	}
}

void UActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UActionComponent::AddAction(TSubclassOf<UAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}
	UAction* NewAction = NewObject<UAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool UActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool UActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StopAction(Instigator);
			return true;
		}
	}
	return false;
}
