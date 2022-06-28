// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/AICharacter.h"
#include "AttributeComponent.h"
#include "EngineUtils.h"
#include "RogueCharacter.h"
#include "RogueState.h"
#include "GameFramework/SaveGame.h"
#include "RogueSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "GameplayInterface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("rr.SpawnBots"), true, TEXT("EnableSpawning of bots via timer"), ECVF_Cheat);

ARogueGameModeBase::ARogueGameModeBase()
{
	SpawnTimerInterval = 2.0f;	
	CreditsPerKill = 20;

	DesiredPowerupCount = 10;
	RequiredPowerupDistance = 2000;

	PlayerStateClass = ARogueState::StaticClass();
	SlotName = "RogueSaveGame01";
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

void ARogueGameModeBase::OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("EQS query failed"));
		return;
	}
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	
	TArray<FVector> UsedLocations;
	int32 SpawnCounter = 0;
	while (SpawnCounter < DesiredPowerupCount && Locations.Num() > 0)
	{
		int32 RandLocationIndex = FMath::RandRange(0, Locations.Num() - 1);
		FVector PickedLocation = Locations[RandLocationIndex];
		Locations.Remove(PickedLocation);
		bool bIsValidLocation = true;
		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();
			if (DistanceTo < RequiredPowerupDistance)
			{
				// Choosen location is too close, skip and look for better location
				bIsValidLocation = false;
				break;
			}
		}
		if(!bIsValidLocation)
			continue;

		int32 RandomClassIndex = FMath::RandRange(0, PowerupClasses.Num() - 1);
		TSubclassOf<AActor> RandomPowerupClass = PowerupClasses[RandomClassIndex];
		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLocation,FRotator::ZeroRotator);

		UsedLocations.Add(PickedLocation);
		SpawnCounter++;
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

	// Credits to player
	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn)
	{
		if (ARogueState* RS = KillerPawn->GetPlayerState<ARogueState>())
		{
			RS->AddCredits(CreditsPerKill);
		}
	}
}

void ARogueGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	LoadSaveGame();
}

void ARogueGameModeBase::StartPlay()
{
	Super::StartPlay();

	//GetWorldTimerManager().SetTimer(TimerHanlde_SpawnBots, this, &ARogueGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);

	if (ensure(PowerupClasses.Num() > 0))
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, PowerupSpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ARogueGameModeBase::OnPowerupSpawnQueryCompleted);
		}
	}
}

void ARogueGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	ARogueState* RogueState = Cast<ARogueState>(NewPlayer->GetPlayerState<ARogueState>());
	if (RogueState)
	{
		RogueState->LoadPlayerState(CurrentSaveGame);
	}
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
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

void ARogueGameModeBase::WriteSaveGame()
{
	for (int i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		ARogueState* RogueState = Cast<ARogueState>(GameState->PlayerArray[i]);
		if (RogueState)
		{
			RogueState->SavePlayerState(CurrentSaveGame);
			break; //Single player option
		}
	}
	CurrentSaveGame->SavedActors.Empty();
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor->Implements<UGameplayInterface>())
		{
			continue;
		}
		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetTransform();

		FMemoryWriter memWriter(ActorData.ByteData);
		FObjectAndNameAsStringProxyArchive Arch(memWriter, true);
		Arch.ArIsSaveGame = true;
		Actor->Serialize(Arch);

		CurrentSaveGame->SavedActors.Add(ActorData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void ARogueGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<URogueSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SavedGame from [%s] slot"), *SlotName);
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("Successfully loaded from [%s] slot"), *SlotName);
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			if (!Actor->Implements<UGameplayInterface>())
			{
				continue;
			}
			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader memReader(ActorData.ByteData);
					FObjectAndNameAsStringProxyArchive Arch(memReader, true);
					Arch.ArIsSaveGame = true;
					Actor->Serialize(Arch);
					
					//IGameplayInterface::Execute_OnActorLoaded(Actor);
					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<URogueSaveGame>(UGameplayStatics::CreateSaveGameObject(URogueSaveGame::StaticClass()));
		UE_LOG(LogTemp, Warning, TEXT("Created the SavedGame"));
	}
}
