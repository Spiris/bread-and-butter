// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once
#include "CoreMinimal.h"

#include "Containers/Queue.h"

class FSocket;
class UWorldNexus;
struct FIPv4Endpoint;
struct FNexusMessage;
class FNexusRelayConnection;
class FTcpListener;

class FNexusRelay
{
public:
	FNexusRelay();
	virtual ~FNexusRelay();
	void OpenRelay(const FIPv4Endpoint& InListenEndpoint);
	void CloseRelay();
	
	void TestConnections();
	void PollPendingConnections();
	void ReadMessages(UWorldNexus* WorldNexus);
	void SendMessage(const FString& InEndpointString, const FNexusMessage& InMessage);
	void AddConnectionAtEndpoint(const FIPv4Endpoint& Endpoint);
	
	void RemoveConnection(const FString& InRemoteEndpointString);
	FIPv4Endpoint GetLocalEndpoint();
private:
	TMap <FString, FNexusRelayConnection*> Connections;
	FTcpListener* TcpListener;
	TQueue<FNexusRelayConnection*> PendingConnections;
	bool AcceptIncomingConnection(FSocket* InSocket, const FIPv4Endpoint& Endpoint);
	void AddNewConnection(FNexusRelayConnection* InConnection);
};
