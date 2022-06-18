// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "Action.generated.h"

class UWorld;

UCLASS(Blueprintable )
class ROGUE_REPO_API UAction : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;
public:

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning () const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction (AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction (AActor* Instigator);

	UWorld* GetWorld () const override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	bool bIsRunning;

protected:

	UFUNCTION(BlueprintCallable, Category ="Action")
	UActionComponent* GetOwningComponent () const;
};
