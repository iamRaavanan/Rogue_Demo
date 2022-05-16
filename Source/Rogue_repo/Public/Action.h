// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Action.generated.h"

class UWorld;

UCLASS(Blueprintable )
class ROGUE_REPO_API UAction : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;
	

public:

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction (AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction (AActor* Instigator);

	UWorld* GetWorld () const override;
};
