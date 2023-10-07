// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MBGameMode.h"
#include "Player/MBPlayerController.h"

AMBGameMode::AMBGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> ClassRef(TEXT("/Script/MOBA.MBCharacterPlayer"));
	if (ClassRef.Class)
	{
		DefaultPawnClass = ClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PClassRef(TEXT("/Script/MOBA.MBPlayerController"));
	if (PClassRef.Class)
	{
		PlayerControllerClass = PClassRef.Class;
	}
}
