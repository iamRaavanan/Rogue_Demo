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

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;
	
public:
	APowerup_HealthPotion ();

	void Interact_Implementation(APawn* InstigatorPawn) override;

};
