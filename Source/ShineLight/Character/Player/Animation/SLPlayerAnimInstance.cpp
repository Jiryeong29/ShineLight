// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Animation/SLPlayerAnimInstance.h"
#include "Character/Player/SLCharacterPlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

USLPlayerAnimInstance::USLPlayerAnimInstance()
{
	// 이렇게 캐릭터를 가져오면 GetOwningActor() 를 쓰면 굳이 헤더파일을 추가하지 않아도 되는거 같다.
	// Owner = Cast<ASLCharacterPlayer>(TryGetPawnOwner());
	
	// 문턱 값 설정.
	MovingThreshold = 3.0f;
	JumpingThreshold = 100.0f;
}

// 애니메이션이 초기화될 때 호출.
void USLPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Owner = Cast<ASLCharacterPlayer>(GetOwningActor());

	// 캐릭터로 형변환 성공.
	if (Owner)
	{
		// 무브먼트 컴포넌트 검색.
		Movement  = Owner->GetCharacterMovement();
	}
}

	
// 애니메이션 업데이트 함수.
void USLPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Movement)
	{
		Velocity = Movement->Velocity;
	}
	// 필요한 값 업데이트.
	if (Movement)
	{
		// 최대 걷기 뛰기 속도 저장.
		PlayerMaxWalkSpeed = Owner->GetMaxWalkSpeed();
		PlayerMaxRunSpeed = Owner->GetMaxRunSpeed();

		// 현재 속도 저장.
		Velocity = Movement->Velocity;
		CurrentGroundSpeed = Velocity.Size2D(); //z축 제거
		
		// 멈춘 상태를 확인.
		bIsIdle = CurrentGroundSpeed < MovingThreshold;
		
		bIsRun = CurrentGroundSpeed >= PlayerMaxRunSpeed;
		bIsAir = Movement->IsFalling();
		bIsFalling = bIsAir & (Velocity.Z < JumpingThreshold	);
		bIsJumping = bIsAir & (Velocity.Z > JumpingThreshold);

		AimPitch = Owner->GetControlRotation().Pitch;
		AimYaw= Owner->GetControlRotation().Yaw;
	}
	
}
