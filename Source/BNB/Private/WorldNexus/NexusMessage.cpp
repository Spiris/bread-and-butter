
#include "NexusMessage.h"
#include "Sockets.h"
#include "BufferReader.h"
FNexusMessage::FNexusMessage() : 
	Header(),
	Content(),
	Message()
{

}

bool FNexusMessage::Validate(FSocket* Socket)
{
	int32 HeaderSize = sizeof(FNexusMessageHeader) + 4;
	uint8* headerBytes = new uint8[HeaderSize];
	int32 read = 0;
	Socket->Recv(headerBytes, HeaderSize, read, ESocketReceiveFlags::WaitAll);
	FBufferReader reader = FBufferReader(headerBytes, HeaderSize, true);
	reader << Header;
	int32 packetSize = 0;
	reader << packetSize;
	delete[] headerBytes;
	reader.Close();
	if (read == HeaderSize && packetSize > 0 && packetSize < 2048)
	{
		uint8* bodyBytes = new uint8[packetSize];
		Socket->Recv(bodyBytes, packetSize, read, ESocketReceiveFlags::WaitAll);
		reader = FBufferReader(bodyBytes, packetSize, true);
		reader << Content;
		reader << Message;
		delete[] bodyBytes;
		reader.Close();
	}
	return false;
}
