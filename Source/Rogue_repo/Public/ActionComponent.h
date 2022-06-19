// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "ActionComponent.generated.h"

class UAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUE_REPO_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActionComponent();

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction (AActor* Instigator, TSubclassOf<UAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	UAction* GetAction(TSubclassOf<UAction> ActionClass) const;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void RemoveAction(UAction* ActionToRemove);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName (AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName (AActor* Instigator, FName ActionName);

	bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

protected:

	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<UAction>> DefaultActions;

	UPROPERTY(Replicated)
	TArray<UAction*> Actions; 

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void ServerStartAction (AActor* Instigator, FName ActionName);
		
};
