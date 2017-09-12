// Fill out your copyright notice in the Description page of Project Settings.

#include "NexusRelayConnection.h"
#include "Sockets.h"
#include "NexusMessage.h"
#include "BufferArchive.h"

FNexusRelayConnection::FNexusRelayConnection(FSocket* InSocket, const FIPv4Endpoint& InRemoteEndpoint)
	: RemoteEndpoint(InRemoteEndpoint)
	, Socket(InSocket)
	, Thread(nullptr)
	, bRun(false)
{
	int32 NewSize = 0;
	Socket->SetReceiveBufferSize(2 * 1024 * 1024, NewSize);
	Socket->SetSendBufferSize(2 * 1024 * 1024, NewSize);
}

FNexusRelayConnection::~FNexusRelayConnection()
{
	if (Thread != nullptr)
	{
		if (bRun)
		{
			bRun = false;
			Thread->WaitForCompletion();
		}
		delete Thread;
	}

	if (Socket)
	{
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
	}
}

/* FTcpMessageTransportConnection interface
*****************************************************************************/

void FNexusRelayConnection::Start()
{
	check(Thread == nullptr);
	bRun = true;
	Thread = FRunnableThread::Create(this, TEXT("NexusConnection"), 128 * 1024, TPri_Normal);
}

bool FNexusRelayConnection::Recv(FNexusMessage& OutMessage)
{
	if (RecvQueue.Dequeue(OutMessage))
	{
		return true;
	}
	return false;
}


bool FNexusRelayConnection::Send(FNexusMessage Message)
{
	return SendQueue.Enqueue(Message);
}


/* FRunnable interface
*****************************************************************************/

bool FNexusRelayConnection::Init()
{
	return (Socket != nullptr);
}

uint32 FNexusRelayConnection::Run()
{
	while (bRun)
	{
		// Try sending and receiving messages and detect if they fail or if another connection error is reported.
		if ((!ReceiveMessages() || !SendMessages() || Socket->GetConnectionState() == SCS_ConnectionError) && bRun)
		{

		}

		FPlatformProcess::Sleep(0.0f);
	}

	return 0;
}

void FNexusRelayConnection::Stop()
{
	if (Socket)
	{
		Socket->Close();
	}
}

void FNexusRelayConnection::Exit() { }


/* FTcpMessageTransportConnection implementation
*****************************************************************************/
void FNexusRelayConnection::EnsureCompletion()
{
	if (Thread)
	{
		bRun = false;
		Thread->WaitForCompletion();
		delete Thread;
		Thread = nullptr;
	}
	if (Socket)
	{
		Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
		Socket = nullptr;
	}
	SendQueue.Empty();
	RecvQueue.Empty();
}
bool FNexusRelayConnection::ReceiveMessages()
{	
	Socket->Wait(ESocketWaitConditions::WaitForRead, FTimespan::FromMilliseconds(200));

	uint32 dataSize = 0;
	if (Socket->HasPendingData(dataSize) && dataSize > 9)
	{
		FNexusMessage message;
		if (message.Validate(Socket))
		{
			RecvQueue.Enqueue(message);
		}
	}
	return true;
}

bool FNexusRelayConnection::SendMessages()
{
	if (SendQueue.IsEmpty())
		return true;
	FNexusMessage message;
	while (SendQueue.Dequeue(message))
	{
		int32 BytesSent = 0;
		FBufferArchive buff;
		buff << message;
		int32 sent = 0;
		if (Socket->Send(buff.GetData(), buff.Num(), sent))
		{
			if (sent != buff.Num())
			{
				UE_LOG(LogTemp, Error, TEXT("Send failed. Sent %d of %d bytes on socket."), sent, buff.Num());
				return false;
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Send failed on socket."));
			return false;
		}
	}
	return true;
}

