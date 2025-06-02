// SLArrow.cpp

#include "Weapon/SLArrow.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ASLArrow::ASLArrow()
{
    PrimaryActorTick.bCanEverTick = true;

    RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    SetRootComponent(RootScene);

    ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
    ArrowMesh->SetupAttachment(RootScene);

    // 충돌 기본 설정
    //ArrowMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    //ArrowMesh->SetCollisionObjectType(ECC_PhysicsBody);
    //ArrowMesh->SetCollisionResponseToAllChannels(ECR_Block);
    ArrowMesh->SetCollisionProfileName(TEXT("NoCollision"));
    ArrowMesh->SetSimulatePhysics(false);
    ArrowMesh->SetEnableGravity(false);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ArrowMeshRef(
        TEXT("/Game/ParagonSparrow/FX/Meshes/Heroes/Sparrow/Abilities/SM_Sparrow_Arrow.SM_Sparrow_Arrow"));
    if (ArrowMeshRef.Succeeded())
    {
        ArrowMesh->SetStaticMesh(ArrowMeshRef.Object);
    }

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = ArrowMesh;
    ProjectileMovement->InitialSpeed = 3000.0f;
    ProjectileMovement->MaxSpeed = 3000.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 1.0f;

    
    InitialLifeSpan = 10.0f;
}

void ASLArrow::BeginPlay()
{
    Super::BeginPlay();

    // 만약 오너를 이미 설정했다면, 오너와 충돌을 무시하도록 처리할 수도 있습니다.
    if (AActor* OwnerActor = GetOwner())
    {
        ArrowMesh->IgnoreActorWhenMoving(OwnerActor, true);
    }
}

void ASLArrow::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASLArrow::FireInDirection(const FVector& ShootDirection)
{
    if (ProjectileMovement)
    {
        ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
    }
}

void ASLArrow::IgnoreCollisionWithActor(AActor* ActorToIgnore)
{
    if (ActorToIgnore && ArrowMesh)
    {
        // 이동 중(ProjectileMovement에 의해) 이 메쉬가 ActorToIgnore와 충돌하지 않도록 설정
        ArrowMesh->IgnoreActorWhenMoving(ActorToIgnore, true);

        // 만약 채널 단위로도 완전히 무시하고 싶다면 아래처럼 추가로 설정 가능
        // ArrowMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
        // ※ 위 예시는 Pawn 채널을 완전히 무시하는 예시이며, 실제 프로젝트에서는 채널 설정을 확인해야 합니다.
    }
}
