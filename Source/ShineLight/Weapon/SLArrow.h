// SLArrow.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SLArrow.generated.h"

UCLASS()
class SHINELIGHT_API ASLArrow : public AActor
{
	GENERATED_BODY()

public:    
	ASLArrow();

protected:
	virtual void BeginPlay() override;

public:    
	virtual void Tick(float DeltaTime) override;

	// 화살을 발사 방향으로 날리기 위한 함수
	void FireInDirection(const FVector& ShootDirection);

	// 외부에서 충돌 무시할 액터를 등록할 때 사용할 함수
	UFUNCTION(BlueprintCallable, Category = "Collision")
	void IgnoreCollisionWithActor(AActor* ActorToIgnore);

	// 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> ArrowMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovement;
};
