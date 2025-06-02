// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SLGameModeBase.h"

ASLGameModeBase::ASLGameModeBase()
{
	
	static ConstructorHelpers::FClassFinder<APawn> PlayerRef = TEXT("/Game/ShineLight/Character/BP_SLCharacterPlayer.BP_SLCharacterPlayer_C");

	if (PlayerRef.Class)
	{
		DefaultPawnClass = PlayerRef.Class; 
	}

	
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerRef = TEXT("/Script/ShineLight.SLPlayerController");
	if (PlayerControllerRef.Class)
	{
		PlayerControllerClass = PlayerControllerRef.Class;
	}
	
}
