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
            pkt.SetPktCount(10);

            Assert::AreEqual(10, pkt.GetPktCount());
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

            Assert::AreEqual(8, pkt.GetLength()); // 4 header + 3 body + 1 crc
        }
    };

}