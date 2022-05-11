// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/AICharacter.h"
#include "AttributeComponent.h"
#include "EngineUtils.h"
#include "RogueCharacter.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("rr.SpawnBots"), true, TEXT("EnableSpawning of bots via timer"), ECVF_Cheat);

ARogueGameModeBase::ARogueGameModeBase()
{
	SpawnTimerInterval = 2.0f;	
}

void ARogueGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via CVarSpawnBots"));
		return;
	}
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (QueryInstance)
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ARogueGameModeBase::OnQueryCompleted);
	}
}

void ARogueGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		return;
	}

	int32 NoOfLiveBots  = 0;
	for (TActorIterator<AAICharacter> It(GetWorld()); It; ++It)
	{
		AAICharacter* Bot = *It;
		UAttributeComponent* AttributeComp = UAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NoOfLiveBots++;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Found %d bots alive."), NoOfLiveBots);
	int MaxBotCount = 10;
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	if (NoOfLiveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("At maximum bots capacity, skipping bot spawn."));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.Num() > 0)	// Locations.IsValidIndex(0)
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}

void ARogueGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

void ARogueGameModeBase::OnActorKilled(AActor* Victim, AActor* Killer)
{
	ARogueCharacter* Player = Cast<ARogueCharacter>(Victim);
	if (Player)
	{
		FTimerHandle TimerHanlde_RespawnDelay;
		
		FTimerDelegate RespawnDelegate;
		RespawnDelegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHanlde_RespawnDelay, RespawnDelegate,RespawnDelay, false);
	}
}

void ARogueGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHanlde_SpawnBots, this, &ARogueGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ARogueGameModeBase::KillAll()
{
	for (TActorIterator<AAICharacter> It(GetWorld()); It; ++It)
	{
		AAICharacter* Bot = *It;
		UAttributeComponent* AttributeComp = UAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this);	// @fixme: pass in player for kill credits
		}
	}
}