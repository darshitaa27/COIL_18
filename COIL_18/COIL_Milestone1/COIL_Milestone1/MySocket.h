#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

enum SocketType
{
    CLIENT,
    SERVER
};

enum ConnectionType
{
    TCP,
    UDP
};

const int DEFAULT_SIZE = 256;

class MySocket
{
private:
    char* Buffer;
    SOCKET WelcomeSocket;
    SOCKET ConnectionSocket;
    sockaddr_in SvrAddr;
    SocketType mySocket;
    std::string IPAddr;
    int Port;
    ConnectionType connectionType;
    bool bTCPConnect;
    int MaxSize;

public:
    MySocket(SocketType type, std::string ip, unsigned int port, ConnectionType connType, unsigned int size);
    ~MySocket();

    void ConnectTCP();
    void DisconnectTCP();
    void SendData(const char* data, int size);
    int GetData(char* outBuffer);

    std::string GetIPAddr();
    void SetIPAddr(std::string ip);

    void SetPort(int port);
    int GetPort();

    SocketType GetType();
    void SetType(SocketType type);
};