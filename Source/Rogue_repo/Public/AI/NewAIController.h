// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NewAIController.generated.h"

/**
 * 
 */
UCLASS()
class ROGUE_REPO_API ANewAIController : public AAIController
{
	GENERATED_BODY()
	

protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviourTree;

protected:
	virtual void BeginPlay() override;
};
