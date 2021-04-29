#ifndef OBDRESULT_H
#define OBDRESULT_H

#include <QString>

class ObdResult {
public:
    ObdResult();

    QString getRdTemp() const;

    void setRdTemp(const QString &value);

    QString getTcRotation() const;

    void setTcRotation(const QString &value);

    QString getTcSolLen() const;

    void setTcSolLen(const QString &value);

    QString getTcSolPos() const;

    void setTcSolPos(const QString &value);

    QString getTcTemp() const;

    void setTcTemp(const QString &value);

    QString getCurrentGear() const;

    void setCurrentGear(const QString &value);

    QString getGbTemp() const;

    void setGbTemp(const QString &value);

    QString getDriveShiftPos() const;

    void setDriveShiftPos(const QString &value);

    QString getSuspFLText() const;

    void setSuspFLText(const QString &value);

    QString getSuspRLText() const;

    void setSuspRLText(const QString &value);

    QString getSuspFRText() const;

    void setSuspFRText(const QString &value);

    QString getSuspRRText() const;

    void setSuspRRText(const QString &value);

    int getSuspFLVal() const;

    void setSuspFLVal(int value);

    int getSuspFRVal() const;

    void setSuspFRVal(int value);

    int getSuspRLVal() const;

    void setSuspRLVal(int value);

    int getSuspRRVal() const;

    void setSuspRRVal(int value);

    int getWheelPos() const;

    void setWheelPos(int value);

    bool getRdBlock() const;

    void setRdBlock(bool value);

    bool getTcBlock() const;

    void setTcBlock(bool value);

private:
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
