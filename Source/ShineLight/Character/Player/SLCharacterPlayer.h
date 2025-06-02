// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SLCharacterBase.h"
#include "InputActionValue.h"
#include "SLCharacterPlayer.generated.h"

/**
 * 
 */
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
struct FTimerHandle;

UCLASS()
class SHINELIGHT_API ASLCharacterPlayer : public ASLCharacterBase
{
	GENERATED_BODY()
public:
	ASLCharacterPlayer();

	void BeginPlay() override;
	void Tick(float DeltaTime) override;

protected:
	// 입력 매핑 컨텍스트(Input Mapping Context) 변수.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	
	// IA_Move 입력 액션 변수.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	// IA_Look 입력 액션 변수.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	// IA_Jump 입력 액션 변수 설정.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	
	// IA_Sprint 입력 액션 변수 설정.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	// IA_Roll 입력 액션 변수 설정.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RollAction;
	
	// IA_Aim 입력 액션 변수 설정.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimAction;

	// IA_Fire 입력 액션 변수 설정.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Animation)
	TObjectPtr<class UAnimMontage> RollMontage;

	
public:
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
protected:

	// 이동.
	void Move(const FInputActionValue& Value);
	// 뷰.
	void Look(const FInputActionValue& Value);
	// 뛰기.
	void StartSprinting(const FInputActionValue& Value);
	void StopSprinting(const FInputActionValue& Value);
	// 점프.
	void Jumping(const FInputActionValue& Value);
	// 구르기.
	void Roll(const FInputActionValue& Value);
	void RollStop(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	// 에임
	void Aim(const FInputActionValue& Value);
	// 공격 
	void Fire(const FInputActionValue& Value);
	
	// 콤보 타이머 설정 함수.
	void SetComboCheckTimer();



	void SpawnWeapon();
public:
	FORCEINLINE float GetMaxWalkSpeed() const { return WalkSpeed; }
	FORCEINLINE float GetMaxRunSpeed() const{ return RunSpeed; }

	FORCEINLINE uint8 GetIsAming() const { return bIsAiming; }
private:
	// 걷기 속도.
	float WalkSpeed;
	// 뛰기 속도.
	float RunSpeed;

	// 뛰는 중인지 확인하는 변수.
	uint8 bIsRunning: 1;
	// 구르는 중인지 확인하는 변수.
	uint8 bPlayerIsRoll : 1 = false;;

	uint8 bIsFire : 1;
	
	// Aming
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	uint8 bIsAiming;
	
	float CameraDefaultFOV;
	float CameraCurrentFOV;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aming, meta = (AllowPrivateAccess = "true"))
	float CameraAmingFOV;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aming, meta = (AllowPrivateAccess = "true"))
	float CameraAmingSpeed;

	void SetAimingFOV(uint8 bpIsAiming);
	void UpdateFOV();


	FTimerHandle AimingTimerHandle;

	// 목표 FOV 변수.
	float TargetFOV;
	
	// 마우스 감도.
	float NormalTurnRate;
	float NormalLookUp;

	float MouseXSensitivity;
	float MouseYSensitivity;

	// 에임 감도.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aming, meta = (AllowPrivateAccess = "true"))
	float AimTurnRate;
	float AimLookUp;
	
	// 에임시 소켓 오프셋 설정 변수.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aming, meta = (AllowPrivateAccess = "true"))
	FVector AimSocketOffsetVecotr = FVector(0.0f, 65.0f, 35.0f);
	// 디폴트 소켓 오프셋 설정 변수.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aming, meta = (AllowPrivateAccess = "true"))
	FVector DefaultCameraSocketOffsetVecotr =  FVector(0.0f, 0.0f, 20.0f);
public:

	
	// 델리게이트를 하지 않고 landed함수를 호출해서 착지를 확인하는 방법.
	// virtual void Landed(const FHitResult& Hit) override;

	// 델리게이트는 컴포넌트 같은 경우에도 할 수 있음. 컴포넌트에서 이 land를 호출할 수 있음
	// 예를들어, 무기컴포넌트에서 바인드하고 싶은 함수를 구현 캐릭터의 델리게이트를 얻어 올 수 있음. 참조해서 다른 클래스에서 함수를 달아놓을 수 있음.
	UFUNCTION()
	void PlayerLand(const FHitResult& Hit);

	// Weapon
	// 생성하고자 하는 무기 참조.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ASLWeaponBase> FakeBowInstance;
	
	// 화면에서 world 투영의 성공여부를 반환하는 함수.
	bool CrosshairWorldProperties(FVector& WorldPos, FVector& WorldDir);

	// World 위치 저장 변수.
	FVector CrosshairWorldPos;
	// World의 방향 저장 변수.
	FVector CrosshairWorldDir;

	bool bScreenWorld;
	
	
	FName WeaponSocketName;
	FName HideBowName;
	FName HideArrowName;

	FName SparrowSocketName;
	FName BowEmitterSocketName;
	// SLCharacterPlayer.h

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class ASLArrow> ArrowClass;


	
};
