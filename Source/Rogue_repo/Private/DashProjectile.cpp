// Fill out your copyright notice in the Description page of Project Settings.


#include "DashProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ADashProjectile::ADashProjectile()
{
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;
	MoveComp->InitialSpeed = 6000.0f;
}

void ADashProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer (TimerHanlde_DelayedDetonate, this, &ADashProjectile::Explode, DetonateDelay);
}

void ADashProjectile::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHanlde_DelayedDetonate);
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
	EffectComp->DeactivateSystem();
	MoveComp->StopMovementImmediately();
	SetActorEnableCollision(false);
	
	FTimerHandle TimerHanlde_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHanlde_DelayedTeleport, this, &ADashProjectile::TeleportInstigator, TeleportDelay);
}

void ADashProjectile::TeleportInstigator()
{
	AActor* ActorTeleport = GetInstigator();
	if (ensure(ActorTeleport))
	{
		ActorTeleport->TeleportTo(GetActorLocation(), GetActorRotation(), false, false);
	}
}
