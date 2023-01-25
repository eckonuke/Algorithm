// Fill out your copyright notice in the Description page of Project Settings.


#include "AlgoPuzzle.h"
#include <Components/Image.h>
#include <Components/CanvasPanelSlot.h>

void UAlgoPuzzle::NativeConstruct() {
	Super::NativeConstruct();
	//카드 이미지(widget) 가져와서 배열에 담자
	for (int32 i = 0; i < 9; i++) {
		FString str = FString::Printf(TEXT("Card%d"), i);
		UImage* img = Cast<UImage>(GetWidgetFromName(*str));
		cards.Add(img);
	}

	//offset 값 세팅
	UWidget* firstCard = GetWidgetFromName(TEXT("Card"));
	UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(firstCard->Slot);
	offX = slot->GetPosition().X;
	offY = slot->GetPosition().Y;

	//카드 초기화
	InitCard();
}

void UAlgoPuzzle::InitCard() {
	//cardValues의 갯수를 0으로 초기화
	cardValues.Empty();
	//sameCards의 갯수를 0으로 초기화
	sameCards.Empty();

	//랜덤한 값(0, 1) 을 뽑아서 0이면 노랑, 1이면 파랑
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
	//마우스x/카드가로크기 + 마우스y/카드세로크기 * 가로카드갯수 = 선택된 카드 idx
	int32 x = (mousePos.X - offX) / cardSize;
	int32 y = (mousePos.Y - offY) / cardSize;



	int32 selectIdx = x + y * hCount;
	UE_LOG(LogTemp, Error, TEXT("select idx : %d"), selectIdx);

	//만약에 범위를 벗어난곳을 선택했다면 함수를 나가자
	if (selectIdx > cards.Num()) return;

	//선택한 카드를 빨간색으로 하자
	cards[selectIdx]->SetColorAndOpacity(FLinearColor::Red);
	//선택한 카드 idx를 sameCards에 넣자
	sameCards.Add(selectIdx);

	//선택된 카드의 값을 설정하자
	selectCardValue = cardValues[selectIdx];

	//선택된 카드 좌,우,상,하 카드를 찾자
	FindNearCard(selectIdx);
}

void UAlgoPuzzle::FindNearCard(int32 idx) {
	int32 n;

	//왼쪽 카드
	if (idx % hCount > 0) {
		n = idx - 1;
		FindSameValue(n);
	}
	//오른쪽 카드
	if (idx % hCount < hCount - 1) {
		n = idx + 1;
		FindSameValue(n);
	}

	//아래쪽 카드
	if (idx / hCount > 0) {
		n = idx - hCount;
		FindSameValue(n);
	}
	//위쪽 카드
	if (idx / hCount < vCount - 1) {
		n = idx + hCount;
		FindSameValue(n);
	}
}

void UAlgoPuzzle::FindSameValue(int32 nearIdx) {
	if (selectCardValue == cardValues[nearIdx]) {
		//nearIdx가 sameCards에 없다면
		if (sameCards.Contains(nearIdx) == false) {
			cards[nearIdx]->SetColorAndOpacity(FLinearColor::Red);
			//sameCards에 추가
			sameCards.Add(nearIdx);
			//nearIdx로 다시 근접한 카드를 검색
			FindNearCard(nearIdx);
		}
	}
}