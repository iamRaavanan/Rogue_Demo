// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerupActor.h"
#include "Powerup_Credits.generated.h"

/**
 * 
 */
UCLASS()
class ROGUE_REPO_API APowerup_Credits : public APowerupActor
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(EditAnywhere, Category = "Credits")
	int32 CreditsAmount;

public:

	void Interact_Implementation(APawn* InstigatorPawn) override;

	APowerup_Credits();
};
