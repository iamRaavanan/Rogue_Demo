// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueState.h"


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

int32 ARogueState::GetCredits() const
{
	return Credits;
}
