// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Engine/DataTable.h"
#include "RogueGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class URogueSaveGame;
class UDataTable;
class UMonsterData;

USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase 
{
	GENERATED_BODY()

public:
	
	FMonsterInfoRow()
	{
		Weight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 20.0f;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId MonsterId;
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMonsterData* MonsterData;*/
	//TSubclassOf<AActor> MonsterClass;

	// Relative chance to pick particular monster
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;

	// Points required for game mode to spawn this unit
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;

	// Amount of credits awarded when user kill this monster
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward;
};

UCLASS()
class ROGUE_REPO_API ARogueGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	FString SlotName;

	UPROPERTY()
	URogueSaveGame* CurrentSaveGame;

	FTimerHandle TimerHanlde_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UDataTable* MonsterTable;

	/*UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;*/

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	int32 CreditsPerKill;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	UEnvQuery* PowerupSpawnQuery;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	TArray<TSubclassOf<AActor>> PowerupClasses;

	/* Distance required between power-up spawn locations */
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	float RequiredPowerupDistance;

	/* Amount of power ups to spawn during match start */
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	int32 DesiredPowerupCount;

	UFUNCTION()
	void SpawnBotTimerElapsed ();

	UFUNCTION()
	void OnQueryCompleted (UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnMosterLoaded (FPrimaryAssetId LoadedId, FVector LoadLocation);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

	UFUNCTION()
	void OnPowerupSpawnQueryCompleted (UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

public:
	ARogueGameModeBase();

	virtual void OnActorKilled (AActor* Victim, AActor* Killer);

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void StartPlay() override;

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	UFUNCTION(Exec)
	void KillAll ();

	UFUNCTION(BlueprintCallable)
	void WriteSaveGame ();

	UFUNCTION()
	void LoadSaveGame ();
};
