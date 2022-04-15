// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RogueCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

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

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClass;

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward (float value);
	void MoveRight(float value);
	void PrimaryAttack();
	void JumpPressed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
