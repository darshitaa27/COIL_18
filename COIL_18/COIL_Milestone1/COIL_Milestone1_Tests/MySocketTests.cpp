#include "pch.h"
#include "CppUnitTest.h"
#include "../COIL_Milestone1/MySocket.h"

#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <cstring>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace
{
    const std::string LOOPBACK = "127.0.0.1";
    const int DEFAULT_EXPECTED_SIZE = 256;

    void SmallDelay()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::string MakePayload(char value, int size)
    {
        return std::string(size, value);
    }

    void ConnectTcpPair(MySocket& server, MySocket& client)
    {
        std::thread serverThread([&server]()
            {
                server.ConnectTCP();
            });

        SmallDelay();
        client.ConnectTCP();
        serverThread.join();
        SmallDelay();
    }

    int ReceiveString(MySocket& socketObj, std::string& out, int bufferSize = 1024)
    {
        std::vector<char> buffer(bufferSize, 0);
        int received = socketObj.GetData(buffer.data());

        if (received > 0)
        {
            out.assign(buffer.data(), received);
        }
        else
        {
            out.clear();
        }

        return received;
    }
}

namespace MySocketTests
{
    TEST_CLASS(MySocketTests)
    {
    public:

        TEST_METHOD(Constructor_TCPClient_StoresIPAddress)
        {
            MySocket sock(CLIENT, LOOPBACK, 56001, TCP, 128);
            Assert::IsTrue(sock.GetIPAddr() == LOOPBACK);
        }

        TEST_METHOD(Constructor_TCPClient_StoresPort)
        {
            MySocket sock(CLIENT, LOOPBACK, 56002, TCP, 128);
            Assert::AreEqual(56002, sock.GetPort());
        }

        TEST_METHOD(Constructor_TCPClient_StoresType)
        {
            MySocket sock(CLIENT, LOOPBACK, 56003, TCP, 128);
            Assert::AreEqual((int)CLIENT, (int)sock.GetType());
        }

        TEST_METHOD(Constructor_TCPServer_StoresIPAddress)
        {
            MySocket sock(SERVER, LOOPBACK, 56004, TCP, 128);
            Assert::IsTrue(sock.GetIPAddr() == LOOPBACK);
        }

        TEST_METHOD(Constructor_TCPServer_StoresPort)
        {
            MySocket sock(SERVER, LOOPBACK, 56005, TCP, 128);
            Assert::AreEqual(56005, sock.GetPort());
        }

        TEST_METHOD(Constructor_TCPServer_StoresType)
        {
            MySocket sock(SERVER, LOOPBACK, 56006, TCP, 128);
            Assert::AreEqual((int)SERVER, (int)sock.GetType());
        }

        TEST_METHOD(Constructor_UDPClient_StoresIPAddress)
        {
            MySocket sock(CLIENT, LOOPBACK, 56007, UDP, 128);
            Assert::IsTrue(sock.GetIPAddr() == LOOPBACK);
        }

        TEST_METHOD(Constructor_UDPClient_StoresPort)
        {
            MySocket sock(CLIENT, LOOPBACK, 56008, UDP, 128);
            Assert::AreEqual(56008, sock.GetPort());
        }

        TEST_METHOD(Constructor_UDPClient_StoresType)
        {
            MySocket sock(CLIENT, LOOPBACK, 56009, UDP, 128);
            Assert::AreEqual((int)CLIENT, (int)sock.GetType());
        }

        TEST_METHOD(Constructor_UDPServer_StoresIPAddress)
        {
            MySocket sock(SERVER, LOOPBACK, 56010, UDP, 128);
            Assert::IsTrue(sock.GetIPAddr() == LOOPBACK);
        }

        TEST_METHOD(Constructor_UDPServer_StoresPort)
        {
            MySocket sock(SERVER, LOOPBACK, 56011, UDP, 128);
            Assert::AreEqual(56011, sock.GetPort());
        }

        TEST_METHOD(Constructor_UDPServer_StoresType)
        {
            MySocket sock(SERVER, LOOPBACK, 56012, UDP, 128);
            Assert::AreEqual((int)SERVER, (int)sock.GetType());
        }

        TEST_METHOD(Constructor_ZeroPort_FallsBackToDefaultPort)
        {
            MySocket sock(CLIENT, LOOPBACK, 0, TCP, 128);
            Assert::AreEqual(54000, sock.GetPort());
        }

        TEST_METHOD(Constructor_InvalidLargePort_FallsBackToDefaultPort)
        {
            MySocket sock(CLIENT, LOOPBACK, 70000, TCP, 128);
            Assert::AreEqual(54000, sock.GetPort());
        }

        TEST_METHOD(SetIPAddr_TCPClientBeforeConnect_UpdatesValue)
        {
            MySocket sock(CLIENT, LOOPBACK, 56013, TCP, 128);
            sock.SetIPAddr("127.0.0.2");
            Assert::IsTrue(sock.GetIPAddr() == "127.0.0.2");
        }

        TEST_METHOD(SetPort_TCPClientBeforeConnect_UpdatesValue)
        {
            MySocket sock(CLIENT, LOOPBACK, 56014, TCP, 128);
            sock.SetPort(56114);
            Assert::AreEqual(56114, sock.GetPort());
        }

        TEST_METHOD(SetType_TCPClientBeforeConnect_UpdatesValue)
        {
            MySocket sock(CLIENT, LOOPBACK, 56015, TCP, 128);
            sock.SetType(SERVER);
            Assert::AreEqual((int)SERVER, (int)sock.GetType());
        }

        TEST_METHOD(SetIPAddr_UDPClientBeforeUse_UpdatesValue)
        {
            MySocket sock(CLIENT, LOOPBACK, 56016, UDP, 128);
            sock.SetIPAddr("127.0.0.2");
            Assert::IsTrue(sock.GetIPAddr() == "127.0.0.2");
        }

        TEST_METHOD(SetPort_UDPClientBeforeUse_UpdatesValue)
        {
            MySocket sock(CLIENT, LOOPBACK, 56017, UDP, 128);
            sock.SetPort(56117);
            Assert::AreEqual(56117, sock.GetPort());
        }

        TEST_METHOD(SetType_UDPClientBeforeUse_UpdatesValue)
        {
            MySocket sock(CLIENT, LOOPBACK, 56018, UDP, 128);
            sock.SetType(SERVER);
            Assert::AreEqual((int)SERVER, (int)sock.GetType());
        }

        TEST_METHOD(SetIPAddr_MultipleChanges_KeepsLatestValue)
        {
            MySocket sock(CLIENT, LOOPBACK, 56019, UDP, 128);
            sock.SetIPAddr("127.0.0.2");
            sock.SetIPAddr("127.0.0.3");
            Assert::IsTrue(sock.GetIPAddr() == "127.0.0.3");
        }

        TEST_METHOD(SetPort_MultipleChanges_KeepsLatestValue)
        {
            MySocket sock(CLIENT, LOOPBACK, 56020, UDP, 128);
            sock.SetPort(56120);
            sock.SetPort(56220);
            Assert::AreEqual(56220, sock.GetPort());
        }

        TEST_METHOD(SetPort_InvalidValue_IsIgnored)
        {
            MySocket sock(CLIENT, LOOPBACK, 56021, UDP, 128);
            sock.SetPort(-1);
            Assert::AreEqual(56021, sock.GetPort());
        }

        TEST_METHOD(SetType_MultipleChanges_KeepsLatestValue)
        {
            MySocket sock(CLIENT, LOOPBACK, 56022, UDP, 128);
            sock.SetType(SERVER);
            sock.SetType(CLIENT);
            Assert::AreEqual((int)CLIENT, (int)sock.GetType());
        }

        TEST_METHOD(TCPServer_SetIPAddr_IsBlockedAfterConstructor)
        {
            MySocket sock(SERVER, LOOPBACK, 56023, TCP, 128);
            sock.SetIPAddr("127.0.0.2");
            Assert::IsTrue(sock.GetIPAddr() == LOOPBACK);
        }

        TEST_METHOD(TCPServer_SetPort_IsBlockedAfterConstructor)
        {
            MySocket sock(SERVER, LOOPBACK, 56024, TCP, 128);
            sock.SetPort(56124);
            Assert::AreEqual(56024, sock.GetPort());
        }

        TEST_METHOD(TCPServer_SetType_IsBlockedAfterConstructor)
        {
            MySocket sock(SERVER, LOOPBACK, 56025, TCP, 128);
            sock.SetType(CLIENT);
            Assert::AreEqual((int)SERVER, (int)sock.GetType());
        }

        TEST_METHOD(UDPClient_ConnectTCP_DoesNotBreakSetterAccess)
        {
            MySocket sock(CLIENT, LOOPBACK, 56026, UDP, 128);
            sock.ConnectTCP();
            sock.SetPort(56126);
            Assert::AreEqual(56126, sock.GetPort());
        }

        TEST_METHOD(UDPClient_DisconnectTCP_DoesNotBreakSetterAccess)
        {
            MySocket sock(CLIENT, LOOPBACK, 56027, UDP, 128);
            sock.DisconnectTCP();
            sock.SetIPAddr("127.0.0.2");
            Assert::IsTrue(sock.GetIPAddr() == "127.0.0.2");
        }

        TEST_METHOD(UDPServer_SetPort_RemainsAllowed)
        {
            MySocket sock(SERVER, LOOPBACK, 56028, UDP, 128);
            sock.SetPort(56128);
            Assert::AreEqual(56128, sock.GetPort());
        }

        TEST_METHOD(UDPServer_SetIPAddr_RemainsAllowed)
        {
            MySocket sock(SERVER, LOOPBACK, 56029, UDP, 128);
            sock.SetIPAddr("127.0.0.2");
            Assert::IsTrue(sock.GetIPAddr() == "127.0.0.2");
        }

        TEST_METHOD(UDPServer_SetType_RemainsAllowed)
        {
            MySocket sock(SERVER, LOOPBACK, 56030, UDP, 128);
            sock.SetType(CLIENT);
            Assert::AreEqual((int)CLIENT, (int)sock.GetType());
        }

        TEST_METHOD(TCPClient_SetIPAddr_IsBlockedAfterConnect)
        {
            MySocket server(SERVER, LOOPBACK, 56031, TCP, 128);
            MySocket client(CLIENT, LOOPBACK, 56031, TCP, 128);

            ConnectTcpPair(server, client);

            client.SetIPAddr("127.0.0.2");
            Assert::IsTrue(client.GetIPAddr() == LOOPBACK);
        }

        TEST_METHOD(TCPClient_SetPort_IsBlockedAfterConnect)
        {
            MySocket server(SERVER, LOOPBACK, 56032, TCP, 128);
            MySocket client(CLIENT, LOOPBACK, 56032, TCP, 128);

            ConnectTcpPair(server, client);

            client.SetPort(56132);
            Assert::AreEqual(56032, client.GetPort());
        }

        TEST_METHOD(TCPClient_SetType_IsBlockedAfterConnect)
        {
            MySocket server(SERVER, LOOPBACK, 56033, TCP, 128);
            MySocket client(CLIENT, LOOPBACK, 56033, TCP, 128);

            ConnectTcpPair(server, client);

            client.SetType(SERVER);
            Assert::AreEqual((int)CLIENT, (int)client.GetType());
        }

        TEST_METHOD(TCPClient_Disconnect_AllowsPortChangeAgain)
        {
            MySocket server(SERVER, LOOPBACK, 56034, TCP, 128);
            MySocket client(CLIENT, LOOPBACK, 56034, TCP, 128);

            ConnectTcpPair(server, client);
            client.DisconnectTCP();
            client.SetPort(56134);

            Assert::AreEqual(56134, client.GetPort());
        }

        TEST_METHOD(TCPClient_Disconnect_AllowsIPChangeAgain)
        {
            MySocket server(SERVER, LOOPBACK, 56035, TCP, 128);
            MySocket client(CLIENT, LOOPBACK, 56035, TCP, 128);

            ConnectTcpPair(server, client);
            client.DisconnectTCP();
            client.SetIPAddr("127.0.0.2");

            Assert::IsTrue(client.GetIPAddr() == "127.0.0.2");
        }

        TEST_METHOD(TCPClient_GetData_WithNullBuffer_ReturnsZero)
        {
            MySocket client(CLIENT, LOOPBACK, 56036, TCP, 128);
            Assert::AreEqual(0, client.GetData(nullptr));
        }

        TEST_METHOD(UDPClient_GetData_WithNullBuffer_ReturnsZero)
        {
            MySocket client(CLIENT, LOOPBACK, 56037, UDP, 128);
            Assert::AreEqual(0, client.GetData(nullptr));
        }

        TEST_METHOD(TCPClient_SendDataToServer_TransfersCorrectBytes)
        {
            MySocket server(SERVER, LOOPBACK, 56038, TCP, 128);
            MySocket client(CLIENT, LOOPBACK, 56038, TCP, 128);

            ConnectTcpPair(server, client);

            std::string payload = "HELLO_TCP";
            client.SendData(payload.c_str(), (int)payload.size());

            std::string received;
            int count = ReceiveString(server, received);

            Assert::AreEqual((int)payload.size(), count);
            Assert::IsTrue(received == payload);
        }

        TEST_METHOD(TCPServer_SendDataToClient_TransfersCorrectBytes)
        {
            MySocket server(SERVER, LOOPBACK, 56039, TCP, 128);
            MySocket client(CLIENT, LOOPBACK, 56039, TCP, 128);

            ConnectTcpPair(server, client);

            std::string payload = "SERVER_TO_CLIENT";
            server.SendData(payload.c_str(), (int)payload.size());

            std::string received;
            int count = ReceiveString(client, received);

            Assert::AreEqual((int)payload.size(), count);
            Assert::IsTrue(received == payload);
        }

        TEST_METHOD(TCPClient_SendData_NullPointer_IsSafeAndNextSendWorks)
        {
            MySocket server(SERVER, LOOPBACK, 56040, TCP, 128);
            MySocket client(CLIENT, LOOPBACK, 56040, TCP, 128);

            ConnectTcpPair(server, client);

            client.SendData(nullptr, 10);

            std::string payload = "VALID_AFTER_NULL";
            client.SendData(payload.c_str(), (int)payload.size());

            std::string received;
            int count = ReceiveString(server, received);

            Assert::AreEqual((int)payload.size(), count);
            Assert::IsTrue(received == payload);
        }

        TEST_METHOD(TCPClient_SendData_ZeroSize_IsSafeAndNextSendWorks)
        {
            MySocket server(SERVER, LOOPBACK, 56041, TCP, 128);
            MySocket client(CLIENT, LOOPBACK, 56041, TCP, 128);

            ConnectTcpPair(server, client);

            std::string ignored = "IGNORE";
            client.SendData(ignored.c_str(), 0);

            std::string payload = "VALID_AFTER_ZERO";
            client.SendData(payload.c_str(), (int)payload.size());

            std::string received;
            int count = ReceiveString(server, received);

            Assert::AreEqual((int)payload.size(), count);
            Assert::IsTrue(received == payload);
        }

        TEST_METHOD(TCPClient_SendData_NegativeSize_IsSafeAndNextSendWorks)
        {
            MySocket server(SERVER, LOOPBACK, 56042, TCP, 128);
            MySocket client(CLIENT, LOOPBACK, 56042, TCP, 128);

            ConnectTcpPair(server, client);

            std::string ignored = "IGNORE";
            client.SendData(ignored.c_str(), -5);

            std::string payload = "VALID_AFTER_NEGATIVE";
            client.SendData(payload.c_str(), (int)payload.size());

            std::string received;
            int count = ReceiveString(server, received);

            Assert::AreEqual((int)payload.size(), count);
            Assert::IsTrue(received == payload);
        }

        TEST_METHOD(TCPClient_DefaultSize_ClipsLargeSendTo256Bytes)
        {
            MySocket server(SERVER, LOOPBACK, 56043, TCP, 0);
            MySocket client(CLIENT, LOOPBACK, 56043, TCP, 0);

            ConnectTcpPair(server, client);

            std::string payload = MakePayload('A', 400);
            client.SendData(payload.c_str(), (int)payload.size());

            std::string received;
            int count = ReceiveString(server, received, 1024);

            Assert::AreEqual(DEFAULT_EXPECTED_SIZE, count);
            Assert::AreEqual(DEFAULT_EXPECTED_SIZE, (int)received.size());
        }

        TEST_METHOD(TCPClient_CustomSmallSize_ClipsSendToConfiguredMaxSize)
        {
            MySocket server(SERVER, LOOPBACK, 56044, TCP, 128);
            MySocket client(CLIENT, LOOPBACK, 56044, TCP, 32);

            ConnectTcpPair(server, client);

            std::string payload = MakePayload('B', 100);
            client.SendData(payload.c_str(), (int)payload.size());

            std::string received;
            int count = ReceiveString(server, received, 1024);

            Assert::AreEqual(32, count);
            Assert::AreEqual(32, (int)received.size());
        }

        TEST_METHOD(TCPConnect_CalledTwice_RemainsUsable)
        {
            MySocket server(SERVER, LOOPBACK, 56045, TCP, 128);
            MySocket client(CLIENT, LOOPBACK, 56045, TCP, 128);

            ConnectTcpPair(server, client);
            client.ConnectTCP();

            std::string payload = "DOUBLE_CONNECT_OK";
            client.SendData(payload.c_str(), (int)payload.size());

            std::string received;
            int count = ReceiveString(server, received);

            Assert::AreEqual((int)payload.size(), count);
            Assert::IsTrue(received == payload);
        }

        TEST_METHOD(TCPDisconnect_WithoutConnect_IsSafe)
        {
            MySocket client(CLIENT, LOOPBACK, 56046, TCP, 128);
            client.DisconnectTCP();
            client.SetPort(56146);
            Assert::AreEqual(56146, client.GetPort());
        }

        TEST_METHOD(UDPClient_SendDataToServer_TransfersCorrectBytes)
        {
            MySocket server(SERVER, LOOPBACK, 56047, UDP, 128);
            MySocket client(CLIENT, LOOPBACK, 56047, UDP, 128);

            std::string payload = "HELLO_UDP";
            client.SendData(payload.c_str(), (int)payload.size());

            std::string received;
            int count = ReceiveString(server, received);

            Assert::AreEqual((int)payload.size(), count);
            Assert::IsTrue(received == payload);
        }

        TEST_METHOD(UDPServer_SendDataToClient_AfterReceive_TransfersCorrectBytes)
        {
            MySocket server(SERVER, LOOPBACK, 56048, UDP, 128);
            MySocket client(CLIENT, LOOPBACK, 56048, UDP, 128);

            std::string first = "CLIENT_FIRST";
            client.SendData(first.c_str(), (int)first.size());

            std::string serverReceived;
            int firstCount = ReceiveString(server, serverReceived);

            Assert::AreEqual((int)first.size(), firstCount);
            Assert::IsTrue(serverReceived == first);

            std::string reply = "SERVER_REPLY";
            server.SendData(reply.c_str(), (int)reply.size());

            std::string clientReceived;
            int replyCount = ReceiveString(client, clientReceived);

            Assert::AreEqual((int)reply.size(), replyCount);
            Assert::IsTrue(clientReceived == reply);
        }

        TEST_METHOD(UDPClient_SendData_NullPointer_IsSafeAndNextSendWorks)
        {
            MySocket server(SERVER, LOOPBACK, 56049, UDP, 128);
            MySocket client(CLIENT, LOOPBACK, 56049, UDP, 128);

            client.SendData(nullptr, 10);

            std::string payload = "VALID_UDP_AFTER_NULL";
            client.SendData(payload.c_str(), (int)payload.size());

            std::string received;
            int count = ReceiveString(server, received);

            Assert::AreEqual((int)payload.size(), count);
            Assert::IsTrue(received == payload);
        }

        TEST_METHOD(UDPClient_SendData_ZeroSize_IsSafeAndNextSendWorks)
        {
            MySocket server(SERVER, LOOPBACK, 56050, UDP, 128);
            MySocket client(CLIENT, LOOPBACK, 56050, UDP, 128);

            std::string ignored = "IGNORE";
            client.SendData(ignored.c_str(), 0);

            std::string payload = "VALID_UDP_AFTER_ZERO";
            client.SendData(payload.c_str(), (int)payload.size());

            std::string received;
            int count = ReceiveString(server, received);

            Assert::AreEqual((int)payload.size(), count);
            Assert::IsTrue(received == payload);
        }

        TEST_METHOD(UDPClient_SendData_NegativeSize_IsSafeAndNextSendWorks)
        {
            MySocket server(SERVER, LOOPBACK, 56051, UDP, 128);
            MySocket client(CLIENT, LOOPBACK, 56051, UDP, 128);

            std::string ignored = "IGNORE";
            client.SendData(ignored.c_str(), -10);

            std::string payload = "VALID_UDP_AFTER_NEGATIVE";
            client.SendData(payload.c_str(), (int)payload.size());

            std::string received;
            int count = ReceiveString(server, received);

            Assert::AreEqual((int)payload.size(), count);
            Assert::IsTrue(received == payload);
        }

        TEST_METHOD(UDPClient_DefaultSize_ClipsLargeSendTo256Bytes)
        {
            MySocket server(SERVER, LOOPBACK, 56052, UDP, 1024);
            MySocket client(CLIENT, LOOPBACK, 56052, UDP, 0);

            std::string payload = MakePayload('U', 400);
            client.SendData(payload.c_str(), (int)payload.size());

            std::string received;
            int count = ReceiveString(server, received, 1024);

            Assert::AreEqual(DEFAULT_EXPECTED_SIZE, count);
            Assert::AreEqual(DEFAULT_EXPECTED_SIZE, (int)received.size());
        }

        TEST_METHOD(UDPClient_CustomSmallSize_ClipsSendToConfiguredMaxSize)
        {
            MySocket server(SERVER, LOOPBACK, 56053, UDP, 1024);
            MySocket client(CLIENT, LOOPBACK, 56053, UDP, 32);

            std::string payload = MakePayload('V', 100);
            client.SendData(payload.c_str(), (int)payload.size());

            std::string received;
            int count = ReceiveString(server, received, 1024);

            Assert::AreEqual(32, count);
            Assert::AreEqual(32, (int)received.size());
        }

        TEST_METHOD(UDPServer_ReceivesMultipleMessagesSequentially)
        {
            MySocket server(SERVER, LOOPBACK, 56054, UDP, 128);
            MySocket client(CLIENT, LOOPBACK, 56054, UDP, 128);

            std::string first = "MSG_ONE";
            std::string second = "MSG_TWO";

            client.SendData(first.c_str(), (int)first.size());

            std::string recv1;
            int count1 = ReceiveString(server, recv1);

            client.SendData(second.c_str(), (int)second.size());

            std::string recv2;
            int count2 = ReceiveString(server, recv2);

            Assert::AreEqual((int)first.size(), count1);
            Assert::AreEqual((int)second.size(), count2);
            Assert::IsTrue(recv1 == first);
            Assert::IsTrue(recv2 == second);
        }

        TEST_METHOD(TCPServer_ReceivesMultipleMessagesSequentially)
        {
            MySocket server(SERVER, LOOPBACK, 56055, TCP, 128);
            MySocket client(CLIENT, LOOPBACK, 56055, TCP, 128);

            ConnectTcpPair(server, client);

            std::string first = "TCP_ONE";
            std::string second = "TCP_TWO";

            client.SendData(first.c_str(), (int)first.size());

            std::string recv1;
            int count1 = ReceiveString(server, recv1);

            client.SendData(second.c_str(), (int)second.size());

            std::string recv2;
            int count2 = ReceiveString(server, recv2);

            Assert::AreEqual((int)first.size(), count1);
            Assert::AreEqual((int)second.size(), count2);
            Assert::IsTrue(recv1 == first);
            Assert::IsTrue(recv2 == second);
        }

        TEST_METHOD(TCPServer_CanReplyAfterReceivingClientMessage)
        {
            MySocket server(SERVER, LOOPBACK, 56056, TCP, 128);
            MySocket client(CLIENT, LOOPBACK, 56056, TCP, 128);

            ConnectTcpPair(server, client);

            std::string first = "REQ";
            client.SendData(first.c_str(), (int)first.size());

            std::string serverReceived;
            int requestCount = ReceiveString(server, serverReceived);

            Assert::AreEqual((int)first.size(), requestCount);
            Assert::IsTrue(serverReceived == first);

            std::string reply = "ACK";
            server.SendData(reply.c_str(), (int)reply.size());

            std::string clientReceived;
            int replyCount = ReceiveString(client, clientReceived);

            Assert::AreEqual((int)reply.size(), replyCount);
            Assert::IsTrue(clientReceived == reply);
        }
    };
}