#include "src/ui/MainWindow.h"

#include <QApplication>
#include <QFile>
#include <QLocale>
#include <QMutex>
#include <QMutexLocker>
#include <QTextStream>
#include <QTime>
#include <QTranslator>
#include <QtGlobal>

#include <cstdio>
#include <cstdlib>

namespace {
const QString LOG_FILE_PATH = QStringLiteral("debug.log");

QString levelName(QtMsgType type) {
    switch (type) {
    case QtDebugMsg:
        return QStringLiteral("Debug");
    case QtInfoMsg:
        return QStringLiteral("Info");
    case QtWarningMsg:
        return QStringLiteral("Warning");
    case QtCriticalMsg:
        return QStringLiteral("Critical");
    case QtFatalMsg:
        return QStringLiteral("Fatal");
    }
    return QStringLiteral("Unknown");
}

void customMessageOutput(QtMsgType type, const QMessageLogContext &context,
                         const QString &msg) {
    const auto formattedTime = QTime::currentTime().toString("hh:mm:ss.zzz");
    const auto level = levelName(type);
    const auto file = context.file != nullptr ? context.file : "";
    const auto line = context.line;

    const auto text = QStringLiteral("%1 %2: %3 (%4:%5)")
                          .arg(formattedTime, level, msg,
                               QString::fromUtf8(file))
                          .arg(line);

    fprintf(stderr, "%s %s: %s (%s:%u)\n", formattedTime.toLocal8Bit().constData(),
            level.toLocal8Bit().constData(), msg.toLocal8Bit().constData(), file,
            line);
    fflush(stderr);

    static QFile logFile(LOG_FILE_PATH);
    static QMutex logMutex;
    QMutexLocker locker(&logMutex);
    if (!logFile.isOpen()) {
        (void)logFile.open(QIODevice::WriteOnly | QIODevice::Append |
                           QIODevice::Text);
    }
    if (logFile.isOpen()) {
        QTextStream stream(&logFile);
        stream << text << Qt::endl;
        stream.flush();
    }

    if (type == QtFatalMsg) {
        abort();
    }
}
} // namespace

int main(int argc, char *argv[]) {
    qInstallMessageHandler(customMessageOutput);

    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName(QStringLiteral("terranz"));
    QCoreApplication::setApplicationName(QStringLiteral("DiscoDiag"));

    qRegisterMetaType<ObdResult>("ObdResult");
    qRegisterMetaType<QBluetoothAddress>("QBluetoothAddress");

    QTranslator translator;
    if (translator.load(QLocale(), QStringLiteral("DiscoDiag"),
                        QStringLiteral("_"), QStringLiteral(":/i18n"))) {
        QCoreApplication::installTranslator(&translator);
    }

    MainWindow window;
    window.show();
    return app.exec();
}
