// Fill out your copyright notice in the Description page of Project Settings.


#include "Action.h"

void UAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running : %s"), *GetNameSafe(this));
}

void UAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped : %s"), *GetNameSafe(this));
}

UWorld* UAction::GetWorld() const
{
	// Outer is set when creating an action via NewObject<T>
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}
	return nullptr;
}
