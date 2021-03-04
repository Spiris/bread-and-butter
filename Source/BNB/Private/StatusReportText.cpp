// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "StatusReportText.h"

#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"


AStatusReportText::AStatusReportText() : Super()
{
	Root = CreateDefaultSubobject<USphereComponent>(FName("Root"));
	SetRootComponent(Root);
	Root->SetHiddenInGame(true, false);

	Text = CreateDefaultSubobject<UTextRenderComponent>(FName("Text"));
	Text->SetupAttachment(Root);
	InitialLifeSpan = 3.0f;
}
void AStatusReportText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStatusReportText::SetText(const FText& InText)
{
	Text->SetText(InText);
}
void AStatusReportText::SetTextColor(const FColor& InColor)
{
	Text->SetTextRenderColor(InColor);
}