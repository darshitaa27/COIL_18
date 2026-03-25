#include "PktDef.h"
#include <cstring>

PktDef::PktDef()
{
    std::memset(&Packet.header, 0, sizeof(Header));
    Packet.Data = nullptr;
    Packet.CRC = 0;
    RawBuffer = nullptr;
}

PktDef::PktDef(char* input)
{
    std::memset(&Packet.header, 0, sizeof(Header));
    Packet.Data = nullptr;
    Packet.CRC = 0;
    RawBuffer = nullptr;

    if (input == nullptr)
        return;

    std::memcpy(&Packet.header, input, HEADERSIZE);

    int bodySize = Packet.header.Length - HEADERSIZE - 1;

    if (bodySize > 0)
    {
        Packet.Data = new char[bodySize];
        std::memcpy(Packet.Data, input + HEADERSIZE, bodySize);
    }

    Packet.CRC = input[Packet.header.Length - 1];

    RawBuffer = new char[Packet.header.Length];
    std::memcpy(RawBuffer, input, Packet.header.Length);
}

PktDef::~PktDef()
{
    delete[] Packet.Data;
    delete[] RawBuffer;
}

void PktDef::SetCmd(CmdType cmd)
{
    Packet.header.Drive = 0;
    Packet.header.Status = 0;
    Packet.header.Sleep = 0;

    if (cmd == DRIVE)
        Packet.header.Drive = 1;
    else if (cmd == SLEEP)
        Packet.header.Sleep = 1;
    else if (cmd == RESPONSE)
        Packet.header.Status = 1;
}

void PktDef::SetBodyData(char* input, int size)
{
    delete[] Packet.Data;
    Packet.Data = nullptr;

    if (input != nullptr && size > 0)
    {
        Packet.Data = new char[size];
        std::memcpy(Packet.Data, input, size);
        Packet.header.Length = HEADERSIZE + size + 1;
    }
    else
    {
        Packet.header.Length = HEADERSIZE + 1;
    }
}

void PktDef::SetPktCount(int count)
{
    Packet.header.PktCount = static_cast<unsigned short>(count);
}

CmdType PktDef::GetCmd()
{
    if (Packet.header.Drive) return DRIVE;
    if (Packet.header.Sleep) return SLEEP;
    return RESPONSE;
}

bool PktDef::GetAck()
{
    return Packet.header.Ack == 1;
}

int PktDef::GetLength()
{
    return Packet.header.Length;
}

char* PktDef::GetBodyData()
{
    return Packet.Data;
}

int PktDef::GetPktCount()
{
    return Packet.header.PktCount;
}

bool PktDef::CheckCRC(char* input, int size)
{
    if (input == nullptr || size <= 0)
        return false;

    unsigned char count = 0;

    for (int i = 0; i < size - 1; i++)
    {
        unsigned char value = static_cast<unsigned char>(input[i]);
        for (int b = 0; b < 8; b++)
        {
            if (value & (1 << b))
                count++;
        }
    }

    return count == static_cast<unsigned char>(input[size - 1]);
}

void PktDef::CalcCRC()
{
    unsigned char count = 0;
    unsigned char* hdr = reinterpret_cast<unsigned char*>(&Packet.header);

    for (int i = 0; i < HEADERSIZE; i++)
    {
        for (int b = 0; b < 8; b++)
        {
            if (hdr[i] & (1 << b))
                count++;
        }
    }

    int bodySize = Packet.header.Length - HEADERSIZE - 1;

    if (Packet.Data != nullptr && bodySize > 0)
    {
        for (int i = 0; i < bodySize; i++)
        {
            unsigned char value = static_cast<unsigned char>(Packet.Data[i]);
            for (int b = 0; b < 8; b++)
            {
                if (value & (1 << b))
                    count++;
            }
        }
    }

    Packet.CRC = static_cast<char>(count);
}

char* PktDef::GenPacket()
{
    delete[] RawBuffer;
    RawBuffer = nullptr;

    if (Packet.header.Length <= 0)
        return nullptr;

    RawBuffer = new char[Packet.header.Length];
    std::memcpy(RawBuffer, &Packet.header, HEADERSIZE);

    int bodySize = Packet.header.Length - HEADERSIZE - 1;

    if (Packet.Data != nullptr && bodySize > 0)
    {
        std::memcpy(RawBuffer + HEADERSIZE, Packet.Data, bodySize);
    }

    CalcCRC();
    RawBuffer[Packet.header.Length - 1] = Packet.CRC;

    return RawBuffer;
}