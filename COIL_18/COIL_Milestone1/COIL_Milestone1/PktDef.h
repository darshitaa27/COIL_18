#pragma once

const unsigned char FORWARD = 1;
const unsigned char BACKWARD = 2;
const unsigned char RIGHT = 3;
const unsigned char LEFT = 4;

const int HEADERSIZE = 4;

enum CmdType
{
    DRIVE,
    SLEEP,
    RESPONSE
};

#pragma pack(push, 1)
struct Header
{
    unsigned short PktCount;

    unsigned char Drive : 1;
    unsigned char Status : 1;
    unsigned char Sleep : 1;
    unsigned char Ack : 1;
    unsigned char Padding : 4;

    unsigned char Length;
};
#pragma pack(pop)

struct DriveBody
{
    unsigned char Direction;
    unsigned char Duration;
    unsigned char Power;
};

struct TurnBody
{
    unsigned char Direction;
    unsigned short Duration;
};

class PktDef
{
private:
    struct CmdPacket
    {
        Header header;
        char* Data;
        char CRC;
    };

    CmdPacket Packet;
    char* RawBuffer;

public:
    PktDef();
    PktDef(char*);
    ~PktDef();

    void SetCmd(CmdType);
    void SetBodyData(char*, int);
    void SetPktCount(int);

    CmdType GetCmd();
    bool GetAck();
    int GetLength();
    char* GetBodyData();
    int GetPktCount();

    bool CheckCRC(char*, int);
    void CalcCRC();
    char* GenPacket();
};