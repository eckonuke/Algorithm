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
	//ī�� ����/���� ũ��
	float cardSize = 200;
	//ī�� ���� ����
	int32 hCount = 3;
	//ī�� ���� ����
	int32 vCount = 3;

	//ī�� �̹��� �迭
	UPROPERTY(EditAnywhere)
	TArray<class UImage*> cards;
	//ī�� ��(0, 1) �迭
	UPROPERTY(EditAnywhere)
	TArray<int32> cardValues;
	//���� ī�� ���� ������ �迭
	UPROPERTY(EditAnywhere)
	TArray<int32> sameCards;

	//������ ī���� ��
	int32 selectCardValue;

	//���콺 Ŭ�� ��ġ�� �������ִ� ��(0��° ī���� ��ǥ)
	float offX;
	float offY;

public:
	void InitCard();
	void SelectedCard(FVector2D mousePos);
	void FindNearCard(int32 idx);
	void FindSameValue(int32 nearIdx);
};
