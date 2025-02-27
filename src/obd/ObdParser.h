#ifndef DISCODIAG_OBDPARSER_H
#define DISCODIAG_OBDPARSER_H

#include <QString>
#include <QStringList>

class ObdParser {
    // <echo> ecu          A
    // 22F405 7E8 04 62 01 81
public:
    static int extractDigitA(const QString &data) {
        return getIntAtPos(data, 5);
    }

    static int extractDigitB(const QString &data) {
        return getIntAtPos(data, 6);
    }

    static int getIntAtPos(const QString &data, const int pos) {
        if (const QStringList &split = data.split(QChar(' ')); split.size() <= pos) {
            const QString &result = split.at(pos);
            bool ok;
            return result.toInt(&ok, 16);
        }
        return -1;
    }

    static QString extractCommandId(const QString &data) {
        const QStringList &split = data.split(QChar(' '));
        return split.at(0);
    }
};


#endif //DISCODIAG_OBDPARSER_H
