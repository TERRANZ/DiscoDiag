# План починки и доборки PID (по данным `pids.txt`)

Сравнение `pids.txt` с реализацией выявило:
- **2 ошибки формул** в уже реализованных датчиках;
- **2 недостающих хелпера** в `ObdParser`;
- **18 нереализованных PID** (из 31 уникальных).

План разбит на 3 фазы. Порядок важен: сначала инфраструктура (`ObdParser`), затем починка формул, потом доборка.

---

## Фаза 0. Инфраструктура — `ObdParser`

**Файл:** `src/obd/ObdParser.h`

### 0.1. Добавить третий байт `C()` (нужен для пробега `22DD01`)

Сейчас есть только `A()` (индекс 0) и `B()` (индекс 1). Добавляем третий байт:

```cpp
static int A(const QString &data) { return payloadByte(data, 0); }
static int B(const QString &data) { return payloadByte(data, 1); }
static int C(const QString &data) { return payloadByte(data, 2); }   // <- новое
```

`payloadByte()` уже принимает индекс — менять ничего не нужно.

### 0.2. Добавить `SIGNED()` (нужен для тормозов `222B0D` и ускорения `222B12`)

В `pids.txt` старший байт `A` для этих датчиков **знаковый** (дополнительный код). `ObdParser::A()` сейчас возвращает беззнаковое 0–255. Добавляем статический конвертер:

```cpp
// Преобразует беззнаковый байт 0..255 в знаковый -128..127 (доп. код).
static int SIGNED(int v) { return v > 127 ? v - 256 : v; }
```

Использование: `ObdParser::SIGNED(ObdParser::A(value))`.

> **Проверка после фазы 0:** проект собирается, существующие датчики работают без изменений (новые методы не влияют на старые).

---

## Фаза 1. Починка ошибок формул (2 датчика)

### 1.1. `22F42F` — уровень топлива

**Файл:** `src/command/impl/lr322/FuelLevelCommandImpl.cpp`

- Сейчас: `return (ObdParser::A(value) * 100) / 255;` — это формула **процентов**, а результат подписан как `CMD_LVL_FUEL_LITRES` / `lvlFuel`.
- В `pids.txt`: бак 105 л → **литры** = `A*105/255`.

**Решение (выбрать одно):**

| Вариант | Формула | Что показывает | Рекомендация |
|---|---|---|---|
| **A (по умолчанию)** | `A*105/255` | Литры (0–105) | ✅ соответствует подписи `CMD_LVL_FUEL_LITRES` и строке `Fuel` из `pids.txt` |
| B | оставить `A*100/255` + переименовать в `Fuel-%` | Проценты | если пользователю важны именно % |

Реализация варианта A:

```cpp
int FuelLevelCommandImpl::calculate(const QString &value) const {
    return (ObdParser::A(value) * 105) / 255;
}
```

Если нужен и **второй датчик** `Fuel-%` (та же строка `22F42F`, формула `A*100/255`), добавить отдельную команду `FuelPercentCommandImpl` с новым полем `lvlFuelPercent` — см. шаблон в Фазе 2.

### 1.2. `222B0D` — давление тормозной системы

**Файл:** `src/command/impl/lr322/BrakePressCommandImpl.cpp`

- Сейчас: `return ObdParser::A(value) * 8533.3337 + ObdParser::B(value) * 33.33;` — `A` беззнаковый.
- В `pids.txt`: `SIGNED(A)*8533.3337+B*33.33` — старший байт **знаковый**.

Исправление:

```cpp
int BrakePressCommandImpl::calculate(const QString &value) const {
  return ObdParser::SIGNED(ObdParser::A(value)) * 8533.3337 +
         ObdParser::B(value) * 33.33;
}
```

> **Важно:** оба результата теперь возвращаются как `int`, но формулы дают дробные значения (8533.3337, 33.33, 32.8, 20.4, 25.6). Это текущее поведение всех `calculate()` (целочисленное округление). Если нужна точность до знака — это отдельный крупный рефакторинг `ObdResult` (int → double/qreal), **вне рамок текущего плана**.

> **Проверка после фазы 1:** сравнить выведенные значения топлива и тормозного давления с реальным ответом ЭБУ; обновить строки в `sensors.md` (разделы `22F42F` и `222B0D`).

---

## Фаза 2. Доборка 18 отсутствующих PID

### 2.0. Общий шаблон нового датчика

Каждый новый PID требует **5 правок**:

1. `src/command/commandIds.h` — константа `CMD_*`.
2. `src/obd/ObdResult.h` — поле результата.
3. `src/command/impl/lr322/<Имя>CommandImpl.h` + `.cpp` — класс с `calculate()`.
4. `src/obd/ObdService.cpp` — `#include` + `m_commands.emplace_back(std::make_unique<...>())`.
5. `sensors.md` — документация (опционально, но желательно).

Шаблон класса (на примере скорости `22F40D` = `A`):

```cpp
// <Имя>CommandImpl.h
#ifndef SPEEDCOMMANDIMPL_H
#define SPEEDCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"
class SpeedCommandImpl : public AbstractCommand {
public:
  SpeedCommandImpl();
  int calculate(const QString &value) const override;
};
#endif
```

```cpp
// <Имя>CommandImpl.cpp
#include "SpeedCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"
SpeedCommandImpl::SpeedCommandImpl()
    : AbstractCommand(CMD_SPEED, "Vehicle speed command", &ObdResult::speed) {}
int SpeedCommandImpl::calculate(const QString &value) const {
    return ObdParser::A(value);
}
```

Далее датчики сгруппированы по формуле — формула одна, различаются только PID и поле результата.

---

### Группа A. Простой байт `A` (5 датчиков)

Формула: `return ObdParser::A(value);`

| PID | Датчик | `CMD_*` | поле `ObdResult` | класс |
|---|---|---|---|---|
| `22033E` | Наддув (Boost) | `CMD_BOOST` | `boost` | `BoostCommandImpl` |
| `22F40B` | Абс. давление в коллекторе | `CMD_INT_PRES` | `intPres` | `IntPressCommandImpl` |
| `22F40D` | Скорость авто | `CMD_SPEED` | `speed` | `SpeedCommandImpl` |
| `222B06` | ABS скор. FL | `CMD_ABS_FL` | `absFL` | `AbsFLCommandImpl` |
| `222B07` | ABS скор. FR | `CMD_ABS_FR` | `absFR` | `AbsFRCommandImpl` |
| `222B08` | ABS скор. RL | `CMD_ABS_RL` | `absRL` | `AbsRLCommandImpl` |
| `222B09` | ABS скор. RR | `CMD_ABS_RR` | `absRR` | `AbsRRCommandImpl` |

> 4 датчика ABS — идентичны по формуле. Опционально: один параметризуемый класс `AbsSpeedCommandImpl(pid, name, &field)` вместо 4 файлов.

### Группа B. Высота подвески `(A*256+B)/10-20` (4 датчика)

Формула: `return ((ObdParser::A(value) * 256) + ObdParser::B(value)) / 10 - 20;`

| PID | Датчик | `CMD_*` | поле | класс |
|---|---|---|---|---|
| `223B03` | Высота FL | `CMD_HEIGHT_FL` | `heightFL` | `HeightFLCommandImpl` |
| `223B04` | Высота FR | `CMD_HEIGHT_FR` | `heightFR` | `HeightFRCommandImpl` |
| `223B05` | Высота RL | `CMD_HEIGHT_RL` | `heightRL` | `HeightRLCommandImpl` |
| `223B06` | Высота RR | `CMD_HEIGHT_RR` | `heightRR` | `HeightRRCommandImpl` |

> Опционально: общий `SuspensionHeightCommandImpl(pid, name, &field)`.

### Группа C. Два байта без деления `A*256+B` (1 датчик)

| PID | Датчик | Формула | `CMD_*` | поле | класс |
|---|---|---|---|---|---|
| `223B07` | Магистральное давление | `A*256+B` | `CMD_MAIN_PRES` | `mainPres` | `MainPressCommandImpl` |

`return (ObdParser::A(value) * 256) + ObdParser::B(value);`

### Группа D. Масштабируемые байты (3 датчика)

| PID | Датчик | Формула | `CMD_*` | поле | класс |
|---|---|---|---|---|---|
| `22197C` | Напряжение датч. раздатки | `A/4` | `CMD_U_TRCASE` | `uTrCase` | `UTrCaseCommandImpl` |
| `22D112` | Напряжение блока ABS | `A*20.4/255` | `CMD_U_ABS` | `uAbs` | `UAbsCommandImpl` |
| `22F410` | MAF | `A*25.6+B*0.01` | `CMD_MAF` | `maf` | `MafCommandImpl` |

```cpp
// UTrCase:  return ObdParser::A(value) / 4;
// UAbs:     return ObdParser::A(value) * 20.4 / 255;
// Maf:      return ObdParser::A(value) * 25.6 + ObdParser::B(value) * 0.01;
```

### Группа E. Специальные (3 датчика — нужен результат фазы 0)

| PID | Датчик | Формула | Требует |
|---|---|---|---|
| `22DD01` | Общий пробег | `A*65536+B*256+C` | `ObdParser::C()` |
| `222B12` | Продольное ускорение | `SIGNED(A)*32.8/255+B/100` | `ObdParser::SIGNED()` |
| `22D120` | Блокировка зад.диф. | `1-{A:1}` | см. примечание ниже |

```cpp
// Mileage:
return ObdParser::A(value) * 65536 +
       ObdParser::B(value) * 256 +
       ObdParser::C(value);

// Acceler:
return ObdParser::SIGNED(ObdParser::A(value)) * 32.8 / 255 +
       ObdParser::B(value) / 100;
```

**`22D120` RDif-Lock — `1-{A:1}`.** Обозначение битов `{A:n}` нестандартно. Наиболее вероятное чтение — «инверсия n-го бита байта A». Реализация:

```cpp
// Инверсия 1-го бита (0x02) байта A. Требует подтверждения по реальному ответу!
return 1 - ((ObdParser::A(value) >> 1) & 1);
```

> ⚠️ Перед вводом в строй снять реальный ответ ЭБУ по `22D120` и подтвердить, какой бит инвертируется (0-й или 1-й) и по какому условию. Если «{A:1}» означает «весь байт A как флаг», то формула `1 - A`.

---

## Фаза 3. Регистрация в цикле опроса

**Файл:** `src/obd/ObdService.cpp`

Для каждого нового датчика — 2 правки:

1. Включить заголовок (блок `#include` в начале, строки 9–21):
   ```cpp
   #include "src/command/impl/lr322/BoostCommandImpl.h"
   // ... остальные
   ```
2. Зарегистрировать в конструкторе `ObdService::ObdService()` (блок `emplace_back`, строки 30–42):
   ```cpp
   m_commands.emplace_back(std::make_unique<BoostCommandImpl>());
   // ... остальные
   ```

Порядок в списке не влияет на корректность (опрос циклический, `sendNextCommand()` идёт по кругу), но логичнее держать температуры/основное давление вверху, а редкие датчики — внизу.

---

## Итоговый чек-лист затронутых файлов

| Файл | Действие |
|---|---|
| `src/obd/ObdParser.h` | + `C()`, + `SIGNED()` |
| `src/command/commandIds.h` | + 18 констант `CMD_*` |
| `src/obd/ObdResult.h` | + 18 полей |
| `src/command/impl/lr322/FuelLevelCommandImpl.cpp` | исправить формулу (`*105`) |
| `src/command/impl/lr322/BrakePressCommandImpl.cpp` | исправить формулу (`SIGNED(A)`) |
| `src/command/impl/lr322/*` | + 18 новых `*CommandImpl.h/.cpp` |
| `src/obd/ObdService.cpp` | + 18 `#include`, + 18 `emplace_back` |
| `sensors.md` | обновить 2 раздела + добавить 18 записей в сводную таблицу |

**Итого правок по датчикам: 13 (было) → 31 (стало).**

---

## Порядок выполнения и контрольные точки

1. **Фаза 0** → сборка → регрессия существующих датчиков.
2. **Фаза 1** → сборка → сверка 2 значений с реальным ЭБУ.
3. **Фаза 2, группа A** (простейшие) → сборка → проверка 5 PID.
4. **Фаза 2, группы B–D** → сборка → проверка.
5. **Фаза 2, группа E** (специальные) → сборка → проверка + подтверждение `22D120` по реальному ответу.
6. **Фаза 3** → сборка → полный цикл опроса.
7. Обновить `sensors.md` и закрыть расхождения.

## Открытые вопросы (требуют решения/подтверждения)

- [ ] `22F42F`: оставить только **литры** (`*105/255`) или добавить отдельный `Fuel-%`?
- [ ] `22D120 RDif-Lock`: какой именно бит инвертируется в `{A:1}`?
- [ ] Нужна ли дробная точность (`double`) для значений с коэффициентами 0.01–8533.3337, или текущее целочисленное округление допустимо?
- [ ] Опционально: объединить одинаковые формулы (4×ABS, 4×Height) в параметризуемый класс вместо 8 файлов.
