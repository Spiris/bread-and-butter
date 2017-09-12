// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Runnable.h"

class FSocket;
class UWorldNexus;
struct FIPv4Endpoint;
struct FNexusMessage;
class FNexusRelayConnection;

class FNexusRelay : public FRunnable
{
public:
	FNexusRelay();
	virtual ~FNexusRelay();
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	void EnsureCompletion();

	void TestConnections();
	void ReadConnections(UWorldNexus* WorldNexus);
	void SendMessage(const FString& InEndpointString, const FNexusMessage& InMessage);
	void AddConnection(FSocket* InSocket, const FIPv4Endpoint& InRemoteEndpoint);
	void RemoveConnection(const FString& InRemoteEndpointString);
private:
	TMap <FString, FNexusRelayConnection*> Connections;
};
