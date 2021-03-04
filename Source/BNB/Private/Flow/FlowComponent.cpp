// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "Flow/FlowComponent.h"

#include "Components/PrimitiveComponent.h"

UFlowComponent::UFlowComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFlowComponent::SetPrimitiveComponent(UPrimitiveComponent* NewPrimitiveComponent)
{
	if (PrimitiveComponent)
	{
		PrimitiveComponent->OnComponentBeginOverlap.RemoveDynamic(this, &UFlowComponent::OnComponentBeginOverlap);
		PrimitiveComponent->OnComponentEndOverlap.RemoveDynamic(this, &UFlowComponent::OnComponentEndOverlap);
	}
	PrimitiveComponent = NewPrimitiveComponent;
	if (PrimitiveComponent)
	{
		PrimitiveComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &UFlowComponent::OnComponentBeginOverlap);
		PrimitiveComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &UFlowComponent::OnComponentEndOverlap);
	}
}

void UFlowComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UFlowComponent::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void UFlowComponent::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}
