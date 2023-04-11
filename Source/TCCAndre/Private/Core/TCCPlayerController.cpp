// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/TCCPlayerController.h"

#include "SpeechRecognizer.h"
#include "JsonObjectConverter.h"
#include "Core/TCCGPT3Request.h"
#include "Core/Models/TCCGPTModels.h"
#include "Sound/CapturableSoundWave.h"


void ATCCPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ATCCPlayerController::StartCapturingMic()
{
	if(CanCaptureMic())
	{
		CurrentRecognizedText.Empty();
		ChangeStatus(EUserStatus::CAPTURING);

		/* Create the object that will convert the speech to text and bind its delegates */
		SpeechRecognizer = USpeechRecognizer::CreateSpeechRecognizer();
		SpeechRecognizer->OnRecognitionFinished.AddDynamic(this, &ATCCPlayerController::HandleRecognitionFinished);
		SpeechRecognizer->OnRecognizedTextSegment.AddDynamic(this, &ATCCPlayerController::HandleRecognizedTextSegment);

		/* Configure the speech recognition */
		SpeechRecognizer->SetStreamingDefaults();
		SpeechRecognizer->SetLanguage(ESpeechRecognizerLanguage::Pt);

		/* Enable the speech recognizer to receive audio */
		SpeechRecognizer->StartSpeechRecognition();

		/* Create the object that will capture our mic */
		CapturableSoundWave =  UCapturableSoundWave::CreateCapturableSoundWave();
		CapturableSoundWave->OnPopulateAudioData.AddDynamic(this, &ATCCPlayerController::HandlePopulatedAudioData);

		/*Start capturing the */
		CapturableSoundWave->StartCapture(0);
	}
}

void ATCCPlayerController::StopCapturingMic()
{
	if(CurrentStatus==EUserStatus::CAPTURING)
	{
		ChangeStatus(EUserStatus::TRANSFORMING_SPEECH_TO_TEXT);
		SpeechRecognizer->ForceProcessPendingAudioData();
		CapturableSoundWave->StopCapture();
	}
}

void ATCCPlayerController::HandleRecognitionFinished()
{
	if(CapturableSoundWave->IsCapturing())
	{
		SpeechRecognizer->StopSpeechRecognition();
		SpeechRecognizer = nullptr;
	}
}

void ATCCPlayerController::HandleRecognizedTextSegment(const FString& RecognizedWords)
{
	CurrentRecognizedText += RecognizedWords;
	ChangeStatus(EUserStatus::CONFIRMING);
}

void ATCCPlayerController::HandlePopulatedAudioData(const TArray<float>& PopulatedAudioData)
{
	SpeechRecognizer->ProcessAudioData(PopulatedAudioData, CapturableSoundWave->GetSampleRate(), CapturableSoundWave->GetNumOfChannels(), false);
}

void ATCCPlayerController::ConfirmSpeechRecognition(const bool& Accepted)
{
	if(Accepted)
	{
		FRequestModel RequestModel;
		TArray<FMessageModel> AuxMessageContainer;

		AddChatEntry("user",CurrentRecognizedText);
		
		for (TTuple<FString, FString> Message : Chat)
		{
			FMessageModel RequestMessages;
			RequestMessages.role = Message.Key;
			RequestMessages.content = Message.Value;
			AuxMessageContainer.Add(RequestMessages);
		}
		RequestModel.messages = AuxMessageContainer;
		FString JSonString;
		FJsonObjectConverter::UStructToJsonObjectString(FRequestModel::StaticStruct(),&RequestModel,JSonString);

		
		UTCCGPT3Request::PostMessageToChatGPT3(OpenAIAPIKey, JSonString)->OnResponse.AddDynamic(this, &ATCCPlayerController::HandleReceivedResponseFromGPT3);
		
		
		OnRecognitionFinished.Broadcast(CurrentRecognizedText);
		ChangeStatus(EUserStatus::GENERATING_ANSWER);
	}
	else
	{
		ChangeStatus(EUserStatus::FINISHED);
	}
}

bool ATCCPlayerController::CanCaptureMic()
{
	if(CurrentStatus==EUserStatus::TRANSFORMING_SPEECH_TO_TEXT) return false;
	if(CurrentStatus==EUserStatus::CONFIRMING) return false;
	if(CurrentStatus==EUserStatus::GENERATING_ANSWER) return false;
	return true;
}

void ATCCPlayerController::HandleReceivedResponseFromGPT3(const FString& Response)
{
	if(!Response.IsEmpty())
	{
		AddChatEntry("assistant", Response);
	}
	ChangeStatus(EUserStatus::FINISHED);
}
