// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionComponent.h"
#include "Action.h"
#include "../Rogue_repo.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

// Sets default values for this component's properties
UActionComponent::UActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<UAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}	
}

void UActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
	for (UAction* Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s : IsRunning:%s : Outer : %s"),
			*GetNameSafe(GetOwner()),
			*Action->ActionName.ToString(),
			Action->IsRunning() ? TEXT("True") : TEXT("False"),
			*GetNameSafe(Action->GetOuter()));
		LogOnScreen(this, ActionMsg, TextColor, 0.f);
	}
}

void UActionComponent::AddAction(AActor* Instigator, TSubclassOf<UAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}
	UAction* NewAction = NewObject<UAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);
		Actions.Add(NewAction);
		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

UAction* UActionComponent::GetAction(TSubclassOf<UAction> ActionClass) const
{
	for (UAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return Action;
		}
	}
	return nullptr;
}

void UActionComponent::RemoveAction(UAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}
	Actions.Remove(ActionToRemove);
}

bool UActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				FString DebugMsg = FString::Printf(TEXT("Failed to run : %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
				continue;
			}
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}
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
			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	return false;
}

bool UActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}
	return WroteSomething;
}

void UActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void UActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UActionComponent, Actions);
}