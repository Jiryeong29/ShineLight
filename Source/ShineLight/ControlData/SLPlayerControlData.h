// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SLPlayerControlData.generated.h"

/**
 * 
 */
UCLASS()
class SHINELIGHT_API USLPlayerControlData : public UPrimaryDataAsset
{
	GENERATED_BODY()

	public:

	// 입력 매핑 컨텍스트 설정
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	// [ 회전 제어 설정 변수들 ]
	// 캐릭터가 컨트롤러의 Yaw 회전값(좌우 방향)을 그대로 따라갈지 여부
	UPROPERTY(EditAnywhere, Category = Pawn)
	uint8 bUseControllerRotationYaw : 1;

	// 캐릭터가 이동하는 방향으로 자동으로 몸을 회전 여부 설정 변수
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint8 bOrientRotationToMovement : 1;

	// 컨트롤러의 회전 방향을 목표 회전(Desired Rotation)으로 사용할지 여부
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint8 bUseControllerDesiredRotation : 1;
	
};
