// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InteractionComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ActionComponent.h"

// Sets default values
ARogueCharacter::ARogueCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UInteractionComponent>("InteractionComp");
	AttributeComp = CreateDefaultSubobject<UAttributeComponent>("AttributeComp");
	ActionComp = CreateDefaultSubobject<UActionComponent>("ActionComp");
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	TimeToHitParamName = "TimeToHit";
}

void ARogueCharacter::MoveForward(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = ControlRot.Roll = 0.0f;
	AddMovementInput(ControlRot.Vector(), value);
}

void ARogueCharacter::MoveRight(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = ControlRot.Roll = 0.0f;
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, value);
}

void ARogueCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void ARogueCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void ARogueCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void ARogueCharacter::BlackHoleAttack()
{
	ActionComp->StartActionByName(this, "Blackhole");
}

void ARogueCharacter::Dash()
{
	ActionComp->StartActionByName(this, "Dash");
}

void ARogueCharacter::PrimaryInteraction()
{
	if(InteractionComp)
		InteractionComp->PrimaryInteraction();
}

void ARogueCharacter::OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwingComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		//GetMesh()->execSetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
	}
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
		SetLifeSpan(5.0f);
	}
}

void ARogueCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this, &ARogueCharacter::OnHealthChanged);
}

FVector ARogueCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

// Called every frame
//void ARogueCharacter::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	const float DrawScale = 100.0f;
//	const float Thickness = 5.0f;
//
//	FVector LineStart = GetActorLocation ();
//	LineStart += GetActorRightVector() * 100;
//	FVector ActorDirectionLineEnd = LineStart + (GetActorForwardVector() * 100.0f);
//
//	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirectionLineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);
//
//	FVector ControllerDirectionLineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
//	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirectionLineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
//}

void ARogueCharacter::HealSelf(float Amount /*= 100*/)
{
	AttributeComp->ApplyHealthValue(this, Amount);
}

// Called to bind functionality to input
void ARogueCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARogueCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARogueCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ARogueCharacter::PrimaryAttack);
	// Used generic name 'SecondaryAttack' for binding 
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ARogueCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ARogueCharacter::Dash);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ARogueCharacter::PrimaryInteraction);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ARogueCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ARogueCharacter::SprintStop);
}

