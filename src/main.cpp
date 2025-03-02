#include "src/ui/MainWindow.h"

#include <QApplication>
#include <QtGlobal>
#include <QTextStream>
#include <QTime>
#include <QFile>

#include "obd/ObdParser.h"

const QString logFilePath = "debug.log";
bool logToFile = false;

void customMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QHash<QtMsgType, QString> msgLevelHash({
        {QtDebugMsg, "Debug"}, {QtInfoMsg, "Info"}, {QtWarningMsg, "Warning"}, {QtCriticalMsg, "Critical"},
        {QtFatalMsg, "Fatal"}
    });
    const auto localMsg = msg.toLocal8Bit();
    const auto time = QTime::currentTime();
    const auto formattedTime = time.toString("hh:mm:ss.zzz");
    const auto formattedTimeMsg = formattedTime.toLocal8Bit();
    const auto logLevelName = msgLevelHash[type];
    const auto logLevelMsg = logLevelName.toLocal8Bit();

    if (logToFile) {
        const auto txt = QString("%1 %2: %3 (%4)").arg(formattedTime, logLevelName, msg, context.file);
        QFile outFile(logFilePath);
        outFile.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream ts(&outFile);
        ts << txt << Qt::endl;
        outFile.close();

        fprintf(stderr, "%s %s: %s (%s:%u, %s)\n", formattedTimeMsg.constData(), logLevelMsg.constData(),
                localMsg.constData(), context.file, context.line, context.function);
        fflush(stderr);
    }

    if (type == QtFatalMsg)
        abort();
}


int main(int argc, char *argv[]) {
    logToFile = true;
    qInstallMessageHandler(customMessageOutput);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
