// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RogueState.generated.h"

class URogueSaveGame;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditChanged, ARogueState*, RogueState, int32, NewCredits, int32, Delta);

UCLASS()
class ROGUE_REPO_API ARogueState : public APlayerState
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing="OnRep_Credits", Category = "Credits")
	int32 Credits;

	UFUNCTION()
	void OnRep_Credits(int32 OldCredits);

public:

	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCredits () const;

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredits(int32 Delta);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditChanged OnCreditChanged;

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(URogueSaveGame* SaveObj);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(URogueSaveGame* SaveObj);
};
