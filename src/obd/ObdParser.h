#ifndef DISCODIAG_OBDPARSER_H
#define DISCODIAG_OBDPARSER_H

#include <QString>
#include <QStringList>

class ObdParser {
    // <echo> ecu       cmd   A
    // 22F405 7E8 04 62 F4 05 5C
    // 22F446 7E8 04 62 F4 46 27
    // 22F40F 7E8 04 62 F4 0F 4A
    // 2203F3 7E8 04 62 03 F3 41
    // 221E69 7E9 04 62 1E 69 44
public:
    static int extractDigitA(const QString &data) {
        return getIntAtPos(data, 6);
    }

    static int extractDigitB(const QString &data) {
        return getIntAtPos(data, 7);
    }

    static int getIntAtPos(const QString &data, const int& pos) {
        auto const split = data.split(' ');
        if (split.size() >= pos) {
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
