// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AlgoPuzzle.generated.h"

/**
 * 
 */
UCLASS()
class ALGORITHM_API UAlgoPuzzle : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	//카드 가로/세로 크기
	float cardSize = 200;
	//카드 가로 갯수
	int32 hCount = 3;
	//카드 세로 갯수
	int32 vCount = 3;

	//카드 이미지 배열
	UPROPERTY(EditAnywhere)
	TArray<class UImage*> cards;
	//카드 값(0, 1) 배열
	UPROPERTY(EditAnywhere)
	TArray<int32> cardValues;
	//같은 카드 값을 가지는 배열
	UPROPERTY(EditAnywhere)
	TArray<int32> sameCards;

	//선택한 카드의 값
	int32 selectCardValue;

	//마우스 클릭 위치를 보정해주는 값(0번째 카드의 좌표)
	float offX;
	float offY;

public:
	void InitCard();
	void SelectedCard(FVector2D mousePos);
	void FindNearCard(int32 idx);
	void FindSameValue(int32 nearIdx);
};
