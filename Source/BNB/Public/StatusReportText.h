// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "StatusReportText.generated.h"

class USphereComponent;
class UTextRenderComponent;

UCLASS()
class BNB_API AStatusReportText : public AActor
{
	GENERATED_BODY()
public:
	
	AStatusReportText();
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USphereComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UTextRenderComponent* Text;

	void SetText(const FText& InText);
	void SetTextColor(const FColor& InColor);
};
