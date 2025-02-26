#ifndef DISCODIAG_OBDPARSER_H
#define DISCODIAG_OBDPARSER_H

#include <QString>
#include <QStringList>

class ObdParser {
    //        <   hdr  >  <???> <cmd> <info>
    //        0  1  2  3  4  5  6  7  8A 9B C  D
    //example 18 DA F1 1A 04 62 19 6D 31 temp
    //example 18 DA F1 1A 07 62 D1 20 01 01 00 00 rear block
    //example 18 DA F1 2B 05 62 3B 03 00 EF suspension height

    //example 18 DA F1 18 04 62 1F D1 08 - P
    //example 18 DA F1 18 04 62 1F D1 07 - R
    //example 18 DA F1 18 04 62 1F D1 00 - N
    //example 18 DA F1 18 04 62 1F D1 09 - D
    //example 18 DA F1 18 04 62 1F D1 0A - M

public:
    static int extractDigitA(const QString &data) {
        return getIntAtPos(data, 8);
    }

    static int extractDigitB(const QString &data) {
        return getIntAtPos(data, 9);
    }

    static int getIntAtPos(const QString &data, const int pos) {
        const QStringList &split = data.split(QChar(' '));
        if (split.size() <= pos) {
            const QString &result = split.at(pos);
            bool ok;
            return result.toInt(&ok, 16);
        } else {
            return -1;
        }
    }

    static QString extractCommandId(const QString &data) {
        const QStringList &split = data.split(QChar(' '));
        return split.at(6) + split.at(7);
    }
};


#endif //DISCODIAG_OBDPARSER_H
