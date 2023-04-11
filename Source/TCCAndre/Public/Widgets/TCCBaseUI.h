// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "CommonActivatableWidget.h"
#include "TCCBaseUI.generated.h"

/**
 * Base Class to Use use widgets 
 */
UCLASS(Abstract)
class TCCANDRE_API UTCCBaseUI : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category="TCC")
	void PushBase( TSubclassOf<UCommonActivatableWidget> CommonActivatableWidget);

	UFUNCTION(BlueprintImplementableEvent, Category="TCC")
	void PushPrompt( TSubclassOf<UCommonActivatableWidget> CommonActivatableWidget);

	UFUNCTION(BlueprintImplementableEvent, Category="TCC")
	void RemoveBase();

	UFUNCTION(BlueprintImplementableEvent, Category="TCC")
	void RemovePrompt();
};
