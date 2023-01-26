// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AlgoPlayer.generated.h"

UCLASS()
class ALGORITHM_API AAlgoPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAlgoPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//마우스 왼쪽 클릭
	void InputMouseLDown();
	void InputMouseLUp();

	//퍼즐 알고리즘
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UAlgoPuzzle> puzzleFactory;
	UPROPERTY(EditAnywhere)
	class UAlgoPuzzle* puzzle;

	//길찾기 알고리즘
public:
	UPROPERTY(EditAnywhere)
	class ACube* startCube;
	UPROPERTY(EditAnywhere)
	class ACube* goalCube;

	UPROPERTY(EditAnywhere)
	class ACube* currCube;

	UPROPERTY(EditAnywhere)
	TArray<class ACube*> openArray;
	UPROPERTY(EditAnywhere)
	TArray<class ACube*> closeArray;
	UPROPERTY(EditAnywhere)
	TArray<class ACube*> pathArray;

	void FindPath();
	void FindNear();
	void AddOpen(FVector dir);
	
	float currTime = 0;
	void SimpleMove();

};
