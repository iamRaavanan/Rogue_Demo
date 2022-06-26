// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"
#include "RogueGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("rr.DamageMultiplier"), 1.0f, TEXT("Global damage modifier for Attribute component"), ECVF_Cheat);

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;

	Rage = 0;
	RageMax = 100;

	SetIsReplicatedByDefault(true);
}


bool UAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthValue(InstigatorActor, -GetHealthMax());
}

bool UAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UAttributeComponent::ApplyHealthValue(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)	// For GOD Mode
	{
		return false;
	}
	if (Delta < 0)
	{
		float damageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= damageMultiplier;
	}
	float oldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	float ActualDelta = NewHealth - oldHealth;
	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;
		if (ActualDelta != 0)
		{
			MulticastHelathChanged(InstigatorActor, Health, ActualDelta);
		}
		if (ActualDelta < 0.0f && Health == 0.0f)
		{
			ARogueGameModeBase* RGM = GetWorld()->GetAuthGameMode<ARogueGameModeBase>();
			if (RGM)
			{
				RGM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}
	//Died
	return ActualDelta != 0;
}

void UAttributeComponent::MulticastHelathChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

bool UAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

float UAttributeComponent::GetHealth() const
{
	return Health;
}

float UAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

float UAttributeComponent::GetRage() const
{
	return Rage;
}

bool UAttributeComponent::ApplyRage(AActor* InstigatorActor, float Delta)
{
	float OldRage = Rage;
	Rage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);
	float ActualDelta = Rage - OldRage;
	if (ActualDelta != 0)
	{
		OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta);
	}
	return ActualDelta != 0;
}

UAttributeComponent* UAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UAttributeComponent>(FromActor->GetComponentByClass(UAttributeComponent::StaticClass()));
	}
	return nullptr;
}
bool UAttributeComponent::IsActorAlive(AActor* CurrentActor)
{
	UAttributeComponent* AttributeComp = GetAttributes(CurrentActor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
}

void UAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UAttributeComponent, Health);
	DOREPLIFETIME(UAttributeComponent, HealthMax);
}