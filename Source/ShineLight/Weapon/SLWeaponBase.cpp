// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/SLWeaponBase.h"

// Sets default values
ASLWeaponBase::ASLWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon Mesh");
	SetRootComponent(SkeletalMesh);
}

// Called when the game starts or when spawned
void ASLWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASLWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

