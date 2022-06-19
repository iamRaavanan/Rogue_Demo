// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

class UPawnSensingComponent;
class UAttributeComponent;
class UUserWidget;
class UWorldUserWidget;
class UActionComponent;

UCLASS()
class ROGUE_REPO_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

protected:
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnPawnSeen (APawn* Pawn);
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPawnSeen();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwingComp, float NewHealth, float Delta);

	UFUNCTION()
	void SetTargetActor (AActor* NewTarget);

	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* GetTargetActor() const;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SpottedWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UActionComponent* ActionComp;

	UPROPERTY()
	UWorldUserWidget* ActiveHealthBar;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TargetActorKey;
};
