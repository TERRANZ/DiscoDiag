#include <QObject>
#include <QString>
#include <QTest>

#include "src/obd/ObdParser.h"

class TestObdParser : public QObject {
  Q_OBJECT

private slots:
  void getIntAtPosValid();
  void AandB();
  void payloadC();
  void signedByte();
  void AandBWithoutEcho();
  void AandBWithMultilineEcho();
  void AandBWithCrLfAndPrompt();
  void getIntAtPosShortReturnsMinusOne();
  void getIntAtPosExactBoundaryDoesNotReadOutOfBounds();
  void getIntAtPosInvalidToken();
  void hasPayload();
  void extractCommandId();
  void extractCommandIdSkipsLeadingWhitespace();
  void extractCommandIdOnEmptyDoesNotCrash();
};

void TestObdParser::getIntAtPosValid() {
  // 0:cmdid 1:ecu 2:len 3:0x62 4:hi 5:lo 6:b6 7:b7
  const QString frame = "22F423 7E8 05 62 F4 23 03 E8";
  QCOMPARE(ObdParser::getIntAtPos(frame, 0), 0x22F423);
  QCOMPARE(ObdParser::getIntAtPos(frame, 4), 0xF4);
  QCOMPARE(ObdParser::getIntAtPos(frame, 6), 0x03);
  QCOMPARE(ObdParser::getIntAtPos(frame, 7), 0xE8);
}

void TestObdParser::AandB() {
  const QString frame = "22F423 7E8 05 62 F4 23 03 E8";
  QCOMPARE(ObdParser::A(frame), 0x03);
  QCOMPARE(ObdParser::B(frame), 0xE8);
}

void TestObdParser::payloadC() {
  // Three data bytes after the 62 <pid hi> <pid lo> anchor.
  const QString frame = "22DD01 7E8 06 62 DD 01 01 02 03";
  QCOMPARE(ObdParser::A(frame), 0x01);
  QCOMPARE(ObdParser::B(frame), 0x02);
  QCOMPARE(ObdParser::C(frame), 0x03);
}

void TestObdParser::signedByte() {
  // Values in 0..127 are unchanged, 128..255 wrap into the negative range.
  QCOMPARE(ObdParser::SIGNED(0), 0);
  QCOMPARE(ObdParser::SIGNED(127), 127);
  QCOMPARE(ObdParser::SIGNED(128), -128);
  QCOMPARE(ObdParser::SIGNED(255), -1);
}

void TestObdParser::AandBWithoutEcho() {
  // Adapters configured with ATE0 do not repeat the request.
  const QString frame = "7E8 05 62 F4 23 03 E8";
  QCOMPARE(ObdParser::A(frame), 0x03);
  QCOMPARE(ObdParser::B(frame), 0xE8);
}

void TestObdParser::AandBWithMultilineEcho() {
  const QString frame = "22F423\r\r7E8 05 62 F4 23 03 E8\r\r>";
  QCOMPARE(ObdParser::A(frame), 0x03);
  QCOMPARE(ObdParser::B(frame), 0xE8);
}

void TestObdParser::AandBWithCrLfAndPrompt() {
  const QString frame = " \r\n22F423\r\n7E8 05 62 F4 23 03 E8\r\n\r\n> ";
  QCOMPARE(ObdParser::A(frame), 0x03);
  QCOMPARE(ObdParser::B(frame), 0xE8);
}

void TestObdParser::getIntAtPosShortReturnsMinusOne() {
  // A single token must not read out of bounds for higher indices.
  const QString shortFrame = "22F405";
  QCOMPARE(ObdParser::getIntAtPos(shortFrame, 6), -1);
  QCOMPARE(ObdParser::getIntAtPos(shortFrame, 7), -1);
}

void TestObdParser::getIntAtPosExactBoundaryDoesNotReadOutOfBounds() {
  // The old implementation used "size() >= pos", which read one element past
  // the end for these inputs.
  const QString frame = "22F405 7E8 04 62 F4 05";
  QCOMPARE(ObdParser::getIntAtPos(frame, 6), -1);
  QCOMPARE(ObdParser::getIntAtPos(frame, 7), -1);
}

void TestObdParser::getIntAtPosInvalidToken() {
  QCOMPARE(ObdParser::getIntAtPos("not-hex", 0), -1);
  QCOMPARE(ObdParser::getIntAtPos("22F405 ZZ", 1), -1);
}

void TestObdParser::hasPayload() {
  QCOMPARE(ObdParser::hasPayload("22F405 7E8 04 62 F4 05 5C"), true);
  QCOMPARE(ObdParser::hasPayload("7E8 04 62 F4 05 5C\r\r>"), true);
  QCOMPARE(ObdParser::hasPayload("NO DATA"), false);
  QCOMPARE(ObdParser::hasPayload("?"), false);
  QCOMPARE(ObdParser::hasPayload("7E8 04 62 F4 05"), false);
}

void TestObdParser::extractCommandId() {
  const QString frame = "22F405 7E8 04 62 F4 05 5C";
  QCOMPARE(ObdParser::extractCommandId(frame), QString("22F405"));
}

void TestObdParser::extractCommandIdSkipsLeadingWhitespace() {
  QCOMPARE(ObdParser::extractCommandId("\r\n 22F405 7E8 04 62 F4 05 5C\r\r>"),
           QString("22F405"));
}

void TestObdParser::extractCommandIdOnEmptyDoesNotCrash() {
  QCOMPARE(ObdParser::extractCommandId(QString()), QString());
}

QTEST_MAIN(TestObdParser)
#include "test_obdparser.moc"
