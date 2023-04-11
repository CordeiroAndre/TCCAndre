#include "Core/TCCPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Core/TCCPlayerController.h"

void ATCCPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Cast<APlayerController>(GetController())->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(GameplayContext,0);
	}

	if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		auto* TCCController = Cast<ATCCPlayerController>(GetController());
		if(TCCController)
		{
			PlayerEnhancedInputComponent->BindAction(SpeechAction, ETriggerEvent::Started, TCCController, &ATCCPlayerController::StartCapturingMic);
			PlayerEnhancedInputComponent->BindAction(SpeechAction, ETriggerEvent::Completed, TCCController, &ATCCPlayerController::StopCapturingMic);
		}
	}
}
