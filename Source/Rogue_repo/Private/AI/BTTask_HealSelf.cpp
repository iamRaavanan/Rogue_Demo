// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_HealSelf.h"
#include "AttributeComponent.h"
#include "AIController.h"

EBTNodeResult::Type UBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Mypawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (Mypawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	UAttributeComponent* AttributeComp = UAttributeComponent::GetAttributes(Mypawn);
	if (ensure(AttributeComp))
	{
		AttributeComp->ApplyHealthValue(Mypawn, AttributeComp->GetHealthMax());
	}
	return EBTNodeResult::Succeeded;
}
