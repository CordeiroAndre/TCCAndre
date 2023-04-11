// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Widgets/TCCBaseUI.h"
#include "TCCHudCore.generated.h"

UCLASS(Abstract)
class TCCANDRE_API ATCCHudCore : public AHUD
{
	GENERATED_BODY()
public:
	ATCCHudCore();
	
protected:
	virtual void BeginPlay() override;

	
public:
	
	/* UI that will be created to manage and update the User interface*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TCC")
	TSubclassOf<UTCCBaseUI> BaseUI;

	UFUNCTION(BlueprintCallable, Category="TCC")
	void PushBase(const TSubclassOf<UCommonActivatableWidget> CommonActivatableWidget);

	UFUNCTION(BlueprintCallable, Category="TCC")
	void PushPrompt(const TSubclassOf<UCommonActivatableWidget> CommonActivatableWidget);

	UFUNCTION(BlueprintCallable, Category="TCC")
	void RemoveBase();

	UFUNCTION(BlueprintCallable, Category="TCC")
	void RemovePrompt();

private:
	UPROPERTY()
	UTCCBaseUI* _baseUIInstance;
};
