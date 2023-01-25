// Fill out your copyright notice in the Description page of Project Settings.


#include "AlgoPuzzle.h"
#include <Components/Image.h>
#include <Components/CanvasPanelSlot.h>

void UAlgoPuzzle::NativeConstruct() {
	Super::NativeConstruct();
	//ī�� �̹���(widget) �����ͼ� �迭�� ����
	for (int32 i = 0; i < 9; i++) {
		FString str = FString::Printf(TEXT("Card%d"), i);
		UImage* img = Cast<UImage>(GetWidgetFromName(*str));
		cards.Add(img);
	}

	//offset �� ����
	UWidget* firstCard = GetWidgetFromName(TEXT("Card"));
	UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(firstCard->Slot);
	offX = slot->GetPosition().X;
	offY = slot->GetPosition().Y;

	//ī�� �ʱ�ȭ
	InitCard();
}

void UAlgoPuzzle::InitCard() {
	//cardValues�� ������ 0���� �ʱ�ȭ
	cardValues.Empty();
	//sameCards�� ������ 0���� �ʱ�ȭ
	sameCards.Empty();

	//������ ��(0, 1) �� �̾Ƽ� 0�̸� ���, 1�̸� �Ķ�
	for (int32 i = 0; i < cards.Num(); i++) {
		int32 rand = FMath::RandRange(0, 1);
		cardValues.Add(rand);
		if (rand == 0) {
			cards[i]->SetColorAndOpacity(FLinearColor::Yellow);
		}
		else {
			cards[i]->SetColorAndOpacity(FLinearColor::Blue);
		}
	}
}

void UAlgoPuzzle::SelectedCard(FVector2D mousePos) {
	//���콺x/ī�尡��ũ�� + ���콺y/ī�弼��ũ�� * ����ī�尹�� = ���õ� ī�� idx
	int32 x = (mousePos.X - offX) / cardSize;
	int32 y = (mousePos.Y - offY) / cardSize;



	int32 selectIdx = x + y * hCount;
	UE_LOG(LogTemp, Error, TEXT("select idx : %d"), selectIdx);

	//���࿡ ������ ������� �����ߴٸ� �Լ��� ������
	if (selectIdx > cards.Num()) return;

	//������ ī�带 ���������� ����
	cards[selectIdx]->SetColorAndOpacity(FLinearColor::Red);
	//������ ī�� idx�� sameCards�� ����
	sameCards.Add(selectIdx);

	//���õ� ī���� ���� ��������
	selectCardValue = cardValues[selectIdx];

	//���õ� ī�� ��,��,��,�� ī�带 ã��
	FindNearCard(selectIdx);
}

void UAlgoPuzzle::FindNearCard(int32 idx) {
	int32 n;

	//���� ī��
	if (idx % hCount > 0) {
		n = idx - 1;
		FindSameValue(n);
	}
	//������ ī��
	if (idx % hCount < hCount - 1) {
		n = idx + 1;
		FindSameValue(n);
	}

	//�Ʒ��� ī��
	if (idx / hCount > 0) {
		n = idx - hCount;
		FindSameValue(n);
	}
	//���� ī��
	if (idx / hCount < vCount - 1) {
		n = idx + hCount;
		FindSameValue(n);
	}
}

void UAlgoPuzzle::FindSameValue(int32 nearIdx) {
	if (selectCardValue == cardValues[nearIdx]) {
		//nearIdx�� sameCards�� ���ٸ�
		if (sameCards.Contains(nearIdx) == false) {
			cards[nearIdx]->SetColorAndOpacity(FLinearColor::Red);
			//sameCards�� �߰�
			sameCards.Add(nearIdx);
			//nearIdx�� �ٽ� ������ ī�带 �˻�
			FindNearCard(nearIdx);
		}
	}
}