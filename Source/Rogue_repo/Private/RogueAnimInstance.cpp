// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAnimInstance.h"
#include "GameplayTagContainer.h"
#include "ActionComponent.h"

void URogueAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	AActor* OwningActor = GetOwningActor();
	if (OwningActor)
	{
		ActionComp = Cast<UActionComponent>(OwningActor->GetComponentByClass(UActionComponent::StaticClass()));
	}
}

void URogueAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
	if (ActionComp)
	{
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
