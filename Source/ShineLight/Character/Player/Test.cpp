// Fill out your copyright notice in the Description page of Project Settings.


#include "Test.h"

#include "SLCharacterPlayer.h"


// Sets default values for this component's properties
UTest::UTest()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTest::BeginPlay()
{
	Super::BeginPlay();

	// ...

	ASLCharacterPlayer* SLPlayer = Cast<ASLCharacterPlayer>(GetOwner());
	if (SLPlayer)
	{
		SLPlayer->LandedDelegate.AddDynamic(this, &UTest::OnLanded);
	}
}


// Called every frame
void UTest::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTest::OnLanded(const FHitResult& Hit)
{
	UE_LOG(LogTemp, Display, TEXT("OnLanded"));
}

