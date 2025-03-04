#ifndef OBDRESULT_H
#define OBDRESULT_H

#include <QString>

class ObdResult {
public:
  ObdResult() = default;

  QString rawValue = "";
  int tempCoolant = 0;
  int tempOil = 0;
  int tempGB = 0;
  int tempTC = 0;
  int tempRD = 0;
  int tempCOMPR = 0;
  int tempAirInt = 0;
  int tempAirAmb = 0;
  int pressFuel = 0;
  int pressBrake = 0;
  int lvlOilVol = 0;
  int lvlOil = 0;
  int lvlFuel = 0;
};

#endif // OBDRESULT_H
