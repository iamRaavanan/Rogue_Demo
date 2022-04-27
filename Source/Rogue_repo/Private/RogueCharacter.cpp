// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InteractionComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AttributeComponent.h"

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
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	AttackAnimDelay = 0.2f;
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

void ARogueCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(ProjectileClass);
}

void ARogueCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimeHanlde_PrimaryAttack, this, &ARogueCharacter::PrimaryAttack_TimeElapsed, AttackAnimDelay);
}

void ARogueCharacter::BlackHoleAttack()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimeHanlde_BlackholeAttack, this, &ARogueCharacter::BlackholeAttack_TimeElapsed, AttackAnimDelay);
}

void ARogueCharacter::BlackholeAttack_TimeElapsed()
{
	SpawnProjectile(BlackHoleProjectileClass);
}

void ARogueCharacter::Dash()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimeHanlde_Dash, this, &ARogueCharacter::Dash_TimeElapsed, AttackAnimDelay);
}

void ARogueCharacter::Dash_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass);
}

void ARogueCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensure(ProjectileClass))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		FHitResult Hit;
		FVector TraceStart = CameraComp->GetComponentLocation();
		FVector TraceEnd = TraceStart + (GetControlRotation().Vector() * 5000);

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);
		
		FRotator ProjRotation;

		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, TraceParams))
		{
			ProjRotation = FRotationMatrix::MakeFromX(Hit.ImpactPoint - HandLocation).Rotator();
		}
		else
		{
			ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
		}
		FTransform SpawnT = FTransform(ProjRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnT, SpawnParams);
	}
}

void ARogueCharacter::PrimaryInteraction()
{
	if(InteractionComp)
		InteractionComp->PrimaryInteraction();
}

// Called every frame
void ARogueCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation ();
	LineStart += GetActorRightVector() * 100;
	FVector ActorDirectionLineEnd = LineStart + (GetActorForwardVector() * 100.0f);

	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirectionLineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirectionLineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirectionLineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
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
}

