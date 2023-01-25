// Fill out your copyright notice in the Description page of Project Settings.


#include "Cube.h"

// Sets default values
ACube::ACube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACube::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACube::SetCost(ACube* currCube, ACube* goalCube) {
	FVector vCurr = currCube->GetActorLocation();
	FVector vGoal = goalCube->GetActorLocation();
	FVector vMy = GetActorLocation();

	//시작점 기준 비용
	startCost = currCube->startCost + FMath::Abs(vCurr.X - vMy.X) + FMath::Abs(vCurr.Y - vMy.Y);
	//도착점 기준 비용
	endCost = currCube->endCost + FMath::Abs(vGoal.X - vMy.X) + FMath::Abs(vGoal.Y - vMy.Y);
	//최종 비용
	totalCost = startCost + endCost;

	UE_LOG(LogTemp, Warning, TEXT("%s --> startCost: %f, endCost: %f, totalCost: %f"), *GetActorNameOrLabel(), startCost, endCost, totalCost);
}