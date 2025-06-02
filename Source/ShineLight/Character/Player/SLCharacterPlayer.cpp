// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/SLCharacterPlayer.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Camera/CameraComponent.h"

#include "Components/SkeletalMeshComponent.h"

#include "Engine/TimerHandle.h"

#include "Weapon/SLWeaponBase.h"
#include "Engine/SkeletalMeshSocket.h"

#include "Kismet/GameplayStatics.h"
#include "Weapon/SLArrow.h"
ASLCharacterPlayer::ASLCharacterPlayer() : WalkSpeed(350.0f), RunSpeed(700.0f), CameraDefaultFOV(0.0f), CameraCurrentFOV(0.0f), CameraAmingFOV(50.0f), CameraAmingSpeed(20.0f), NormalTurnRate(1.0f), NormalLookUp(1.0f), AimTurnRate(0.25f), AimLookUp(0.25f), MouseXSensitivity(0.0f), MouseYSensitivity(0.0f)
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false; 
	bUseControllerRotationRoll = false;
	// 무브먼트 설정.
	// 이동 방향으로 자동 회전
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 400.f;
	JumpMaxCount = 2; // 기본은 1, 2로 하면 이단점프 가능
	
	// 카메라 붐(SpringArm) 생성 (충돌이 발생하면 플레이어 쪽으로 당겨짐)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // 카메라는 캐릭터 뒤에서 이 거리만큼 떨어져 따라감

	CameraBoom->bUsePawnControlRotation = true; // 컨트롤러의 회전을 기준으로 붐이 회전하도록 설정

	
	// --- 실제 시점을 제공할 카메라 생성 ----
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // 카메라를 붐의 끝에 부착하고, 붐이 컨트롤러 방향에 맞춰 회전하게 함
	FollowCamera->bUsePawnControlRotation = false; // 카메라는 붐에 상대적으로 회전하지 않음 (붐만 회전하고 카메라는 고정된 방향 유지)
	

	// 리소스 설정.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(TEXT("/Game/ParagonSparrow/Characters/Heroes/Sparrow/Skins/ZechinHuntress/Meshes/Sparrow_ZechinHuntress.Sparrow_ZechinHuntress"));
	if (CharacterMesh.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);
	}
	
	// 이동(WASD) 입력 액션 로딩.
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Game/ShineLight/Input/Action/IA_Move.IA_Move"));
	if (MoveActionRef.Object)
	{
		MoveAction = MoveActionRef.Object;
	}
	// 뷰(마우스) 입력 액션 로딩.
	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(TEXT("/Game/ShineLight/Input/Action/IA_Look.IA_Look"));
	if (LookActionRef.Object)
	{
		LookAction = LookActionRef.Object;
	}
	// 점프 입력 액션 로딩.
	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(TEXT("/Game/ShineLight/Input/Action/IA_Jump.IA_Jump"));
	if (JumpActionRef.Object)
	{
		JumpAction = JumpActionRef.Object;
	}
	
	// 점프 입력 액션 로딩.
	static ConstructorHelpers::FObjectFinder<UInputAction> SprintActionRef(TEXT("/Game/ShineLight/Input/Action/IA_Sprint.IA_Sprint"));
	if (SprintActionRef.Object)
	{
		SprintAction = SprintActionRef.Object;
	}
	// 점프 입력 액션 로딩.
	static ConstructorHelpers::FObjectFinder<UInputAction> RollActionRef(TEXT("/Game/ShineLight/Input/Action/IA_Roll.IA_Roll"));
	if (RollActionRef.Object)
	{
		RollAction = RollActionRef.Object;
	}
	
	// 조준 입력 액션 로딩.
	static ConstructorHelpers::FObjectFinder<UInputAction> AimActionRef(TEXT("/Game/ShineLight/Input/Action/IA_Aim.IA_Aim"));
	if(AimActionRef.Object)
	{
		AimAction = AimActionRef.Object;
	}
	
	// 조준 입력 액션 로딩.
	static ConstructorHelpers::FObjectFinder<UInputAction> FrieActionRef(TEXT("/Game/ShineLight/Input/Action/IA_Fire.IA_Fire"));
	if(FrieActionRef.Object)
	{
		FireAction = FrieActionRef.Object;
	}
	

	// 입력 매핑 시스템 설정.
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_DefaultRef(TEXT("/Game/ShineLight/Input/IMC_Default.IMC_Default"));
	if (IMC_DefaultRef.Object)
	{
		DefaultMappingContext = IMC_DefaultRef.Object;
	}
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));


	// [애니메이션 설정]
	// [애니메이션 설정]
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/ShineLight/Animation/ABP_SLPlayerAnim.ABP_SLPlayerAnim_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}


	// 구르기 몽타주 애니메이션
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RollMontageRef(TEXT("/Game/ShineLight/Animation/Montage/AM_Roll.AM_Roll"));
	if (RollMontageRef.Object)
	{
		RollMontage = RollMontageRef.Object;
	}

	// 무기 로딩.
	static ConstructorHelpers::FClassFinder<ASLWeaponBase> DefaultWeaponRef(TEXT("/Game/ShineLight/Blueprint/BP_Weapon.BP_Weapon_C"));
	if (DefaultWeaponRef.Class)
	{
		FakeBowInstance = DefaultWeaponRef.Class;
	}
	
	LandedDelegate.AddDynamic(this, &ASLCharacterPlayer::PlayerLand);
	

	WeaponSocketName= FName(TEXT("Custom_Bow"));
	HideBowName = FName(TEXT("bow_base"));
	HideArrowName = FName(TEXT("arrow_nock"));
	SparrowSocketName = FName(TEXT("ArrowSpawnSocket"));
	BowEmitterSocketName = FName(TEXT("BowEmitterSocket"));

	
	// 무기 로딩.
	static ConstructorHelpers::FClassFinder<ASLArrow> ArrowRef(TEXT("/Game/ShineLight/Blueprint/BP_SLArrow.BP_SLArrow_C"));
	if (ArrowRef.Class)
	{
		ArrowClass = ArrowRef.Class;
	}
	
}

void ASLCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (FollowCamera)
	{
		CameraDefaultFOV = FollowCamera->FieldOfView;
		CameraCurrentFOV = CameraDefaultFOV;
	}
	//  본 숨기기
	GetMesh()->HideBoneByName(HideBowName, EPhysBodyOp::PBO_None);
	GetMesh()->HideBoneByName(HideArrowName, EPhysBodyOp::PBO_None);

	SpawnWeapon();
}

void ASLCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASLCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// 입력 매핑 컨텍스트 설정.
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// 입력 액션 바인드.
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// 이동 입력 바인딩.
		EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered, this, &ASLCharacterPlayer::Move);

		// 뷰 입력 바인딩.
		EnhancedInputComponent->BindAction(LookAction,ETriggerEvent::Triggered, this, &ASLCharacterPlayer::Look);

		// 접프 입력 바인딩.
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASLCharacterPlayer::Jumping);
		
		// 가속 입력 바인딩.
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ASLCharacterPlayer::StartSprinting);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASLCharacterPlayer::StopSprinting);
		
		// 구르기 시작 입력 바인딩.
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Started, this, &ASLCharacterPlayer::Roll);

		// 조준 시작 입력 바인딩.
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this,&ASLCharacterPlayer::Aim );
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this,&ASLCharacterPlayer::Aim );

		// 공격 입력 바인딩.
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this,&ASLCharacterPlayer::Fire );
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this,&ASLCharacterPlayer::Fire );
		
	}
	
}

void ASLCharacterPlayer::Move(const FInputActionValue& Value)
{
	// 입력 값 읽기.
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// 컨트롤러의 회전 값.	
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 방향 구하기.
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 무브먼트 컴포넌트에 값 전달.
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASLCharacterPlayer::Look(const FInputActionValue& Value)
{
	// 마우스 입력값 읽기.
	FVector2D LookDirectionVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if (!bIsAiming)
		{
			MouseXSensitivity = NormalTurnRate;
			MouseYSensitivity = NormalTurnRate;
		}
		else
		{
			MouseXSensitivity = AimTurnRate;
			MouseYSensitivity = AimLookUp;
		}
		// 좌우 방향.
		AddControllerYawInput(LookDirectionVector.X *MouseXSensitivity);
		// 위아래 방향.
		AddControllerPitchInput(LookDirectionVector.Y *MouseYSensitivity);
	}
}

void ASLCharacterPlayer::StartSprinting(const FInputActionValue& Value)
{
	bIsRunning = true;
	// 스프린트 값 설정
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void ASLCharacterPlayer::StopSprinting(const FInputActionValue& Value)
{
	bIsRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ASLCharacterPlayer::Jumping(const FInputActionValue& Value)
{
	Super::Jump();
}

void ASLCharacterPlayer::Roll(const FInputActionValue& Value)
{
	if (bPlayerIsRoll || RollMontage == nullptr)
	{
		// 구르기 중이거나 롤 몽타주 없으면 무시
		return;
	}
	bPlayerIsRoll = true;
	
	UAnimInstance * AnimInstance = GetMesh()->GetAnimInstance();
	
	if (AnimInstance != nullptr)
	{
		AnimInstance->Montage_Play(RollMontage);
	
		
		// 몽타주 재생이 시작되면, 재생이 종료될 때 호출되는 델리게이트에 등록.
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &ASLCharacterPlayer::RollStop);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, RollMontage);
	
	}
}

void ASLCharacterPlayer::RollStop(class UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	bPlayerIsRoll = false;
}

void ASLCharacterPlayer::Aim(const FInputActionValue& Value)
{
	bIsAiming = Value.Get<bool>();
	
	if (bIsAiming)
	{
		bUseControllerRotationYaw = true; 
		GetCharacterMovement()->bOrientRotationToMovement = false;
		CameraBoom->SocketOffset = AimSocketOffsetVecotr;
		UE_LOG(LogTemp, Warning, TEXT("bIsAiming(): %s"), bIsAiming ? TEXT("true") : TEXT("false"));
		
	}
	else
	{
		bUseControllerRotationYaw = false; 
		GetCharacterMovement()->bOrientRotationToMovement = true;
		CameraBoom->SocketOffset = DefaultCameraSocketOffsetVecotr;
	}

	SetAimingFOV(bIsAiming);
}

void ASLCharacterPlayer::Fire(const FInputActionValue& Value)
{
	bIsFire = Value.Get<bool>();
	
	
	// 화살이 발사되었을 때 
	if (bIsFire)
	{
		
		// 본을 표시.
		GetMesh()->UnHideBoneByName(HideBowName);
		GetMesh()->UnHideBoneByName(HideArrowName);
		
		// 현재 무기의 소켓을 받아온다.
		const USkeletalMeshSocket* BowEmitterSocket =  GetMesh()->GetSocketByName(BowEmitterSocketName);
		
		if (!BowEmitterSocket)
		{
			return;
		}

		
		// 현재 설정된 무기에서 화살이 발사되는 소켓의 위치값을 받아온다.
		const FTransform SocketTransform =  BowEmitterSocket->GetSocketTransform(GetMesh());
		
		bScreenWorld =CrosshairWorldProperties(CrosshairWorldPos, CrosshairWorldDir);
		
		if (bScreenWorld)
		{
			FHitResult HitResult;
			FVector Start = CrosshairWorldPos;
			FVector End = Start + CrosshairWorldDir * 60000.0f;
			GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility);
			
			if (HitResult.bBlockingHit)
			{
				DrawDebugLine(GetWorld(),Start, End, FColor::Red,false, 2.0f);
				DrawDebugPoint(GetWorld(), HitResult.Location, 4.0f, FColor::Red, false, 2.0f);
			}
			FHitResult WeaponTraceHit;
			const FVector WeaponStart = SocketTransform.GetLocation();
			const FVector WeaponEnd = HitResult.Location;
		
			GetWorld()->LineTraceSingleByChannel(WeaponTraceHit, WeaponStart, WeaponEnd, ECollisionChannel::ECC_Visibility);
			if (WeaponTraceHit.bBlockingHit)
			{
				// UE_LOG(LogTemp, Warning, TEXT("WeaponTraceHit.bBlockingHit : %s"), WeaponTraceHit.bBlockingHit?  TEXT("true") :  TEXT("false"));
				DrawDebugLine(GetWorld(), WeaponStart, WeaponTraceHit.Location, FColor::Red, false, 2.0f);
				DrawDebugPoint(GetWorld(), WeaponTraceHit.Location, 4.0f, FColor::Blue, false, 2.0f);
			}
			
			// 목표 위치 → 화살 방향 벡터 계산
			FVector Direction = (HitResult.Location - SocketTransform.GetLocation()).GetSafeNormal();
			
			// 화살 액터 생성
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			
			ASLArrow* Arrow = GetWorld()->SpawnActor<ASLArrow>(
				ArrowClass, // UPROPERTY로 미리 세팅한 화살 클래스
				WeaponStart,
				Direction.Rotation(),
				SpawnParams
			);
			if (!ArrowClass)
			{
				UE_LOG(LogTemp, Error, TEXT("ArrowClass is NULL! Check Blueprint assignment."));
				return;
			}
			if (Arrow)
			{
				UE_LOG(LogTemp, Error, TEXT("FireInDirection"));
				Arrow->FireInDirection(WeaponTraceHit.Location);
			
				
				// 화살이 플레이어 자신과 충돌하지 않게 설정
				Arrow->ArrowMesh->IgnoreActorWhenMoving(this, true);  // 화살이 메인 콜리전 컴포넌트일 경우
			
			}
			
		}
	}
	else
	{
		//  본 숨기기
		GetMesh()->HideBoneByName(HideBowName, EPhysBodyOp::PBO_None);
		GetMesh()->HideBoneByName(HideArrowName, EPhysBodyOp::PBO_None);
		
	}
	
		
}

void ASLCharacterPlayer::SpawnWeapon()
{
	// 가짜 무기 손에 장착.
	ASLWeaponBase* Weapon = GetWorld()->SpawnActor<ASLWeaponBase>(FakeBowInstance);
	if (Weapon)
	{
		
		const USkeletalMeshSocket* LeftHandSocket =  GetMesh()->GetSocketByName(WeaponSocketName);
		if (LeftHandSocket)
		{
			LeftHandSocket->AttachActor(Cast<AActor>(Weapon), GetMesh());
		}
	}
	
}


void ASLCharacterPlayer::SetAimingFOV(uint8 bpIsAiming)
{
	TargetFOV = bpIsAiming ? CameraAmingFOV : CameraDefaultFOV;

	GetWorld()->GetTimerManager().SetTimer(AimingTimerHandle, this, &ASLCharacterPlayer::UpdateFOV, GetWorld()->DeltaTimeSeconds, true);

}

void ASLCharacterPlayer::UpdateFOV()
{
	CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV, TargetFOV, GetWorld()->DeltaTimeSeconds, CameraAmingSpeed);
	FollowCamera->SetFieldOfView(CameraCurrentFOV);

	if (FMath::IsNearlyEqual(CameraCurrentFOV, TargetFOV, 0.1f))
	{
		GetWorld()->GetTimerManager().ClearTimer(AimingTimerHandle);
		
	}
}

void ASLCharacterPlayer::PlayerLand(const FHitResult& Hit)
{
	if (bIsRunning)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

bool ASLCharacterPlayer::CrosshairWorldProperties(FVector& WorldPos, FVector& WorldDir)
{
	// 화면의 너비와 높이를 픽셀단위로 나타내는 뷰포트의 크기를 저장하는 변수.
	FVector2D ViewportSize;
	// 값을 검색하기 위해 엔진과 게임 뷰포트가 유효한지 확인
	if (GEngine && GEngine->GameViewport)
	{
		// 뷰포트 사이즈의 값 할당.
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	// 화면에서 크로스 헤어의 위치를 계산.
	FVector2D CrosshairLocation(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);
	return UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), CrosshairLocation, WorldPos,WorldDir);
}
