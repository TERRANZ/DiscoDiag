#include <QObject>
#include <QTest>
#include <QString>

#include "src/command/AbstractCommand.h"

class StubCommand : public AbstractCommand {
public:
    StubCommand() : AbstractCommand("TESTCMD", "stub command") {}
};

class TestAbstractCommand : public QObject {
    Q_OBJECT

private slots:
    void defaultCalculateReturnsMinusOne();
    void gettersReturnConstructorValues();
};

void TestAbstractCommand::defaultCalculateReturnsMinusOne() {
    StubCommand cmd;
    QCOMPARE(cmd.calculate("whatever"), -1);
}

void TestAbstractCommand::gettersReturnConstructorValues() {
    StubCommand cmd;
    QCOMPARE(cmd.getCmdId(), QString("TESTCMD"));
    QCOMPARE(cmd.getCmdName(), QString("stub command"));
    QCOMPARE(cmd.getRawData(), QString());
}

QTEST_MAIN(TestAbstractCommand)
#include "test_abstractcommand.moc"
