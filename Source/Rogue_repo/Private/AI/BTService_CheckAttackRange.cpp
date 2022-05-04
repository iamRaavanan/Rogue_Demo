// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void UBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if (TargetActor)
		{
			AAIController* NewAIController = OwnerComp.GetAIOwner();
			if (ensure(NewAIController))
			{
				APawn* AIPawn = NewAIController->GetPawn();
				if (ensure(AIPawn))
				{
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
					bool bWithinRange = DistanceTo < ReachableDistance;
					bool bHasLOS = false;
					if (bWithinRange)
					{
						bHasLOS = NewAIController->LineOfSightTo(TargetActor);
					}
					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWithinRange && bHasLOS);
				}
			}
		}
	}
}
