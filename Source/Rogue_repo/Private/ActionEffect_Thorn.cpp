// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEffect_Thorn.h"
#include "AttributeComponent.h"
#include "ActionComponent.h"
#include "GameplayFunctionLibrary.h"

UActionEffect_Thorn::UActionEffect_Thorn()
{
	ReflectFraction = 0.2f;
	Duration = 0.0f;
	Period = 0.0f;
}


void UActionEffect_Thorn::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	UAttributeComponent* Attributes = UAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.AddDynamic(this, &UActionEffect_Thorn::OnHealthChanged);
	}
}

void UActionEffect_Thorn::StopAction_Implementation(AActor* InstigatorActor)
{
	Super::StopAction_Implementation(InstigatorActor);
	UAttributeComponent* Attributes = UAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.RemoveDynamic(this, &UActionEffect_Thorn::OnHealthChanged);
	}
}

void UActionEffect_Thorn::OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();

	if (Delta > 0.0f && OwningActor != InstigatorActor)
	{
		int ReflectAmt = FMath::RoundToInt(Delta * ReflectFraction);
		if(ReflectAmt == 0)
			return;

		ReflectAmt = FMath::Abs(ReflectAmt);
		UGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectAmt);
	}
}
