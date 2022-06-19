// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayInterface.h"
#include "PowerupActor.generated.h"

class USphereComponent;

UCLASS()
class ROGUE_REPO_API APowerupActor : public AActor, public IGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerupActor();


protected:
	UPROPERTY(ReplicatedUsing="OnRep_IsActive")
	bool bIsActive;

	UPROPERTY(EditAnywhere, Category = "Powerup")
	float RespawnTime;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	FTimerHandle TimerHanlde_RespawnTimer;

protected:
	
	UFUNCTION()
	void OnRep_IsActive();

	UFUNCTION()
	void ShowPowerup ();

	UFUNCTION()
	void HideAndCooldownPowerup();

	UFUNCTION()
	void SetPowerupState(bool bIsNewActive);
public:
	void Interact_Implementation(APawn* InstigatorPawn) override;

};
