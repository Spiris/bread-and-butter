// Fill out your copyright notice in the Description page of Project Settings.

#include "NexusRelay.h"
#include "IPv4Endpoint.h"
#include "NexusRelayConnection.h"
#include "NexusMessage.h"
#include "WorldNexus.h"
#include "Sockets.h"
#include "Common/TcpListener.h"

FNexusRelay::FNexusRelay()
{
}
FIPv4Endpoint FNexusRelay::GetLocalEndpoint()
{
	if (TcpListener)
	{
		return TcpListener->GetLocalEndpoint();
	}
	return FIPv4Endpoint::Any;
}
FNexusRelay::~FNexusRelay()
{
	CloseRelay();
}
void FNexusRelay::OpenRelay(const FIPv4Endpoint& InListenEndpoint)
{
	if (InListenEndpoint != FIPv4Endpoint::Any)
	{
		TcpListener = new FTcpListener(InListenEndpoint);
		TcpListener->OnConnectionAccepted().BindRaw(this, &FNexusRelay::AcceptIncomingConnection);
		UE_LOG(LogNexus, Log, TEXT("Nexus relay listening for connections on %s"), *InListenEndpoint.ToString());
	}
}
bool FNexusRelay::AcceptIncomingConnection(FSocket* InSocket, const FIPv4Endpoint& Endpoint)
{
	PendingConnections.Enqueue(new FNexusRelayConnection(InSocket, Endpoint));
	return true;
}
void FNexusRelay::AddConnectionAtEndpoint(const FIPv4Endpoint& Endpoint)
{
	FSocket* Socket = FTcpSocketBuilder(TEXT("NexusRelay.Connection"));
	if (Socket)
	{
		if (!Socket->Connect(*Endpoint.ToInternetAddr()))
		{
			ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
		}
		else
		{
			PendingConnections.Enqueue(new FNexusRelayConnection(Socket, Endpoint));
		}
	}
}
void FNexusRelay::CloseRelay()
{
	if (TcpListener)
	{
		TcpListener->Stop();
		delete TcpListener;
		TcpListener = nullptr;
	}
	for (TPair<FString, FNexusRelayConnection*> pair : Connections)
	{
		if (pair.Value)
		{
			pair.Value->EnsureCompletion();
		}
	}
	Connections.Empty();
	PendingConnections.Empty();
}


void FNexusRelay::TestConnections()
{
	TArray<FString> emptyConnections;
	for (TPair<FString, FNexusRelayConnection*> pair : Connections)
	{
		if (pair.Value)
		{
			// TODO
		}
		else
		{
			emptyConnections.Add(pair.Key);
		}
	}
	for (FString empty : emptyConnections)
	{
		Connections.Remove(empty);
	}
}
void FNexusRelay::PollPendingConnections()
{
	if (PendingConnections.IsEmpty())
		return;
	FNexusRelayConnection* newConnection = nullptr;
	while (PendingConnections.Dequeue(newConnection))
	{
		AddNewConnection(newConnection);
	}
}
void FNexusRelay::ReadMessages(UWorldNexus* WorldNexus)
{
	for (TPair<FString, FNexusRelayConnection*> pair : Connections)
	{
		if (pair.Value)
		{
			FNexusMessage message;
			if (pair.Value->Recv(message))
			{
				WorldNexus->ReceiveMessage(message);
			}
		}
	}
}
void FNexusRelay::SendMessage(const FString& InEndpointString, const FNexusMessage& InMessage)
{
	if (FNexusRelayConnection** connectionPtr = Connections.Find(InEndpointString))
	{
		if (FNexusRelayConnection* connection = *connectionPtr)
		{
			connection->Send(InMessage);
		}
	}
}
void FNexusRelay::AddNewConnection(FNexusRelayConnection* InConnection)
{
	FString endpointString = InConnection->RemoteEndpoint.ToString();
	if (!Connections.Contains(endpointString))
	{
		Connections.Add(endpointString, InConnection);
		UE_LOG(LogNexus, Log, TEXT("Added new nexus connection at [%s]"), *endpointString);
	}
	else
	{
		if (FNexusRelayConnection* connection = Connections[endpointString])
		{
			connection->EnsureCompletion();
			connection = nullptr;
		}
		Connections[endpointString] = InConnection;
		UE_LOG(LogNexus, Log, TEXT("Replaced existing connection at [%s]"), *endpointString);
	}
}
void FNexusRelay::RemoveConnection(const FString& InRemoteEndpointString)
{
	if (FNexusRelayConnection** connectionPtr = Connections.Find(InRemoteEndpointString))
	{
		if (FNexusRelayConnection* connection = *connectionPtr)
		{
			connection->EnsureCompletion();
			connection = nullptr;
		}
	}
	Connections.Remove(InRemoteEndpointString);
}