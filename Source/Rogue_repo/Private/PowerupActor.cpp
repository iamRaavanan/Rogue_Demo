// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerupActor.h"
#include "Components/SphereComponent.h"

// Sets default values
APowerupActor::APowerupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");
	RootComponent = SphereComp;

	RespawnTime = 10.0f;

	SetReplicates(true);
}

void APowerupActor::ShowPowerup()
{
	SetPowerupState(true);
}

void APowerupActor::HideAndCooldownPowerup()
{	
	SetPowerupState(false);
	GetWorldTimerManager().SetTimer(TimerHanlde_RespawnTimer, this, &APowerupActor::ShowPowerup, RespawnTime);
}

void APowerupActor::SetPowerupState(bool bIsActive)
{
	SetActorEnableCollision(bIsActive);
	RootComponent->SetVisibility(bIsActive, true);
}

void APowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	// Logic is in dervied class.
}


