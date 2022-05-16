// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RogueCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInteractionComponent;
class UAnimMontage;
class UAttributeComponent;
class UParticleSystem;
class UActionComponent;

UCLASS()
class ROGUE_REPO_API ARogueCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARogueCharacter();

protected:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UActionComponent* ActionComp;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

protected:

	void MoveForward (float value);
	void MoveRight(float value);

	void SprintStart ();
	void SprintStop ();

	void PrimaryAttack();
	void BlackHoleAttack();
	void Dash();

	void PrimaryInteraction();

	UFUNCTION()
	void OnHealthChanged (AActor* InstigatorActor, UAttributeComponent* OwingComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;


public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UFUNCTION(Exec)
	void HealSelf (float Amount = 100);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;
};
