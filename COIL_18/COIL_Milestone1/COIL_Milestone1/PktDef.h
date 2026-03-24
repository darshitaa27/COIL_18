#pragma once

// Direction constants
const unsigned char FORWARD = 1;
const unsigned char BACKWARD = 2;
const unsigned char RIGHT = 3;
const unsigned char LEFT = 4;

// Header size: PktCount(2) + flags(1) + Length(1)
const int HEADERSIZE = 4;

// Command type enum
enum CmdType
{
    DRIVE,
    SLEEP,
    RESPONSE
};

// Packet header
struct Header
{
    unsigned short pktCount;   // 2 bytes

    // flags packed into 1 byte logically
    unsigned char Drive : 1;
    unsigned char Status : 1;
    unsigned char Sleep : 1;
    unsigned char Ack : 1;
    unsigned char Padding : 4;

    unsigned char Length;      // total packet length
};

// Drive body for forward/backward
struct DriveBody
{
    unsigned char Direction;
    unsigned char Duration;
    unsigned char Power;
};

// Turn body for left/right
struct TurnBody
{
    unsigned char Direction;
    unsigned char Duration;
};

class PktDef
{
private:
    Header header;
    char* Data;
    unsigned char CRC;
    char* RawBuffer;

public:
    PktDef();
    PktDef(char* rawData);
    ~PktDef();

    void SetCmd(CmdType cmd);
    void SetBodyData(char* data, int size);
    void SetPktCount(int count);

    CmdType GetCmd() const;
    bool GetAck() const;
    int GetLength() const;
    char* GetBodyData() const;
    int GetPktCount() const;

    bool CheckCRC(char* buffer, int size);
    unsigned char CalcCRC();
    char* GenPacket();
};