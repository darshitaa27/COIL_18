#include "MySocket.h"
#include <iostream>
#include <cstring>

MySocket::MySocket(SocketType type, std::string ip, unsigned int port, ConnectionType connType, unsigned int size)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cout << "ERROR: WSAStartup failed.\n";
    }

    mySocket = type;
    IPAddr = ip;

    if (port == 0 || port > 65535)
        Port = DEFAULT_PORT;
    else
        Port = static_cast<int>(port);

    connectionType = connType;
    bTCPConnect = false;

    MaxSize = (size == 0) ? DEFAULT_SIZE : static_cast<int>(size);

    Buffer = new char[MaxSize];
    std::memset(Buffer, 0, MaxSize);

    WelcomeSocket = INVALID_SOCKET;
    ConnectionSocket = INVALID_SOCKET;

    std::memset(&SvrAddr, 0, sizeof(SvrAddr));
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_port = htons(Port);

    if (inet_pton(AF_INET, IPAddr.c_str(), &SvrAddr.sin_addr) <= 0)
    {
        SvrAddr.sin_addr.s_addr = INADDR_ANY;
    }

    std::cout << "[MySocket] Created "
        << (mySocket == SERVER ? "SERVER" : "CLIENT")
        << " on " << IPAddr << ":" << Port
        << " using " << (connectionType == TCP ? "TCP" : "UDP")
        << std::endl;

    if (connectionType == TCP)
    {
        if (mySocket == SERVER)
        {
            WelcomeSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

            if (WelcomeSocket != INVALID_SOCKET)
            {
                if (bind(WelcomeSocket, (sockaddr*)&SvrAddr, sizeof(SvrAddr)) == SOCKET_ERROR)
                {
                    std::cout << "ERROR: TCP server bind failed.\n";
                }
                else if (listen(WelcomeSocket, 1) == SOCKET_ERROR)
                {
                    std::cout << "ERROR: TCP server listen failed.\n";
                }
            }
        }
        else
        {
            ConnectionSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        }
    }
    else
    {
        ConnectionSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        if (mySocket == SERVER && ConnectionSocket != INVALID_SOCKET)
        {
            if (bind(ConnectionSocket, (sockaddr*)&SvrAddr, sizeof(SvrAddr)) == SOCKET_ERROR)
            {
                std::cout << "ERROR: UDP server bind failed.\n";
            }
        }
    }
}

MySocket::~MySocket()
{
    if (ConnectionSocket != INVALID_SOCKET)
    {
        closesocket(ConnectionSocket);
        ConnectionSocket = INVALID_SOCKET;
    }

    if (WelcomeSocket != INVALID_SOCKET)
    {
        closesocket(WelcomeSocket);
        WelcomeSocket = INVALID_SOCKET;
    }

    delete[] Buffer;
    Buffer = nullptr;

    WSACleanup();
}

void MySocket::ConnectTCP()
{
    if (connectionType != TCP)
    {
        std::cout << "ERROR: UDP sockets cannot establish TCP connections.\n";
        return;
    }

    if (bTCPConnect)
        return;

    if (mySocket == CLIENT)
    {
        if (connect(ConnectionSocket, (sockaddr*)&SvrAddr, sizeof(SvrAddr)) == 0)
        {
            bTCPConnect = true;
        }
    }
    else
    {
        int addrLen = sizeof(SvrAddr);
        SOCKET accepted = accept(WelcomeSocket, (sockaddr*)&SvrAddr, &addrLen);

        if (accepted != INVALID_SOCKET)
        {
            ConnectionSocket = accepted;
            bTCPConnect = true;
        }
    }
}

void MySocket::DisconnectTCP()
{
    if (connectionType != TCP)
    {
        std::cout << "ERROR: UDP sockets do not use TCP disconnect.\n";
        return;
    }

    if (ConnectionSocket != INVALID_SOCKET)
    {
        closesocket(ConnectionSocket);
        ConnectionSocket = INVALID_SOCKET;
    }

    bTCPConnect = false;
}

void MySocket::SendData(const char* data, int size)
{
    if (data == nullptr || size <= 0)
        return;

    int sendSize = (size > MaxSize) ? MaxSize : size;

    if (connectionType == TCP)
    {
        if (!bTCPConnect || ConnectionSocket == INVALID_SOCKET)
            return;

        if (send(ConnectionSocket, data, sendSize, 0) == SOCKET_ERROR)
        {
            std::cout << "ERROR: TCP send failed.\n";
        }
    }
    else
    {
        if (ConnectionSocket == INVALID_SOCKET)
            return;

        if (sendto(ConnectionSocket, data, sendSize, 0, (sockaddr*)&SvrAddr, sizeof(SvrAddr)) == SOCKET_ERROR)
        {
            std::cout << "ERROR: UDP send failed.\n";
        }
    }
}

int MySocket::GetData(char* outBuffer)
{
    if (outBuffer == nullptr)
        return 0;

    std::memset(Buffer, 0, MaxSize);
    int bytesReceived = 0;

    if (connectionType == TCP)
    {
        if (!bTCPConnect || ConnectionSocket == INVALID_SOCKET)
            return 0;

        bytesReceived = recv(ConnectionSocket, Buffer, MaxSize, 0);
    }
    else
    {
        if (ConnectionSocket == INVALID_SOCKET)
            return 0;

        int addrLen = sizeof(SvrAddr);
        bytesReceived = recvfrom(ConnectionSocket, Buffer, MaxSize, 0, (sockaddr*)&SvrAddr, &addrLen);
    }

    if (bytesReceived > 0)
    {
        std::memcpy(outBuffer, Buffer, bytesReceived);
        return bytesReceived;
    }

    return 0;
}

std::string MySocket::GetIPAddr()
{
    return IPAddr;
}

void MySocket::SetIPAddr(std::string ip)
{
    if (bTCPConnect || WelcomeSocket != INVALID_SOCKET)
    {
        std::cout << "ERROR: Cannot change IP after TCP connection/open socket.\n";
        return;
    }

    IPAddr = ip;

    if (inet_pton(AF_INET, IPAddr.c_str(), &SvrAddr.sin_addr) <= 0)
    {
        SvrAddr.sin_addr.s_addr = INADDR_ANY;
    }
}

void MySocket::SetPort(int port)
{
    if (bTCPConnect || WelcomeSocket != INVALID_SOCKET)
    {
        std::cout << "ERROR: Cannot change port after TCP connection/open socket.\n";
        return;
    }

    if (port <= 0 || port > 65535)
        return;

    Port = port;
    SvrAddr.sin_port = htons(Port);
}

int MySocket::GetPort()
{
    return Port;
}

SocketType MySocket::GetType()
{
    return mySocket;
}

void MySocket::SetType(SocketType type)
{
    if (bTCPConnect || WelcomeSocket != INVALID_SOCKET)
    {
        std::cout << "ERROR: Cannot change type after TCP connection/open socket.\n";
        return;
    }

    mySocket = type;
}