#pragma once
#include "CoreMinimal.h"

enum class ENexusMessageId : uint16
{
	None,
	Ping,
	End,
};
FORCEINLINE FArchive& operator<<(FArchive& Ar, ENexusMessageId& Id)
{
	uint16 type = (uint16)Id;
	Ar << type;
	Id = (ENexusMessageId)type;
	return Ar;
}
