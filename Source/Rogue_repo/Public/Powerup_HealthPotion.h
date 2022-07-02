// Fill out your copyright notice 0in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerupActor.h"
#include "Powerup_HealthPotion.generated.h"

class UStaticMeshComponent;
/**
 * 
 */
UCLASS()
class ROGUE_REPO_API APowerup_HealthPotion : public APowerupActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "HealthPotion")
	int32 CreditCost;

public:
	APowerup_HealthPotion ();

	void Interact_Implementation(APawn* InstigatorPawn) override;

	FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
};
