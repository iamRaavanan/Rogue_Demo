// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorldUserWidget.generated.h"

class USizeBox;
/**
 * 
 */
UCLASS()
class ROGUE_REPO_API UWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	AActor* AttachedActor;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FVector worldOffset;

	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;


protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
