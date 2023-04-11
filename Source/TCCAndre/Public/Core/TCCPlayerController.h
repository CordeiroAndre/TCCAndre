// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpeechRecognizer.h"
#include "GameFramework/PlayerController.h"
#include "Sound/CapturableSoundWave.h"
#include "TCCPlayerController.generated.h"


UENUM(BlueprintType)
enum class EUserStatus : uint8
{
	INITIAL,
	CAPTURING,
	TRANSFORMING_SPEECH_TO_TEXT,
	CONFIRMING,
	FINISHED,
	GENERATING_ANSWER
};

UCLASS(Abstract)
class TCCANDRE_API ATCCPlayerController : public APlayerController
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatusChange, const EUserStatus&, NewStatus );
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRecognitionFinished, const FString&, Message);

	/* Maps cannot be bound to delegates or somethign idc, just use the 'chat' getter */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNewChatEntry);


public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TCC | GPT3 ")
	FString OpenAIAPIKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TCC | GPT3 ")
	FString SystemContext;

	
	UFUNCTION(BlueprintCallable, Category="TCC | Speech Recognition")
	void StartCapturingMic();

	UFUNCTION(BlueprintCallable, Category="TCC | Speech Recognition")
	void StopCapturingMic();
	
	UPROPERTY(BlueprintAssignable, Category="TCC | Speech Recognition")
	FOnStatusChange OnStatusChange;

	UPROPERTY(BlueprintAssignable, Category="TCC | Speech Recognition")
	FOnRecognitionFinished OnRecognitionFinished;

	UPROPERTY(BlueprintAssignable, Category="TCC | ChatGPT")
	FOnNewChatEntry OnNewChatEntry;
	
	/* As the HandleRecognizedTextSegment might be triggered multiple times before
	 * the HandleRecognitionFinished the current text must be a separated field. 
	 */
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="TCC | Speech Recognition")
	FString CurrentRecognizedText;

	UFUNCTION()
	void HandleReceivedResponseFromGPT3(const FString& Response);
	
	UFUNCTION(BlueprintCallable, Category="TCC | Speech Recognition")
	void ConfirmSpeechRecognition(const bool& Accepted);

	UFUNCTION(BlueprintCallable, Category="TCC | Speech Recognition")
	bool CanCaptureMic();

	/*Map containing all the messages of the system */
	TArray<TTuple<FString, FString>> Chat;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FString> GetChatKeys()
	{
		TArray<FString> Keys;
		for (TTuple<FString, FString> Tuple : Chat){
		Keys.Add(Tuple.Key);
		} 
		return Keys;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FString> GetChatValues()
	{
		TArray<FString> Values;
		for (TTuple<FString, FString> Tuple : Chat){
			Values.Add(Tuple.Value);
		} 
		return Values;
	}

	UFUNCTION(BlueprintCallable)
	void SetSystemContext(const FString& Context)
	{
		AddChatEntry("system", Context);
	}

	
protected:
	
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadOnly, Category="TCC | Speech Recognition")
	EUserStatus CurrentStatus = EUserStatus::INITIAL;

	/* Object that will handle the Speech to text transformation */
	UPROPERTY()
	USpeechRecognizer* SpeechRecognizer;

	/* Object that will handle the mic capture */
	UPROPERTY()
	UCapturableSoundWave* CapturableSoundWave;

	/* Delegate Handlers */

	UFUNCTION()
	void HandleRecognitionFinished();

	UFUNCTION()
	void HandleRecognizedTextSegment(const FString& RecognizedWords);

	UFUNCTION()
	void HandlePopulatedAudioData(const TArray<float>& PopulatedAudioData);

	void ChangeStatus(const EUserStatus& NewStatus)
	{
		CurrentStatus = NewStatus;
		OnStatusChange.Broadcast(CurrentStatus);
	}

	void AddChatEntry(const FString& GPTRole, const FString Content)
	{
		Chat.Add(TTuple<FString, FString>(GPTRole, Content));
		OnNewChatEntry.Broadcast();
	}
};
