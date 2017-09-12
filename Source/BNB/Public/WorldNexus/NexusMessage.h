
#pragma once
#include "NexusEnums.h"
struct FNexusMessageHeader
{
public:
	ENexusMessageId Id = ENexusMessageId::None;
	uint16 Major = 0;
	uint16 Minor = 0;
	FNexusMessageHeader() {}
	FNexusMessageHeader(uint16 InMajor, uint16 InMinor) :Major(InMajor), Minor(InMinor) {}
	
};

FORCEINLINE FArchive& operator<<(FArchive& Ar, FNexusMessageHeader& Header)
{
	Ar << Header.Id;
	Ar << Header.Major;
	Ar << Header.Minor;
	return Ar;
}

struct FNexusMessage
{
public:
	FNexusMessageHeader Header;	
	TArray<uint8> Content;
	FString Message;
	FNexusMessage();
	bool Validate(class FSocket* Socket);
};

FORCEINLINE FArchive& operator<<(FArchive& Ar, FNexusMessage& Msg)
{
	Ar << Msg.Header;
	Ar << Msg.Content;
	Ar << Msg.Message;
	return Ar;
}