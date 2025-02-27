#ifndef OBDRESULT_H
#define OBDRESULT_H

#include <QString>

class ObdResult {
public:
    ObdResult();

    QString rawValue = "";
    QString tempCoolant = "";
    QString tempOil = "";
    QString tempGB = "";
    QString tempTC = "";
    QString tempRD = "";
    QString tempCOMPR = "";
    QString tempAirInt = "";
    QString tempAirAmb = "";
    QString pressFuel = "";
    QString pressBrake = "";
    QString lvlOilVol = "";
    QString lvlOil = "";
    QString lvlFuel = "";
};

#endif // OBDRESULT_H
