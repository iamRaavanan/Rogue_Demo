// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueState.h"
#include "Net/UnrealNetwork.h"
#include "RogueSaveGame.h"


void ARogueState::AddCredits(int32 Delta)
{
	if (!ensure(Delta > 0.0f))
	{
		return;
	}
	Credits += Delta;
	OnCreditChanged.Broadcast(this, Credits, Delta);
}

bool ARogueState::RemoveCredits(int32 Delta)
{
	if (!ensure(Delta > 0.0f))
	{
		return false;
	}
	if (Credits < Delta)
	{
		return false;
	}
	Credits -= Delta;
	OnCreditChanged.Broadcast(this, Credits, Delta);
	return true;
}

void ARogueState::SavePlayerState_Implementation(URogueSaveGame* SaveObj)
{
	if (SaveObj)
	{
		SaveObj->Credits = Credits;
	}
}

void ARogueState::LoadPlayerState_Implementation(URogueSaveGame* SaveObj)
{
	if (SaveObj)
	{
		Credits = SaveObj->Credits;
	}
}

void ARogueState::OnRep_Credits(int32 OldCredits)
{
	OnCreditChanged.Broadcast(this, Credits, Credits- OldCredits);
}

int32 ARogueState::GetCredits() const
{
	return Credits;
}

void ARogueState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARogueState, Credits);
}