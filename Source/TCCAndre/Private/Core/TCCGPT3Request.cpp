// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/TCCGPT3Request.h"
#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"



UTCCGPT3Request* UTCCGPT3Request::PostMessageToChatGPT3(const FString& APIKey, const FString& JSonContentAsString)
{
	UTCCGPT3Request* Request = NewObject<UTCCGPT3Request>();
	
	TSharedRef<IHttpRequest> httpRequest = FHttpModule::Get().CreateRequest();
	
	FString Authorization = "Bearer ";
	Authorization.Append(APIKey);
	httpRequest->SetURL("https://api.openai.com/v1/chat/completions");
	httpRequest->SetVerb(TEXT("POST"));
	httpRequest->SetHeader("Content-Type", "application/json");
	httpRequest->SetHeader("Authorization", Authorization);
	httpRequest->SetContentAsString(JSonContentAsString);
	
	httpRequest->OnProcessRequestComplete().BindLambda([Request](FHttpRequestPtr, FHttpResponsePtr response, bool bSuccessful)
	{
		if (bSuccessful)
		{
			TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
			TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(response->GetContentAsString());
			if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
			{
				TArray<TSharedPtr<FJsonValue>> choices = JsonObject->GetArrayField("choices");
				
				   if (choices.Num() > 0)
				   {
					   TSharedPtr<FJsonObject> messageObj = choices[0]->AsObject()->GetObjectField("message");
				
					   if (messageObj.IsValid())
					   {
					   	FString content = messageObj->GetStringField("content");
					   	Request->OnResponse.Broadcast(content);
					   }
				   }
			}
		}
		else
		{
			Request->OnError.Broadcast();
			UE_LOG(LogTemp, Error, TEXT("CURL request failed: %s"), *response->GetContentAsString());
		}
	});
	
	httpRequest->ProcessRequest();
	return Request;
}