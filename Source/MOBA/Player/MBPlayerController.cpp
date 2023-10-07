// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MBPlayerController.h"

void AMBPlayerController::BeginPlay()
{
	Super::BeginPlay();
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
