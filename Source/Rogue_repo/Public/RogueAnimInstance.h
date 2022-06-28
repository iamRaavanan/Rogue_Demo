// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RogueAnimInstance.generated.h"

class UActionComponent;

UCLASS()
class ROGUE_REPO_API URogueAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bIsStunned;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	UActionComponent* ActionComp;

public:

	void NativeInitializeAnimation () override;

	void NativeUpdateAnimation(float DeltaSeconds) override;
};
