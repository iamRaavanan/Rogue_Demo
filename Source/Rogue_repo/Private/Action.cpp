// Fill out your copyright notice in the Description page of Project Settings.


#include "Action.h"
#include "ActionComponent.h"
#include "../Rogue_repo.h"
#include "Net/UnrealNetwork.h"

void UAction::Initialize(UActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

bool UAction::CanStart_Implementation(AActor* Instigator)
{
	if(IsRunning())
		return false;

	UActionComponent* Comp = GetOwningComponent();
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
		return false;

	return true;
}

void UAction::StartAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Running : %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);
	
	UActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantTags);

	bIsRunning = true;
}

void UAction::StopAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Stopped : %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

	//ensureAlways(bIsRunning);

	UActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantTags);

	bIsRunning = false;
}

bool UAction::IsRunning() const
{
	return bIsRunning;
}


UWorld* UAction::GetWorld() const
{
	// Outer is set when creating an action via NewObject<T>
	/*UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}*/
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}
	return nullptr;
}

UActionComponent* UAction::GetOwningComponent() const
{
	// 1st option - Not optimal
	// As, the GetComponentByClass iterate over all the component on each call
	//AActor* Actor = Cast<AActor>(GetOuter());
	//return Actor->GetComponentByClass(UActionComponent::StaticClass());

	//return Cast<UActionComponent>(GetOuter());
	return ActionComp;
}

void UAction::OnRep_IsRunning()
{
	if (bIsRunning)
	{
		StartAction(nullptr);
	}
	else
	{
		StopAction(nullptr);
	}
}

void UAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UAction, bIsRunning);
	DOREPLIFETIME(UAction, ActionComp);
}
