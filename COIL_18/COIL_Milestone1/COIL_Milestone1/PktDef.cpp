#include "PktDef.h"
#include <cstring>

// Default constructor
PktDef::PktDef()
{
    std::memset(&header, 0, sizeof(header));
    Data = nullptr;
    CRC = 0;
    RawBuffer = nullptr;
}

// Raw packet constructor
PktDef::PktDef(char* rawData)
{
    std::memset(&header, 0, sizeof(header));
    Data = nullptr;
    CRC = 0;
    RawBuffer = nullptr;

    // We will implement this properly later
}

// Destructor
PktDef::~PktDef()
{
    delete[] Data;
    delete[] RawBuffer;
}

void PktDef::SetCmd(CmdType cmd)
{
    header.Drive = 0;
    header.Status = 0;
    header.Sleep = 0;

    switch (cmd)
    {
    case DRIVE:
        header.Drive = 1;
        break;
    case SLEEP:
        header.Sleep = 1;
        break;
    case RESPONSE:
        header.Status = 1;
        break;
    }
}

void PktDef::SetBodyData(char* data, int size)
{
    delete[] Data;
    Data = new char[size];
    std::memcpy(Data, data, size);

    header.Length = HEADERSIZE + size + 1; // +1 for CRC
}

void PktDef::SetPktCount(int count)
{
    header.pktCount = static_cast<unsigned short>(count);
}

CmdType PktDef::GetCmd() const
{
    if (header.Drive) return DRIVE;
    if (header.Sleep) return SLEEP;
    return RESPONSE;
}

bool PktDef::GetAck() const
{
    return header.Ack == 1;
}

int PktDef::GetLength() const
{
    return header.Length;
}

char* PktDef::GetBodyData() const
{
    return Data;
}

int PktDef::GetPktCount() const
{
    return header.pktCount;
}

bool PktDef::CheckCRC(char* buffer, int size)
{
    if (size <= 0) return false;

    unsigned char count = 0;

    for (int i = 0; i < size - 1; i++)
    {
        unsigned char byte = static_cast<unsigned char>(buffer[i]);
        for (int b = 0; b < 8; b++)
        {
            if (byte & (1 << b))
                count++;
        }
    }

    return count == static_cast<unsigned char>(buffer[size - 1]);
}

unsigned char PktDef::CalcCRC()
{
    unsigned char count = 0;

    // count bits in header bytes
    unsigned char* hdr = reinterpret_cast<unsigned char*>(&header);
    for (int i = 0; i < sizeof(Header); i++)
    {
        for (int b = 0; b < 8; b++)
        {
            if (hdr[i] & (1 << b))
                count++;
        }
    }

    // count bits in data bytes
    int bodySize = header.Length - HEADERSIZE - 1;
    if (Data != nullptr && bodySize > 0)
    {
        for (int i = 0; i < bodySize; i++)
        {
            unsigned char byte = static_cast<unsigned char>(Data[i]);
            for (int b = 0; b < 8; b++)
            {
                if (byte & (1 << b))
                    count++;
            }
        }
    }

    CRC = count;
    return CRC;
}

char* PktDef::GenPacket()
{
    delete[] RawBuffer;

    int totalLength = header.Length;
    RawBuffer = new char[totalLength];

    std::memcpy(RawBuffer, &header, HEADERSIZE);

    int bodySize = totalLength - HEADERSIZE - 1;
    if (Data != nullptr && bodySize > 0)
    {
        std::memcpy(RawBuffer + HEADERSIZE, Data, bodySize);
    }

    RawBuffer[totalLength - 1] = static_cast<char>(CalcCRC());

    return RawBuffer;
}