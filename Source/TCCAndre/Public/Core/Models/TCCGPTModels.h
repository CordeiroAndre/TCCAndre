#pragma once
#include "CoreMinimal.h"
#include "TCCGPTModels.generated.h"


USTRUCT(BlueprintType)
struct FMessageModel
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString role;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString content;
};

USTRUCT(BlueprintType)
struct FRequestModel
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	 FString model = "gpt-3.5-turbo";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMessageModel> messages;
};