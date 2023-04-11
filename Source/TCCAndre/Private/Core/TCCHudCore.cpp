// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/TCCHudCore.h"

ATCCHudCore::ATCCHudCore()
{

}

void ATCCHudCore::BeginPlay()
{
	Super::BeginPlay();
	_baseUIInstance = CreateWidget<UTCCBaseUI>(GetOwningPlayerController(), BaseUI, TEXT("BaseUI"));
	_baseUIInstance->AddToPlayerScreen();
}

void ATCCHudCore::PushBase( TSubclassOf<UCommonActivatableWidget> CommonActivatableWidget)
{
	if(_baseUIInstance)
	{
		_baseUIInstance->PushBase(CommonActivatableWidget);
	}
}

void ATCCHudCore::PushPrompt( TSubclassOf<UCommonActivatableWidget> CommonActivatableWidget)
{
	if(_baseUIInstance)
	{
		_baseUIInstance->PushPrompt(CommonActivatableWidget);
	}
}

void ATCCHudCore::RemoveBase()
{
	if(_baseUIInstance)
	{
		_baseUIInstance->RemoveBase();
	}
}

void ATCCHudCore::RemovePrompt( )
{
	if(_baseUIInstance)
	{
		_baseUIInstance->RemovePrompt();
	}
}
