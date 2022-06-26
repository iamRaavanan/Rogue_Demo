// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueController.h"

void ARogueController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	OnPawnChanged.Broadcast(InPawn);
}
