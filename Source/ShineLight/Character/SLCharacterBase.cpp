// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SLCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
ASLCharacterBase::ASLCharacterBase()
{

	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.0f, 0.0f, -88.0f),
		FRotator(0.0f, -90.0f, 0.0f)
	);

}