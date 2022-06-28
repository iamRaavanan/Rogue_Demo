// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueController.h"

void ARogueController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	OnPawnChanged.Broadcast(InPawn);
}

void ARogueController::BeginPlayingState()
{
	BlueprintBeginPlayingState();
}

void ARogueController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	OnPlayerStateReceived.Broadcast(PlayerState);
}
