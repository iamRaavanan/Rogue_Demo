// Fill out your copyright notice in the Description page of Project Settings.


#include "TagetDummy.h"
#include "AttributeComponent.h"

// Sets default values
ATargetDummy::ATargetDummy()
{
 	MeshComp = CreateDefaultSubobject< UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;
	AttributeComp = CreateDefaultSubobject<UAttributeComponent>("AttributeComp");
	AttributeComp->OnUpdateHealth.AddDynamic(this, &ATargetDummy::OnHealthChanged);

}

void ATargetDummy::OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwingComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}
