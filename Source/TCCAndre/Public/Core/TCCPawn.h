// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/Pawn.h"
#include "TCCPawn.generated.h"

UCLASS(Abstract)
class TCCANDRE_API ATCCPawn : public APawn
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TCC | Input | Mapping Context")
	TObjectPtr<UInputMappingContext> GameplayContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TCC | Input | Action")
	TObjectPtr<UInputAction> SpeechAction;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

};
