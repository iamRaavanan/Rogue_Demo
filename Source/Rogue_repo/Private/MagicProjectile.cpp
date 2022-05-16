// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicProjectile.h"
#include "AttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameplayFunctionLibrary.h"
#include "ActionComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AMagicProjectile::AMagicProjectile()
{
	SphereComp->SetSphereRadius(20.0f);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AMagicProjectile::OnActorOverlap);
	DamageAmount = 20.0f;
}

// Called when the game starts or when spawned
void AMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		//UAttributeComponent* AttributeComp = Cast<UAttributeComponent>(OtherActor->GetComponentByClass(UAttributeComponent::StaticClass()));
		//if (AttributeComp)
		//{
		//	AttributeComp->ApplyHealthValue(GetInstigator(), -DamageAmount);	//reduce the damage amount
		//	// Only explode when we hit something
		//	Explode();
		//}
		//static FGameplayTag Tag = FGameplayTag::RequestGameplayTag("Status.Parrying");
		UActionComponent* ActionComp = Cast<UActionComponent>(OtherActor->GetComponentByClass(UActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MoveComp->Velocity = -MoveComp->Velocity;
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}
		if (UGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			Explode();
		}
	}
}
