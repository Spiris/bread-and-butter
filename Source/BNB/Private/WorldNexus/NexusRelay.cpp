// Fill out your copyright notice in the Description page of Project Settings.

#include "NexusRelay.h"
#include "IPv4Endpoint.h"
#include "NexusRelayConnection.h"
#include "NexusMessage.h"
#include "WorldNexus.h"

FNexusRelay::FNexusRelay()
{

}
FNexusRelay::~FNexusRelay()
{

}
bool FNexusRelay::Init()
{
	return true;
}
uint32 FNexusRelay::Run()
{
	return 0;
}
void FNexusRelay::Stop()
{

}
void FNexusRelay::EnsureCompletion()
{

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
void FNexusRelay::ReadConnections(UWorldNexus* WorldNexus)
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
void FNexusRelay::AddConnection(FSocket* InSocket, const FIPv4Endpoint& InRemoteEndpoint)
{
	FString endpointString = InRemoteEndpoint.ToString();
	FNexusRelayConnection* newConnection = new FNexusRelayConnection(InSocket, InRemoteEndpoint);
	if (!Connections.Contains(endpointString))
	{
		Connections.Add(endpointString, newConnection);
	}
	else
	{
		if (FNexusRelayConnection* connection = Connections[endpointString])
		{
			connection->EnsureCompletion();
			connection = nullptr;
		}
		Connections[endpointString] = newConnection;
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