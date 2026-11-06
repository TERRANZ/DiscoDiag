#ifndef OBDRESULT_H
#define OBDRESULT_H

#include <QMetaType>
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
  int boost = 0;
  int intPres = 0;
  int speed = 0;
  int absFL = 0;
  int absFR = 0;
  int absRL = 0;
  int absRR = 0;
  int heightFL = 0;
  int heightFR = 0;
  int heightRL = 0;
  int heightRR = 0;
  int mainPres = 0;
  int uTrCase = 0;
  int uAbs = 0;
  int maf = 0;
};

Q_DECLARE_METATYPE(ObdResult)

#endif // OBDRESULT_H
