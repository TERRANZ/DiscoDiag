#ifndef OBDRESULT_H
#define OBDRESULT_H

#include <QString>

class ObdResult {
public:
    ObdResult();

    QString rawValue = "";
    QString rdTemp = "";
    QString tcRotation = "";
    QString tcSolLen = "";
    QString tcSolPos = "";
    QString tcTemp = "";
    QString currentGear = "";
    QString gbTemp = "";
    QString driveShiftPos = "";
    QString suspFLText = "";
    QString suspRLText = "";
    QString suspFRText = "";
    QString suspRRText = "";
    int suspFLVal = 0;
    int suspFRVal = 0;
    int suspRLVal = 0;
    int suspRRVal = 0;
    int wheelPos = 0;
    bool rdBlock = false;
    bool tcBlock = false;
};

#endif // OBDRESULT_H
