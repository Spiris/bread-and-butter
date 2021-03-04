// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "CoreFunctionLibrary.h"

#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/PlayerCameraManager.h"
#include "GameStateCore.h"

void UCoreFunctionLibrary::FaceCamera(AActor* Actor)
{
	if (APlayerCameraManager* cm = UGameplayStatics::GetPlayerCameraManager(Actor, 0))
	{
		const FVector& up = cm->GetActorUpVector();
		const FVector& right = cm->GetActorRightVector();
		const FVector& forward = FVector::CrossProduct(up, right);
		const FRotator& r = UKismetMathLibrary::MakeRotationFromAxes(forward, -right, up);
		Actor->SetActorRotation(r);
	}
}

void UCoreFunctionLibrary::FaceCamera_Component(USceneComponent* Component)
{
	if (APlayerCameraManager* cm = UGameplayStatics::GetPlayerCameraManager(Component, 0))
	{
		const FVector& up = cm->GetActorUpVector();
		const FVector& right = cm->GetActorRightVector();
		const FVector& forward = FVector::CrossProduct(up, right);
		const FRotator& r = UKismetMathLibrary::MakeRotationFromAxes(forward, -right, up);
		Component->SetWorldRotation(r);
	}
}

AGameStateCore* UCoreFunctionLibrary::GetGameStateCore(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (AGameStateBase* baseGS = UGameplayStatics::GetGameState(WorldContextObject))
		{
			return Cast<AGameStateCore>(baseGS);
		}
	}
	return nullptr;
}
