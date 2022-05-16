// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ROGUE_REPO_API UGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static bool ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmt);

	static bool ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmt, const FHitResult& HitResult);
};