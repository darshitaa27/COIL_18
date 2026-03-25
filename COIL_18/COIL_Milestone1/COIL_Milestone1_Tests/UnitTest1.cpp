#include "pch.h"
#include "CppUnitTest.h"
#include "../COIL_Milestone1/PktDef.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace COILMilestone1Tests
{
    TEST_CLASS(COILMilestone1Tests)
    {
    public:

        TEST_METHOD(DefaultConstructorTest)
        {
            PktDef pkt;
            Assert::AreEqual(0, pkt.GetPktCount());
            Assert::AreEqual(0, pkt.GetLength());
            Assert::IsFalse(pkt.GetAck());
        }

        TEST_METHOD(SetPktCountTest)
        {
            PktDef pkt;
            pkt.SetPktCount(12);
            Assert::AreEqual(12, pkt.GetPktCount());
        }

        TEST_METHOD(SetCmdDriveTest)
        {
            PktDef pkt;
            pkt.SetCmd(DRIVE);
            Assert::AreEqual((int)DRIVE, (int)pkt.GetCmd());
        }

        TEST_METHOD(SetCmdSleepTest)
        {
            PktDef pkt;
            pkt.SetCmd(SLEEP);
            Assert::AreEqual((int)SLEEP, (int)pkt.GetCmd());
        }

        TEST_METHOD(SetBodyDataTest)
        {
            PktDef pkt;
            char body[3] = { FORWARD, 5, 80 };
            pkt.SetBodyData(body, 3);
            Assert::AreEqual(8, pkt.GetLength());
        }

        TEST_METHOD(GenPacketTest)
        {
            PktDef pkt;
            pkt.SetPktCount(1);
            pkt.SetCmd(DRIVE);

            char body[3] = { FORWARD, 10, 80 };
            pkt.SetBodyData(body, 3);

            char* raw = pkt.GenPacket();

            Assert::IsNotNull(raw);
            Assert::AreEqual(8, pkt.GetLength());
        }

        TEST_METHOD(CheckCRCValidTest)
        {
            PktDef pkt;
            pkt.SetPktCount(1);
            pkt.SetCmd(DRIVE);

            char body[3] = { FORWARD, 10, 80 };
            pkt.SetBodyData(body, 3);

            char* raw = pkt.GenPacket();
            Assert::IsTrue(pkt.CheckCRC(raw, pkt.GetLength()));
        }

        TEST_METHOD(CheckCRCInvalidTest)
        {
            PktDef pkt;
            pkt.SetPktCount(1);
            pkt.SetCmd(DRIVE);

            char body[3] = { FORWARD, 10, 80 };
            pkt.SetBodyData(body, 3);

            char* raw = pkt.GenPacket();
            raw[pkt.GetLength() - 1] = 0;

            Assert::IsFalse(pkt.CheckCRC(raw, pkt.GetLength()));
        }

        TEST_METHOD(RawPacketConstructorTest)
        {
            PktDef pkt1;
            pkt1.SetPktCount(21);
            pkt1.SetCmd(DRIVE);

            char body[3] = { BACKWARD, 7, 90 };
            pkt1.SetBodyData(body, 3);

            char* raw = pkt1.GenPacket();

            PktDef pkt2(raw);

            Assert::AreEqual(21, pkt2.GetPktCount());
            Assert::AreEqual((int)DRIVE, (int)pkt2.GetCmd());
            Assert::AreEqual(8, pkt2.GetLength());
            Assert::IsTrue(pkt2.CheckCRC(raw, pkt2.GetLength()));
        }
    };
}