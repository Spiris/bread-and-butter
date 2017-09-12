// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "ServerStats.generated.h"

USTRUCT()
struct FServerRoute
{
	GENERATED_BODY()
public:
	uint8 WHY;
	FServerRoute() {}
	UPROPERTY()
		FString Name = "";
};
FORCEINLINE FArchive& operator<<(FArchive& Ar, FServerRoute& Rt)
{
	Ar << Rt.Name;
	return Ar;
}

USTRUCT()
struct FServerStats
{
	GENERATED_BODY()
public:
	uint8 WHY;
	FServerStats() : Routes() {}
	UPROPERTY()
		FString Name = "";
	UPROPERTY()
		FString EndpointString = "";
	UPROPERTY()
		int32 Population = 0;
	UPROPERTY()
		TArray<FServerRoute> Routes;
};

FORCEINLINE FArchive& operator<<(FArchive& Ar, FServerStats& SS)
{
	Ar << SS.Name;
	Ar << SS.EndpointString;
	Ar << SS.Population;
	Ar << SS.Routes;
	return Ar;
}