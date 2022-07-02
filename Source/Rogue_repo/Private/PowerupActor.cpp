// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerupActor.h"
#include "Net/UnrealNetwork.h"
#include "Components/SphereComponent.h"

// Sets default values
APowerupActor::APowerupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);
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

void APowerupActor::SetPowerupState(bool bIsNewActive)
{
	bIsActive = bIsNewActive;
	OnRep_IsActive();
}


void APowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	// Logic is in dervied class.
}

FText APowerupActor::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}

void APowerupActor::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	RootComponent->SetVisibility(bIsActive, true);
}

void APowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APowerupActor, bIsActive);
}
