// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionEffect.h"
#include "ActionEffect_Thorn.generated.h"

class UAttributeComponent;

UCLASS()
class ROGUE_REPO_API UActionEffect_Thorn : public UActionEffect
{
	GENERATED_BODY()
public:
	
	UActionEffect_Thorn();
	
	void StartAction_Implementation(AActor* Instigator);

	void StopAction_Implementation(AActor* Instigator);


protected:

	UPROPERTY(EditDefaultsOnly, Category="Thorn")
	float ReflectFraction;

protected:

	UFUNCTION()
	void OnHealthChanged(AActor* Instigator, UAttributeComponent* OwningComp, float NewHealth, float Delta);
};
