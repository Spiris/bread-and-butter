// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CoreFunctionLibrary.generated.h"


class AActor;
class USceneComponent;
class AGameStateCore;
UCLASS()
class BNB_API UCoreFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "Actor"))
		static void FaceCamera(AActor* Actor);
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "Component"))
		static void FaceCamera_Component(USceneComponent* Component);
	UFUNCTION(BlueprintPure, meta = (DefaultToSelf = "WorldContextObject", WorldContext = "WorldContextObject"))
		static AGameStateCore* GetGameStateCore(UObject* WorldContextObject);
	
};
