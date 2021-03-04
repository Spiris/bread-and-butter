// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "Containers/Queue.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"

struct FNexusMessage;
class FSocket;
class FNexusRelayConnection
	: public FRunnable
{
public:	
	FNexusRelayConnection(FSocket* InSocket, const FIPv4Endpoint& InRemoteEndpoint);
	virtual ~FNexusRelayConnection();
	void Start();

	bool Recv(FNexusMessage& OutMessage);
	bool Send(FNexusMessage Message);
	// stops the thread and finishes all message transfer before cleaning up
	void EnsureCompletion();
	FIPv4Endpoint RemoteEndpoint;
protected:

	bool ReceiveMessages();
	bool SendMessages();
private:

	//~ FRunnable interface
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;
	
private:
	TQueue<FNexusMessage> RecvQueue;
	TQueue<FNexusMessage> SendQueue;

	
	FSocket* Socket;
	FRunnableThread* Thread;
	bool bRun;
};
