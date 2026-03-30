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

        TEST_METHOD(DefaultConstructor_PktCountZero)
        {
            PktDef pkt;
            Assert::AreEqual(0, pkt.GetPktCount());
        }

        TEST_METHOD(DefaultConstructor_LengthZero)
        {
            PktDef pkt;
            Assert::AreEqual(0, pkt.GetLength());
        }

        TEST_METHOD(DefaultConstructor_AckFalse)
        {
            PktDef pkt;
            Assert::IsFalse(pkt.GetAck());
        }

        TEST_METHOD(DefaultConstructor_BodyNull)
        {
            PktDef pkt;
            Assert::IsTrue(pkt.GetBodyData() == nullptr);
        }

        TEST_METHOD(RawConstructor_NullInput_SafeState)
        {
            PktDef pkt(nullptr);
            Assert::AreEqual(0, pkt.GetPktCount());
            Assert::AreEqual(0, pkt.GetLength());
            Assert::IsFalse(pkt.GetAck());
            Assert::IsTrue(pkt.GetBodyData() == nullptr);
        }

        TEST_METHOD(SetPktCount_PositiveValue)
        {
            PktDef pkt;
            pkt.SetPktCount(12);
            Assert::AreEqual(12, pkt.GetPktCount());
        }

        TEST_METHOD(SetPktCount_ZeroValue)
        {
            PktDef pkt;
            pkt.SetPktCount(0);
            Assert::AreEqual(0, pkt.GetPktCount());
        }

        TEST_METHOD(SetCmd_Drive_ReturnsDrive)
        {
            PktDef pkt;
            pkt.SetCmd(DRIVE);
            Assert::AreEqual((int)DRIVE, (int)pkt.GetCmd());
        }

        TEST_METHOD(SetCmd_Sleep_ReturnsSleep)
        {
            PktDef pkt;
            pkt.SetCmd(SLEEP);
            Assert::AreEqual((int)SLEEP, (int)pkt.GetCmd());
        }

        TEST_METHOD(SetCmd_Response_ReturnsResponse)
        {
            PktDef pkt;
            pkt.SetCmd(RESPONSE);
            Assert::AreEqual((int)RESPONSE, (int)pkt.GetCmd());
        }

        TEST_METHOD(SetCmd_ChangeFromDriveToSleep)
        {
            PktDef pkt;
            pkt.SetCmd(DRIVE);
            pkt.SetCmd(SLEEP);
            Assert::AreEqual((int)SLEEP, (int)pkt.GetCmd());
        }

        TEST_METHOD(SetBodyData_DriveForward_LengthIs8)
        {
            PktDef pkt;
            pkt.SetCmd(DRIVE);
            char body[3] = { FORWARD, 5, 80 };
            pkt.SetBodyData(body, 3);
            Assert::AreEqual(8, pkt.GetLength());
        }

        TEST_METHOD(SetBodyData_DriveBackward_LengthIs8)
        {
            PktDef pkt;
            pkt.SetCmd(DRIVE);
            char body[3] = { BACKWARD, 7, 90 };
            pkt.SetBodyData(body, 3);
            Assert::AreEqual(8, pkt.GetLength());
        }

        TEST_METHOD(SetBodyData_NoBody_LengthIs5)
        {
            PktDef pkt;
            pkt.SetCmd(SLEEP);
            pkt.SetBodyData(nullptr, 0);
            Assert::AreEqual(5, pkt.GetLength());
        }

        TEST_METHOD(GetBodyData_ReturnsCopiedDriveBody)
        {
            PktDef pkt;
            pkt.SetCmd(DRIVE);

            char body[3] = { FORWARD, 5, 80 };
            pkt.SetBodyData(body, 3);

            char* out = pkt.GetBodyData();

            Assert::IsNotNull(out);
            Assert::AreEqual((char)FORWARD, out[0]);
            Assert::AreEqual((char)5, out[1]);
            Assert::AreEqual((char)80, out[2]);
        }

        TEST_METHOD(GenPacket_ReturnsNonNullBuffer)
        {
            PktDef pkt;
            pkt.SetPktCount(1);
            pkt.SetCmd(DRIVE);

            char body[3] = { FORWARD, 10, 80 };
            pkt.SetBodyData(body, 3);

            char* raw = pkt.GenPacket();

            Assert::IsNotNull(raw);
        }

        TEST_METHOD(GenPacket_DrivePacket_LengthIs8)
        {
            PktDef pkt;
            pkt.SetPktCount(1);
            pkt.SetCmd(DRIVE);

            char body[3] = { FORWARD, 10, 80 };
            pkt.SetBodyData(body, 3);

            pkt.GenPacket();
            Assert::AreEqual(8, pkt.GetLength());
        }

        TEST_METHOD(GenPacket_SleepPacket_LengthIs5)
        {
            PktDef pkt;
            pkt.SetPktCount(3);
            pkt.SetCmd(SLEEP);
            pkt.SetBodyData(nullptr, 0);

            char* raw = pkt.GenPacket();

            Assert::IsNotNull(raw);
            Assert::AreEqual(5, pkt.GetLength());
        }

        TEST_METHOD(GenPacket_ResponsePacket_LengthIs5)
        {
            PktDef pkt;
            pkt.SetPktCount(4);
            pkt.SetCmd(RESPONSE);
            pkt.SetBodyData(nullptr, 0);

            char* raw = pkt.GenPacket();

            Assert::IsNotNull(raw);
            Assert::AreEqual(5, pkt.GetLength());
        }

        TEST_METHOD(CheckCRC_ValidDrivePacket_ReturnsTrue)
        {
            PktDef pkt;
            pkt.SetPktCount(1);
            pkt.SetCmd(DRIVE);

            char body[3] = { FORWARD, 10, 80 };
            pkt.SetBodyData(body, 3);

            char* raw = pkt.GenPacket();

            Assert::IsTrue(pkt.CheckCRC(raw, pkt.GetLength()));
        }

        TEST_METHOD(CheckCRC_InvalidCRCByte_ReturnsFalse)
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

        TEST_METHOD(CheckCRC_NoBodyPacket_ReturnsTrue)
        {
            PktDef pkt;
            pkt.SetPktCount(3);
            pkt.SetCmd(SLEEP);
            pkt.SetBodyData(nullptr, 0);

            char* raw = pkt.GenPacket();

            Assert::IsTrue(pkt.CheckCRC(raw, pkt.GetLength()));
        }

        TEST_METHOD(CheckCRC_ModifiedBodyByte_ReturnsFalse)
        {
            PktDef pkt;
            pkt.SetPktCount(5);
            pkt.SetCmd(DRIVE);

            char body[3] = { BACKWARD, 9, 95 };
            pkt.SetBodyData(body, 3);

            char* raw = pkt.GenPacket();
            raw[HEADERSIZE] = FORWARD; // modify first body byte

            Assert::IsFalse(pkt.CheckCRC(raw, pkt.GetLength()));
        }

        TEST_METHOD(RawConstructor_ParsesDrivePacketCorrectly)
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

            char* parsed = pkt2.GetBodyData();
            Assert::IsNotNull(parsed);
            Assert::AreEqual((char)BACKWARD, parsed[0]);
            Assert::AreEqual((char)7, parsed[1]);
            Assert::AreEqual((char)90, parsed[2]);

            Assert::IsTrue(pkt2.CheckCRC(raw, pkt2.GetLength()));
        }

        TEST_METHOD(RawConstructor_ParsesSleepPacketCorrectly)
        {
            PktDef pkt1;
            pkt1.SetPktCount(30);
            pkt1.SetCmd(SLEEP);
            pkt1.SetBodyData(nullptr, 0);

            char* raw = pkt1.GenPacket();

            PktDef pkt2(raw);

            Assert::AreEqual(30, pkt2.GetPktCount());
            Assert::AreEqual((int)SLEEP, (int)pkt2.GetCmd());
            Assert::AreEqual(5, pkt2.GetLength());
            Assert::IsTrue(pkt2.GetBodyData() == nullptr);
            Assert::IsTrue(pkt2.CheckCRC(raw, pkt2.GetLength()));
        }

        TEST_METHOD(RawConstructor_ParsesResponsePacketCorrectly)
        {
            PktDef pkt1;
            pkt1.SetPktCount(40);
            pkt1.SetCmd(RESPONSE);
            pkt1.SetBodyData(nullptr, 0);

            char* raw = pkt1.GenPacket();

            PktDef pkt2(raw);

            Assert::AreEqual(40, pkt2.GetPktCount());
            Assert::AreEqual((int)RESPONSE, (int)pkt2.GetCmd());
            Assert::AreEqual(5, pkt2.GetLength());
            Assert::IsTrue(pkt2.GetBodyData() == nullptr);
            Assert::IsTrue(pkt2.CheckCRC(raw, pkt2.GetLength()));
        }
    };
}