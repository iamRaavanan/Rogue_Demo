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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAttributeComponent* AttributeComp;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> BlackHoleProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> DashProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;

	FTimerHandle TimeHanlde_PrimaryAttack;
	FTimerHandle TimeHanlde_BlackholeAttack;
	FTimerHandle TimeHanlde_Dash;

protected:

	void MoveForward (float value);
	void MoveRight(float value);

	void PrimaryAttack_TimeElapsed();
	void PrimaryAttack();

	void BlackHoleAttack();
	void BlackholeAttack_TimeElapsed();

	void Dash();
	void Dash_TimeElapsed();

	// Re-use spawn logic between attacks
	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

	void PrimaryInteraction();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
