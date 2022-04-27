// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "MagicProjectile.generated.h"

UCLASS()
class ROGUE_REPO_API AMagicProjectile : public AProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagicProjectile();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
