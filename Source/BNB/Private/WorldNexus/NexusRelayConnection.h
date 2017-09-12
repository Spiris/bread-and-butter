// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runnable.h"
#include "Queue.h"
#include "IPv4Endpoint.h"

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

	FIPv4Endpoint RemoteEndpoint;
	FSocket* Socket;
	FRunnableThread* Thread;
	bool bRun;
};
