#include <QObject>
#include <QTest>
#include <QString>

#include "src/command/commandIds.h"

#include "src/command/impl/lr322/temperature/CoolantTempCommandImpl.h"
#include "src/command/impl/lr322/temperature/OilTempCommandImpl.h"
#include "src/command/impl/lr322/temperature/GbTempCommandImpl.h"
#include "src/command/impl/lr322/temperature/IntakeAirTempCommandImpl.h"
#include "src/command/impl/lr322/temperature/AmbientAirTempCommandImpl.h"
#include "src/command/impl/lr322/temperature/ComprTempCommandImpl.h"
#include "src/command/impl/lr322/temperature/TCTempCommandImpl.h"
#include "src/command/impl/lr322/temperature/RDTempCommandImpl.h"
#include "src/command/impl/lr322/FuelPressCommandImpl.h"
#include "src/command/impl/lr322/BrakePressCommandImpl.h"
#include "src/command/impl/lr322/FuelLevelCommandImpl.h"
#include "src/command/impl/lr322/OilLevelCommandImpl.h"
#include "src/command/impl/lr322/OilVolumeCommandImpl.h"

// Realistic ELM327 response: command echo, CR-separated data line, prompt.
// The parser must not depend on absolute token positions.
static QString frame(const QString &cmdId, int b6, int b7 = 0) {
    return QStringLiteral("%1\r\r7E8 04 62 F4 05 %2 %3\r\r>")
        .arg(cmdId)
        .arg(b6, 2, 16, QChar('0'))
        .arg(b7, 2, 16, QChar('0'));
}

class TestCommands : public QObject {
    Q_OBJECT

private slots:
    void commandIds();
    void getCmdId();
    void coolantTemp();
    void oilTemp();
    void gbTemp();
    void intakeAirTemp();
    void ambientAirTemp();
    void comprTemp();
    void tcTemp();
    void rdTemp();
    void fuelPress();
    void brakePress();
    void fuelLevel();
    void oilLevel();
    void oilVolume();
    void resultFields();
};

void TestCommands::commandIds() {
    QCOMPARE(CMD_TEMP_COOLANT, QString("22F405"));
    QCOMPARE(CMD_TEMP_OIL, QString("2203F3"));
    QCOMPARE(CMD_TEMP_GB, QString("221E69"));
    QCOMPARE(CMD_TEMP_TC, QString("22D11C"));
    QCOMPARE(CMD_TEMP_RD, QString("221E8A"));
    QCOMPARE(CMD_TEMP_COMPR, QString("22D11A"));
    QCOMPARE(CMD_TEMP_AIR_INTAKE, QString("22F40F"));
    QCOMPARE(CMD_TEMP_AIR_AMBIENT, QString("22F446"));
    QCOMPARE(CMD_PRESS_FUEL, QString("22F423"));
    QCOMPARE(CMD_PRESS_BRAKE, QString("222B0D"));
    QCOMPARE(CMD_LVL_OIL_VOL, QString("2203F2"));
    QCOMPARE(CMD_LVL_OIL, QString("2203E6"));
    QCOMPARE(CMD_LVL_FUEL_LITRES, QString("22F42F"));
}

void TestCommands::getCmdId() {
    QCOMPARE(CoolantTempCommandImpl().getCmdId(), QString("22F405"));
    QCOMPARE(FuelPressCommandImpl().getCmdId(), QString("22F423"));
}

void TestCommands::coolantTemp() {
    // b6 = 0x5C = 92 -> 92 - 40 = 52
    CoolantTempCommandImpl cmd;
    QCOMPARE(cmd.calculate(frame("22F405", 0x5C)), 52);
}

void TestCommands::oilTemp() {
    // b6 = 0x4B = 75 -> 75 - 40 = 35
    OilTempCommandImpl cmd;
    QCOMPARE(cmd.calculate(frame("2203F3", 0x4B)), 35);
}

void TestCommands::gbTemp() {
    // b6 = 0x53 = 83 -> 83 - 40 = 43
    GbTempCommandImpl cmd;
    QCOMPARE(cmd.calculate(frame("221E69", 0x53)), 43);
}

void TestCommands::intakeAirTemp() {
    // b6 = 0x3C = 60 -> 60 - 40 = 20
    IntakeAirTempCommandImpl cmd;
    QCOMPARE(cmd.calculate(frame("22F40F", 0x3C)), 20);
}

void TestCommands::ambientAirTemp() {
    // b6 = 0x23 = 35 -> 35 - 40 = -5
    AmbientAirTempCommandImpl cmd;
    QCOMPARE(cmd.calculate(frame("22F446", 0x23)), -5);
}

void TestCommands::comprTemp() {
    // b6 = 0x67 = 103 -> 103 - 40 = 63
    ComprTempCommandImpl cmd;
    QCOMPARE(cmd.calculate(frame("22D11A", 0x67)), 63);
}

void TestCommands::tcTemp() {
    // b6 = 0x5A = 90 -> 90 - 40 = 50
    TCTempCommandImpl cmd;
    QCOMPARE(cmd.calculate(frame("22D11C", 0x5A)), 50);
}

void TestCommands::rdTemp() {
    // b6 = 0x4C = 76 -> 76 - 40 = 36
    RDTempCommandImpl cmd;
    QCOMPARE(cmd.calculate(frame("221E8A", 0x4C)), 36);
}

void TestCommands::fuelPress() {
    // (0x03 * 256 + 0xE8) / 100 = (768 + 232) / 100 = 10
    FuelPressCommandImpl cmd;
    QCOMPARE(cmd.calculate(frame("22F423", 0x03, 0xE8)), 10);
}

void TestCommands::brakePress() {
    // 0x01 * 8533.3337 + 0x00 * 33.33 -> 8533.33 -> truncated to 8533
    BrakePressCommandImpl cmd;
    QCOMPARE(cmd.calculate(frame("222B0D", 0x01, 0x00)), 8533);
}

void TestCommands::fuelLevel() {
    // (0xFF * 100) / 255 = 25500 / 255 = 100
    FuelLevelCommandImpl cmd;
    QCOMPARE(cmd.calculate(frame("22F42F", 0xFF)), 100);
}

void TestCommands::oilLevel() {
    // identity: b6 = 0x42 = 66
    OilLevelCommandImpl cmd;
    QCOMPARE(cmd.calculate(frame("2203E6", 0x42)), 66);
}

void TestCommands::oilVolume() {
    // b6 / 10 = 0x96 / 10 = 150 / 10 = 15
    OilVolumeCommandImpl cmd;
    QCOMPARE(cmd.calculate(frame("2203F2", 0x96)), 15);
}

void TestCommands::resultFields() {
    QVERIFY(CoolantTempCommandImpl().getResultField() == &ObdResult::tempCoolant);
    QVERIFY(OilTempCommandImpl().getResultField() == &ObdResult::tempOil);
    QVERIFY(GbTempCommandImpl().getResultField() == &ObdResult::tempGB);
    QVERIFY(IntakeAirTempCommandImpl().getResultField() == &ObdResult::tempAirInt);
    QVERIFY(AmbientAirTempCommandImpl().getResultField() == &ObdResult::tempAirAmb);
    QVERIFY(ComprTempCommandImpl().getResultField() == &ObdResult::tempCOMPR);
    QVERIFY(TCTempCommandImpl().getResultField() == &ObdResult::tempTC);
    QVERIFY(RDTempCommandImpl().getResultField() == &ObdResult::tempRD);
    QVERIFY(FuelPressCommandImpl().getResultField() == &ObdResult::pressFuel);
    QVERIFY(BrakePressCommandImpl().getResultField() == &ObdResult::pressBrake);
    QVERIFY(FuelLevelCommandImpl().getResultField() == &ObdResult::lvlFuel);
    QVERIFY(OilLevelCommandImpl().getResultField() == &ObdResult::lvlOil);
    QVERIFY(OilVolumeCommandImpl().getResultField() == &ObdResult::lvlOilVol);
}

QTEST_MAIN(TestCommands)
#include "test_commands.moc"
