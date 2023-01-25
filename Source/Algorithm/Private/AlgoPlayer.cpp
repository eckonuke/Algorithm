// Fill out your copyright notice in the Description page of Project Settings.


#include "AlgoPlayer.h"
#include "AlgoPuzzle.h"
#include <Blueprint/WidgetLayoutLibrary.h>
#include "Cube.h"

// Sets default values
AAlgoPlayer::AAlgoPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//퍼즐 클래스 가져오자
	ConstructorHelpers::FClassFinder<UAlgoPuzzle> tempPuzzle(TEXT("WidgetBlueprint'/Game/BP_AlgoPuzzle.BP_AlgoPuzzle_C'"));
	if (tempPuzzle.Succeeded()) {
		puzzleFactory = tempPuzzle.Class;
	}
}

// Called when the game starts or when spawned
void AAlgoPlayer::BeginPlay()
{
	Super::BeginPlay();
	//퍼즐 화면 보이게 하자
// 	puzzle = CreateWidget<UAlgoPuzzle>(GetWorld(), puzzleFactory);
// 	puzzle->AddToViewport();

	//마우스 커서 보이세 하자
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}

// Called every frame
void AAlgoPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAlgoPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//마우스 왼쪽 버튼
	PlayerInputComponent->BindAction(TEXT("MouseL"), IE_Pressed, this, &AAlgoPlayer::InputMouseLDown);
	PlayerInputComponent->BindAction(TEXT("MouseL"), IE_Released, this, &AAlgoPlayer::InputMouseLUp);
}

void AAlgoPlayer::InputMouseLDown() {
	//마우스 클릭 좌표를 얻어오자
	FVector2D mousePos =  UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	
	//선택된 카드 알아오자
	//puzzle->SelectedCard(mousePos);

	FVector start, end;
	FHitResult hit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	//Start Cube 알아오자
	start = GetActorLocation() + FVector::UpVector;
	end = start + FVector::DownVector * 10;
	if (GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, params)) {
		startCube = Cast<ACube>(hit.GetActor());
		openArray.Add(startCube);
		//UE_LOG(LogTemp, Warning, TEXT("Start: %s"), *hit.GetActor()->GetActorNameOrLabel());
	}

	//Goal Cube 알아오자
	FVector dir;
	//마우스 클릭위치를 3D공간의 좌표로 바꾸자
	GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(start, dir);
	end = start + dir *100000;
	if (GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, params)) {
		goalCube = Cast<ACube>(hit.GetActor());
		//UE_LOG(LogTemp, Warning, TEXT("Goal: %s"), *hit.GetActor()->GetActorNameOrLabel());
	}

	FindPath();
}

void AAlgoPlayer::InputMouseLUp() {
	//카드 초기화
	//puzzle->InitCard();
}

void AAlgoPlayer::FindPath() {
	//기준이 되는 큐브 설정
	currCube = openArray[0];
	
	//오른쪽
	AddOpen(FVector::RightVector);
	//왼쪽
	AddOpen(FVector::LeftVector);
	//앞
	
	//뒤

}

void AAlgoPlayer::AddOpen(FVector dir) {
	FHitResult hit;
	FVector start, end;
	FCollisionQueryParams param;
	param.AddIgnoredActor(currCube);

	start = currCube->GetActorLocation();
	end = start + dir * 100;
	if (GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, param)) {
		ACube* cube = Cast<ACube>(hit.GetActor());
		//openArray 와 closeArray에 존재 하지 않는다면
		if (openArray.Contains(cube) == false && closeArray.Contains(cube) == false) {
			//openArray 추가
			openArray.Add(cube);
			//부딪힌 큐브에 cost(비용)을 구하자
			cube->SetCost(currCube, goalCube);
		}
	}
}

void AAlgoPlayer::FindNear() {
	
}
