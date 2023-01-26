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

	//���� Ŭ���� ��������
	ConstructorHelpers::FClassFinder<UAlgoPuzzle> tempPuzzle(TEXT("WidgetBlueprint'/Game/BP_AlgoPuzzle.BP_AlgoPuzzle_C'"));
	if (tempPuzzle.Succeeded()) {
		puzzleFactory = tempPuzzle.Class;
	}
}

// Called when the game starts or when spawned
void AAlgoPlayer::BeginPlay()
{
	Super::BeginPlay();
	//���� ȭ�� ���̰� ����
// 	puzzle = CreateWidget<UAlgoPuzzle>(GetWorld(), puzzleFactory);
// 	puzzle->AddToViewport();

	//���콺 Ŀ�� ���̼� ����
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}

// Called every frame
void AAlgoPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SimpleMove();
}

void AAlgoPlayer::SimpleMove() {
	//���࿡ pathArray�� ���� ���ٸ� �Լ��� ������.
	if(pathArray.Num() == 0) return;

	currTime += GetWorld()->GetDeltaSeconds();
	if (currTime > 0.3f) {
		currTime = 0;
		FVector pos = pathArray[0]->GetActorLocation();
		pos.Z = 100;
		SetActorLocation(pos);

		pathArray.RemoveAt(0);
	}
}

// Called to bind functionality to input
void AAlgoPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//���콺 ���� ��ư
	PlayerInputComponent->BindAction(TEXT("MouseL"), IE_Pressed, this, &AAlgoPlayer::InputMouseLDown);
	PlayerInputComponent->BindAction(TEXT("MouseL"), IE_Released, this, &AAlgoPlayer::InputMouseLUp);
}

void AAlgoPlayer::InputMouseLDown() {
	//
	if (startCube != nullptr) {
		FindPath();
		return;
	}

	//�ʱ⼳��
	for (int32 i = 0; i < openArray.Num(); i++) {
		openArray[i]->SetInit();
	}
	for (int32 i = 0; i < closeArray.Num(); i++) {
		closeArray[i]->SetInit();
	}
	openArray.Empty();
	closeArray.Empty();

	//���콺 Ŭ�� ��ǥ�� ������
	FVector2D mousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

	//���õ� ī�� �˾ƿ���
	//puzzle->SelectedCard(mousePos);

	FVector start, end;
	FHitResult hit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	//Start Cube �˾ƿ���
	start = GetActorLocation() + FVector::UpVector;
	end = start + FVector::DownVector * 10;
	if (GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, params)) {
		startCube = Cast<ACube>(hit.GetActor());
		openArray.Add(startCube);
		//UE_LOG(LogTemp, Warning, TEXT("Start: %s"), *hit.GetActor()->GetActorNameOrLabel());
	}

	//Goal Cube �˾ƿ���
	FVector dir;
	//���콺 Ŭ����ġ�� 3D������ ��ǥ�� �ٲ���
	GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(start, dir);
	end = start + dir * 100000;
	if (GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, params)) {
		goalCube = Cast<ACube>(hit.GetActor());
		//UE_LOG(LogTemp, Warning, TEXT("Goal: %s"), *hit.GetActor()->GetActorNameOrLabel());
	}

	FindPath();
}

void AAlgoPlayer::InputMouseLUp() {
	//ī�� �ʱ�ȭ
	//puzzle->InitCard();
}

void AAlgoPlayer::FindPath() {
	//������ �Ǵ� ť�� ����
	currCube = openArray[0];

	//������
	AddOpen(FVector::RightVector);
	//����
	AddOpen(FVector::LeftVector);
	//��
	AddOpen(FVector::BackwardVector);
	//��
	AddOpen(FVector::ForwardVector);

	//currCube�� openArray���� ����
	openArray.Remove(currCube);
	//currCube�� closeArray�� �߰�
	closeArray.Add(currCube);
	//currCube�� �׵θ� ���� ���������� �ٲ۴�.
	currCube->SetColor(FLinearColor::Red);

	//goal���� ã�ư�����? (��ã�Ⱑ ������?)
	if (openArray[0] == goalCube || openArray.Num() == 0) {
		//�ִܰŸ��� ��������� ǥ������
		ACube* cube = goalCube;
		while (cube->parentCube != nullptr) {
			//���� �� �߰�
			pathArray.Insert(cube, 0);

			cube->SetColor(FLinearColor::Yellow);
			cube = cube->parentCube;
		}
		//ó�� ť�� �߰�
		pathArray.Insert(startCube, 0);
		startCube->SetColor(FLinearColor::Yellow);

		//�ʱ�ȭ
		startCube = nullptr;
		goalCube = nullptr;
	}
	else {
		FindPath();
	}
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
		//openArray �� closeArray�� ���� ���� �ʴ´ٸ�
		if (openArray.Contains(cube) == false && closeArray.Contains(cube) == false) {
			//�ε��� ť�꿡 cost(���)�� ������
			cube->SetCost(currCube, goalCube);

			//openArray�� ����ִ� ť����� totalCost�� cube�� totalCost�� ���Ѵ�
			int32 i = 0;
			for (i = 0; i < openArray.Num(); i++) {
				if (openArray[i]->totalCost >= cube->totalCost) {
					break;
				}
			}
			//openArray �߰�
			openArray.Insert(cube, i);
		}
	}
}

void AAlgoPlayer::FindNear() {

}
