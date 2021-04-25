#include "obdresult.h"

ObdResult::ObdResult()
{

}

QString ObdResult::getRdTemp() const
{
    return rdTemp;
}

void ObdResult::setRdTemp(const QString &value)
{
    rdTemp = value;
}

QString ObdResult::getTcRotation() const
{
    return tcRotation;
}

void ObdResult::setTcRotation(const QString &value)
{
    tcRotation = value;
}

QString ObdResult::getTcSolLen() const
{
    return tcSolLen;
}

void ObdResult::setTcSolLen(const QString &value)
{
    tcSolLen = value;
}

QString ObdResult::getTcSolPos() const
{
    return tcSolPos;
}

void ObdResult::setTcSolPos(const QString &value)
{
    tcSolPos = value;
}

QString ObdResult::getTcTemp() const
{
    return tcTemp;
}

void ObdResult::setTcTemp(const QString &value)
{
    tcTemp = value;
}

QString ObdResult::getCurrentGear() const
{
    return currentGear;
}

void ObdResult::setCurrentGear(const QString &value)
{
    currentGear = value;
}

QString ObdResult::getGbTemp() const
{
    return gbTemp;
}

void ObdResult::setGbTemp(const QString &value)
{
    gbTemp = value;
}

QString ObdResult::getDriveShiftPos() const
{
    return driveShiftPos;
}

void ObdResult::setDriveShiftPos(const QString &value)
{
    driveShiftPos = value;
}

QString ObdResult::getSuspFLText() const
{
    return suspFLText;
}

void ObdResult::setSuspFLText(const QString &value)
{
    suspFLText = value;
}

QString ObdResult::getSuspRLText() const
{
    return suspRLText;
}

void ObdResult::setSuspRLText(const QString &value)
{
    suspRLText = value;
}

QString ObdResult::getSuspFRText() const
{
    return suspFRText;
}

void ObdResult::setSuspFRText(const QString &value)
{
    suspFRText = value;
}

QString ObdResult::getSuspRRText() const
{
    return suspRRText;
}

void ObdResult::setSuspRRText(const QString &value)
{
    suspRRText = value;
}

int ObdResult::getSuspFLVal() const
{
    return suspFLVal;
}

void ObdResult::setSuspFLVal(int value)
{
    suspFLVal = value;
}

int ObdResult::getSuspFRVal() const
{
    return suspFRVal;
}

void ObdResult::setSuspFRVal(int value)
{
    suspFRVal = value;
}

int ObdResult::getSuspRLVal() const
{
    return suspRLVal;
}

void ObdResult::setSuspRLVal(int value)
{
    suspRLVal = value;
}

int ObdResult::getSuspRRVal() const
{
    return suspRRVal;
}

void ObdResult::setSuspRRVal(int value)
{
    suspRRVal = value;
}

int ObdResult::getWheelPos() const
{
    return wheelPos;
}

void ObdResult::setWheelPos(int value)
{
    wheelPos = value;
}

bool ObdResult::getRdBlock() const
{
    return rdBlock;
}

void ObdResult::setRdBlock(bool value)
{
    rdBlock = value;
}

bool ObdResult::getTcBlock() const
{
    return tcBlock;
}

void ObdResult::setTcBlock(bool value)
{
    tcBlock = value;
}
