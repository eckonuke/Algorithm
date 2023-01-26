// Fill out your copyright notice in the Description page of Project Settings.


#include "Cube.h"
#include <Components/WidgetComponent.h>
#include <Components/TextBlock.h>
#include <Components/Border.h>

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

	//Widget Component�� ��������
	UWidgetComponent* widget = Cast<UWidgetComponent>(GetComponentByClass(UWidgetComponent::StaticClass()));
	UUserWidget* userWidget = widget->GetUserWidgetObject();
	//�׵θ� widget
	outLine = Cast<UBorder>(userWidget->GetWidgetFromName(TEXT("OutLine")));
	//totalCost widget
	textTotal = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("Total")));
	//startCost widget
	textStart = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("byStart")));
	//goalCost widget
	textGoal = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("byGoal")));
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

	//������ ���� ���
	startCost = currCube->startCost + FMath::Abs(vCurr.X - vMy.X) + FMath::Abs(vCurr.Y - vMy.Y);
	//������ ���� ���
	goalCost = FMath::Abs(vGoal.X - vMy.X) + FMath::Abs(vGoal.Y - vMy.Y);
	//���� ���
	totalCost = startCost + goalCost;

	//UI�� ǥ��
	textTotal->SetText(FText::AsNumber(totalCost));
	textStart->SetText(FText::AsNumber(startCost));
	textGoal->SetText(FText::AsNumber(goalCost));

	//�׵θ��� �Ķ������� �ٲ���
	SetColor(FLinearColor::Blue);

	//������ �������� Cost�� ����߳�? (�θ�)
	parentCube = currCube;

	UE_LOG(LogTemp, Warning, TEXT("%s --> startCost: %f, goalCost: %f, totalCost: %f"), *GetActorNameOrLabel(), startCost, goalCost, totalCost);
}

void ACube::SetColor(FLinearColor color) {
	outLine->SetBrushColor(color);
}

void ACube::SetInit() {
	SetColor(FLinearColor::Black);
	parentCube = nullptr;
	//�Ÿ� ��� ��� �ʱ�ȭ
	totalCost = startCost = goalCost = 0;

	textTotal->SetText(FText::AsNumber(totalCost));
	textStart->SetText(FText::AsNumber(startCost));
	textGoal->SetText(FText::AsNumber(goalCost));
}