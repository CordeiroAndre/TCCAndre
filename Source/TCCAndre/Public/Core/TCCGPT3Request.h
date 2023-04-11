// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TCCGPT3Request.generated.h"

/**
 * 
 */
UCLASS()
class TCCANDRE_API UTCCGPT3Request : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGotResponse, const FString&, AssistantResponse);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGotResponseFail);
		
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true", WorldContext="WorldContextObject"), Category="TCC | GPT3 | Requests")
	static UTCCGPT3Request* PostMessageToChatGPT3(const FString& APIKey, const FString& JSonContentAsString);

	UPROPERTY(BlueprintAssignable, Category="TCC | GPT3 | Delegates")
	FOnGotResponse OnResponse;

	UPROPERTY(BlueprintAssignable, Category="TCC | GPT3 | Delegates")
	FOnGotResponseFail OnError;
};
