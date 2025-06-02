// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SLWeaponBase.generated.h"

UCLASS()
class SHINELIGHT_API ASLWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASLWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category= "Weapon", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* SkeletalMesh;

public:
	FORCEINLINE USkeletalMeshComponent* const GetMesh() { return SkeletalMesh; } 
};
