# OBD2 Command Reference

Справочник всех диагностических команд DiscoDiag с формулами расчёта и ссылками на реализации.

---

## Содержание

- [Общая структура](#общая-структура)
- [Общие OBD2 AT-команды](#общие-obd2-at-команды)
- [Команды датчиков LR322](#команды-датчиков-lr322)
  - [Температура](#температура)
  - [Давление](#давление)
  - [Уровень жидкостей](#уровень-жидкостей)
- [Структура OBD2-ответа](#структура-obd2-ответа)
- [ObdParser — методы извлечения данных](#obdparser--методы-извлечения-данных)

---

## Общая структура

Все команды наследуются от `AbstractCommand` ([`src/command/AbstractCommand.h`](src/command/AbstractCommand.h)):

```cpp
class AbstractCommand {
public:
    virtual int calculate(const QString &value);
    // По умолчанию возвращает -1.
    // Переопределяется в подклассах для преобразования сырых hex-данных ответа ЭБУ
    // в осмысленное целочисленное значение.

    QString getCmdId() const;  // Возвращает OBD2-идентификатор
    QString getCmdName() const; // Возвращает имя команды
    QString getRawData() const; // Возвращает сырые данные ответа
};
```

Константы всех идентификаторов команд определены в [`src/command/commandIds.h`](src/command/commandIds.h):

```cpp
static const auto CMD_TEMP_COOLANT = QString("22F405");       // Температура охлаждающей жидкости
static const auto CMD_TEMP_OIL = QString("2203F3");            // Температура масла
static const auto CMD_TEMP_GB = QString("221E69");             // Температура коробки передач
static const auto CMD_TEMP_TC = QString("22D11C");             // Температура раздаточной коробки
static const auto CMD_TEMP_RD = QString("221E8A");             // Температура заднего дифференциала
static const auto CMD_TEMP_COMPR = QString("22D11A");          // Температура компрессора
static const auto CMD_TEMP_AIR_INTAKE = QString("22F40F");     // Температура воздуха на впуске
static const auto CMD_TEMP_AIR_AMBIENT = QString("22F446");    // Температура наружного воздуха
static const auto CMD_PRESS_FUEL = QString("22F423");          // Давление топлива
static const auto CMD_PRESS_BRAKE = QString("222B0D");         // Давление тормозной системы
static const auto CMD_LVL_OIL_VOL = QString("2203F2");         // Объём масла
static const auto CMD_LVL_OIL = QString("2203E6");             // Уровень масла
static const auto CMD_LVL_FUEL_LITRES = QString("22F42F");     // Уровень топлива (литры)
```

---

## Общие OBD2 AT-команды

Эти команды используются для инициализации и настройки OBD2-сессии. Они не возвращают измерительных данных, а управляют состоянием ЭБУ.

### ATZ — Сброс ЭБУ

| Параметр | Значение |
|---|---|
| **Command ID** | `ATZ` |
| **Имя** | `Obd Reset` |
| **Реализация** | [`src/command/impl/common/ObdResetFixCommandImpl.h`](src/command/impl/common/ObdResetFixCommandImpl.h) |
| **Формула** | Нет (команда управления) |
| **Статус** | ✅ Реализована |

**Описание:** Сбрасывает ЭБУ / OBD-адаптер. Отправляется первым в последовательности инициализации после установления Bluetooth-соединения. ЭБУ возвращает `OK>`.

---

### ATH1 — Включение заголовков

| Параметр | Значение |
|---|---|
| **Command ID** | `ATH1` |
| **Имя** | `Set display headers parameter.` |
| **Реализация** | [`src/command/impl/common/DisplayHeaderCommandImpl.h`](src/command/impl/common/DisplayHeaderCommandImpl.h) |
| **Формула** | Нет (команда управления) |
| **Статус** | ✅ Реализована |

**Описание:** Включает отображение заголовков в ответах ЭБУ. После этой команды ЭБУ prepend'ит ID отправленной команды к каждому ответу, что позволяет парсеру сопоставлять ответ с конкретным запросом.

---

### ATSP6 — Выбор протокола

| Параметр | Значение |
|---|---|
| **Command ID** | `ATSP6` |
| **Имя** | `Select Protocol Command` |
| **Реализация** | [`src/command/impl/common/SelectProtocolObdCommandImpl.h`](src/command/impl/common/SelectProtocolObdCommandImpl.h) |
| **Формула** | Нет (команда управления) |
| **Статус** | ✅ Реализована |

**Описание:** Устанавливает OBD2-протокол 6 (KWP2000 Fast Init). Отправляется последним в последовательности инициализации. После успешного выполнения начинается циклический опрос датчиков.

---

### EXT — Расширенные параметры (stub)

| Параметр | Значение |
|---|---|
| **Command ID** | *(пустая строка)* |
| **Имя** | `Extended params command` |
| **Реализация** | [`src/command/impl/common/ExtParamsCommandImpl.h`](src/command/impl/common/ExtParamsCommandImpl.h) |
| **Формула** | Нет |
| **Статус** | ⚪ Заглушка (не реализована) |

**Описание:** Заготовка для расширенных параметров. Command ID пустой, метод `calculate()` не переопределён.

---

## Команды датчиков LR322

### Температура

Все температурные датчики используют одинаковый алгоритм парсинга и одинаковую формулу перевода сырых байтов в температуру по Цельсию.

**Формула:**

```
result = ObdParser::A(value) - 40
```

Где `ObdParser::A(value)` извлекает байт данных из позиции 6 hex-строки ответа ЭБУ.

Вычитание 40 — стандартный OBD2-сдвиг для температуры:
- Диапазон поддерживаемых значений: **-40 °C … +215 °C** (сырой байт 0x00 … 0xFF)

---

#### 22F405 — Температура охлаждающей жидкости

| Параметр | Значение |
|---|---|
| **Command ID** | `22F405` |
| **Имя** | `Coolant temp command` |
| **Переменная в ObdResult** | `tempCoolant` |
| **Реализация** | [`src/command/impl/lr322/temperature/CoolantTempCommandImpl.h`](src/command/impl/lr322/temperature/CoolantTempCommandImpl.h) · [`CoolantTempCommandImpl.cpp`](src/command/impl/lr322/temperature/CoolantTempCommandImpl.cpp) |
| **Статус** | ✅ Реализована, в цикле опроса |

**Код:**
```cpp
int CoolantTempCommandImpl::calculate(const QString &value) {
    return ObdParser::A(value) - 40;
}
```

---

#### 2203F3 — Температура масла

| Параметр | Значение |
|---|---|
| **Command ID** | `2203F3` |
| **Имя** | `Oil temp command` |
| **Переменная в ObdResult** | `tempOil` |
| **Реализация** | [`src/command/impl/lr322/temperature/OilTempCommandImpl.h`](src/command/impl/lr322/temperature/OilTempCommandImpl.h) · [`OilTempCommandImpl.cpp`](src/command/impl/lr322/temperature/OilTempCommandImpl.cpp) |
| **Статус** | ✅ Реализована, в цикле опроса |

**Код:**
```cpp
int OilTempCommandImpl::calculate(const QString &value) {
    return ObdParser::A(value) - 40;
}
```

---

#### 221E69 — Температура коробки передач

| Параметр | Значение |
|---|---|
| **Command ID** | `221E69` |
| **Имя** | `Gearbox temp command` |
| **Переменная в ObdResult** | `tempGB` |
| **Реализация** | [`src/command/impl/lr322/temperature/GbTempCommandImpl.h`](src/command/impl/lr322/temperature/GbTempCommandImpl.h) · [`GbTempCommandImpl.cpp`](src/command/impl/lr322/temperature/GbTempCommandImpl.cpp) |
| **Статус** | ✅ Реализована, в цикле опроса |

**Код:**
```cpp
int GbTempCommandImpl::calculate(const QString &value) {
    return ObdParser::A(value) - 40;
}
```

---

#### 22D11C — Температура раздаточной коробки

| Параметр | Значение |
|---|---|
| **Command ID** | `22D11C` |
| **Имя** | `Transfer case temp command` |
| **Переменная в ObdResult** | `tempTC` |
| **Реализация** | [`src/command/impl/lr322/temperature/TCTempCommandImpl.h`](src/command/impl/lr322/temperature/TCTempCommandImpl.h) · [`TCTempCommandImpl.cpp`](src/command/impl/lr322/temperature/TCTempCommandImpl.cpp) |
| **Статус** | ✅ Реализована, НЕ в цикле опроса, данные не обновляются в UI |

**Код:**
```cpp
int TCTempCommandImpl::calculate(const QString &value) {
    return ObdParser::A(value) - 40;
}
```

---

#### 221E8A — Температура заднего дифференциала

| Параметр | Значение |
|---|---|
| **Command ID** | `221E8A` |
| **Имя** | `Rear diff temp command` |
| **Переменная в ObdResult** | `tempRD` |
| **Реализация** | [`src/command/impl/lr322/temperature/RDTempCommandImpl.h`](src/command/impl/lr322/temperature/RDTempCommandImpl.h) · [`RDTempCommandImpl.cpp`](src/command/impl/lr322/temperature/RDTempCommandImpl.cpp) |
| **Статус** | ✅ Реализована |

**Код:**
```cpp
int RDTempCommandImpl::calculate(const QString &value) {
    return ObdParser::A(value) - 40;
}
```

---

#### 22D11A — Температура компрессора

| Параметр | Значение |
|---|---|
| **Command ID** | `22D11A` |
| **Имя** | `Ambient air temp command` ⚠️ |
| **Переменная в ObdResult** | `tempCOMPR` |
| **Реализация** | [`src/command/impl/lr322/temperature/ComprTempCommandImpl.h`](src/command/impl/lr322/temperature/ComprTempCommandImpl.h) · [`ComprTempCommandImpl.cpp`](src/command/impl/lr322/temperature/ComprTempCommandImpl.cpp) |
| **Статус** | ✅ Реализована, в цикле опроса |

**Код:**
```cpp
int ComprTempCommandImpl::calculate(const QString &value) {
    return ObdParser::A(value) - 40;
}
```

> ⚠️ **Баг:** Имя команды в конструкторе — `"Ambient air temp command"`, хотя это датчик компрессора.

---

#### 22F40F — Температура воздуха на впуске

| Параметр | Значение |
|---|---|
| **Command ID** | `22F40F` |
| **Имя** | `Intake air temp command` |
| **Переменная в ObdResult** | `tempAirInt` |
| **Реализация** | [`src/command/impl/lr322/temperature/IntakeAirTempCommandImpl.h`](src/command/impl/lr322/temperature/IntakeAirTempCommandImpl.h) · [`IntakeAirTempCommandImpl.cpp`](src/command/impl/lr322/temperature/IntakeAirTempCommandImpl.cpp) |
| **Статус** | ✅ Реализована, в цикле опроса |

**Код:**
```cpp
int IntakeAirTempCommandImpl::calculate(const QString &value) {
    return ObdParser::A(value) - 40;
}
```

---

#### 22F446 — Температура наружного воздуха

| Параметр | Значение |
|---|---|
| **Command ID** | `22F446` |
| **Имя** | `Ambient air temp command` |
| **Переменная в ObdResult** | `tempAirAmb` |
| **Реализация** | [`src/command/impl/lr322/temperature/AmbientAirTempCommandImpl.h`](src/command/impl/lr322/temperature/AmbientAirTempCommandImpl.h) · [`AmbientAirTempCommandImpl.cpp`](src/command/impl/lr322/temperature/AmbientAirTempCommandImpl.cpp) |
| **Статус** | ✅ Реализована, в цикле опроса |

**Код:**
```cpp
int AmbientAirTempCommandImpl::calculate(const QString &value) {
    return ObdParser::A(value) - 40;
}
```

---

### Давление

---

#### 22F423 — Давление топлива

| Параметр | Значение |
|---|---|
| **Command ID** | `22F423` |
| **Имя** | `Fuel Pressure Command` |
| **Переменная в ObdResult** | `pressFuel` |
| **Реализация** | [`src/command/impl/lr322/FuelPressCommandImpl.h`](src/command/impl/lr322/FuelPressCommandImpl.h) · [`FuelPressCommandImpl.cpp`](src/command/impl/lr322/FuelPressCommandImpl.cpp) |
| **Статус** | ✅ Реализована, НЕ в цикле опроса |

**Формула:**

```
result = ((ObdParser::A(value) * 256) + ObdParser::B(value)) / 100
```

Где:
- `ObdParser::A(value)` — байт данных из позиции 6 (старший байт)
- `ObdParser::B(value)` — байт данных из позиции 7 (младший байт)

Метод объединяет два байта в 16-битное значение, делит на 100. Результат — давление в kPa (с двумя знаками после запятой, но возвращается как `int`).

**Код:**
```cpp
int FuelPressCommandImpl::calculate(const QString &value) {
    return ((ObdParser::A(value) * 256) + ObdParser::B(value)) / 100;
}
```

---

#### 222B0D — Давление тормозной системы

| Параметр | Значение |
|---|---|
| **Command ID** | `222B0D` |
| **Имя** | `Brake pressure command` |
| **Переменная в ObdResult** | `pressBrake` |
| **Реализация** | [`src/command/impl/lr322/BrakePressCommandImpl.h`](src/command/impl/lr322/BrakePressCommandImpl.h) · [`BrakePressCommandImpl.cpp`](src/command/impl/lr322/BrakePressCommandImpl.cpp) |
| **Статус** | ✅ Реализована, НЕ в цикле опроса |

**Формула:**

```
result = ObdParser::A(value) * 8533.3337 + ObdParser::B(value) * 33.33
```

Где:
- `ObdParser::A(value)` — байт данных из позиции 6 (коэффициент масштабирования старшего байта)
- `ObdParser::B(value)` — байт данных из позиции 7 (коэффициент масштабирования младшего байта)

Результат — давление в kPa.

**Код:**
```cpp
int BrakePressCommandImpl::calculate(const QString &value) {
    return ObdParser::A(value) * 8533.3337 +
           ObdParser::B(value) * 33.33;
}
```

---

### Уровень жидкостей

---

#### 22F42F — Уровень топлива

| Параметр | Значение |
|---|---|
| **Command ID** | `22F42F` |
| **Имя** | `Fuel Level Command` |
| **Переменная в ObdResult** | `lvlFuel` |
| **Реализация** | [`src/command/impl/lr322/FuelLevelCommandImpl.h`](src/command/impl/lr322/FuelLevelCommandImpl.h) · [`FuelLevelCommandImpl.cpp`](src/command/impl/lr322/FuelLevelCommandImpl.cpp) |
| **Статус** | ✅ Реализована, НЕ в цикле опроса |

**Формула:**

```
result = (ObdParser::A(value) * 100) / 255
```

Где:
- `ObdParser::A(value)` — сырое значение (0–255)

Метод конвертирует 8-битное сырое значение датчика уровня топлива в литры. Диапазон: 0–100 литров (при сырых 0–255).

**Код:**
```cpp
int FuelLevelCommandImpl::calculate(const QString &value) {
    return (ObdParser::A(value) * 100) / 255;
}
```

---

#### 2203E6 — Уровень масла (stub)

| Параметр | Значение |
|---|---|
| **Command ID** | `2203E6` |
| **Имя** | `Oil level command` |
| **Переменная в ObdResult** | `lvlOil` |
| **Реализация** | [`src/command/impl/lr322/OilLevelCommandImpl.h`](src/command/impl/lr322/OilLevelCommandImpl.h) · [`OilLevelCommandImpl.cpp`](src/command/impl/lr322/OilLevelCommandImpl.cpp) |
| **Статус** | ✅ Реализована |

**Код:**
```cpp
int OilLevelCommandImpl::calculate(const QString &value) {
    return ObdParser::A(value);
}
```

---

#### 2203F2 — Объём масла

| Параметр | Значение |
|---|---|
| **Command ID** | `2203F2` |
| **Имя** | `Oil Volume Command` |
| **Переменная в ObdResult** | `lvlOilVol` |
| **Реализация** | [`src/command/impl/lr322/OilVolumeCommandImpl.h`](src/command/impl/lr322/OilVolumeCommandImpl.h) · [`OilVolumeCommandImpl.cpp`](src/command/impl/lr322/OilVolumeCommandImpl.cpp) |
| **Статус** | ✅ Реализована |

**Код:**
```cpp
int OilVolumeCommandImpl::calculate(const QString &value) {
    return ObdParser::A(value) / 10;
}
```

---

## Структура OBD2-ответа

Все команды датчиков используют режим OBD2 `22` (Read Data By Identifier) с 3-байтным PID.

### Формат ответа (после `ATH1`)

```
22F405 7E8 04 62 F4 05 5C
│    │   │   │  │  │  │
│    │   │   │  │  │  └── 0x5C — контрольная сумма (checksum)
│    │   │   │  │  └───── 0x05 — байт данных (позиция 7 = ObdParser::B)
│    │   │   │  └──────── 0xF4 — байт данных (позиция 6 = ObdParser::A)
│    │   │   └─────────── 0x62 — SID подтверждающего ответа (Positive Response)
│    │   └─────────────── 0x04 — количество данных байт
│    └─────────────────── 0x7E8 — идентификатор ЭБУ (ECU identifier)
└──────────────────────── эхо отправленной команды (22F405)
```

### Разделение позиций

| Токен (по пробелам) | Позиция | Содержание |
|---|---|---|
| 0 | `22F405` | Эхо команды |
| 1 | `7E8` | ECU identifier |
| 2 | `04` | Длина данных |
| 3 | `62` | SID (0x62 = подтверждающий ответ) |
| 4 | `F4` | Параметр байт 1 |
| 5 | `05` | Параметр байт 2 |
| 6 | `5C` | Checksum |

---

## ObdParser — методы извлечения данных

Класс [`src/obd/ObdParser.h`](src/obd/ObdParser.h) — статический утилитарный класс для парсинга hex-строк OBD2-ответов.

### `ObdParser::A(const QString &data)`

Извлекает целочисленное значение из **позиции 6** (шестой токен, разделённый пробелами) — байт данных датчика.

```cpp
// Пример:
//   data = "22F405 7E8 04 62 F4 05 5C"
//   ObdParser::A(data) → 0x5C → 92 (в десятичной)
```

Используется всеми температурными командами и как старший байт в командах давления/уровня.

---

### `ObdParser::B(const QString &data)`

Извлекает целочисленное значение из **позиции 7** — второй байт данных датчика.

```cpp
// Пример:
//   data = "22F405 7E8 04 62 F4 05 5C"
//   ObdParser::B(data) → 0x05 → 5 (в десятичной)
```

Используется как младший байт в командах давления топлива и тормозов.

---

### `ObdParser::getIntAtPos(const QString &data, int pos)`

Общий метод извлечения hex-значения по произвольной позиции (индекс токена, разделённых пробелами).

---

### `ObdParser::extractCommandId(const QString &data)`

Извлекает ID команды из первого токена строки ответа.

```cpp
//   data = "22F405 7E8 04 62 F4 05 5C"
//   ObdParser::extractCommandId(data) → "22F405"
```

---

## Сводная таблица всех команд

| Command ID | Датчик | Класс | Формула | Статус | В цикле |
|---|---|---|---|---|---|
| `ATZ` | — | `ObdResetFixCommandImpl` | — | ✅ | — |
| `ATH1` | — | `DisplayHeaderCommandImpl` | — | ✅ | — |
| `ATSP6` | — | `SelectProtocolObdCommandImpl` | — | ✅ | — |
| `22F405` | Охлаждающая жидкость | `CoolantTempCommandImpl` | `A - 40` | ✅ | ✅ |
| `2203F3` | Масло | `OilTempCommandImpl` | `A - 40` | ✅ | ✅ |
| `221E69` | Коробка передач | `GbTempCommandImpl` | `A - 40` | ✅ | ✅ |
| `22D11C` | Раздатка | `TCTempCommandImpl` | `A - 40` | ✅ | ❌ |
| `221E8A` | Задний дифференциал | `RDTempCommandImpl` | `A - 40` | ✅ | ❌ |
| `22D11A` | Компрессор | `ComprTempCommandImpl` | `A - 40` | ✅ | ✅ |
| `22F40F` | Воздух на впуске | `IntakeAirTempCommandImpl` | `A - 40` | ✅ | ✅ |
| `22F446` | Наружный воздух | `AmbientAirTempCommandImpl` | `A - 40` | ✅ | ✅ |
| `22F423` | Давление топлива | `FuelPressCommandImpl` | `(A × 256 + B) / 100` | ✅ | ❌ |
| `222B0D` | Тормозная система | `BrakePressCommandImpl` | `A × 8533.3337 + B × 33.33` | ✅ | ❌ |
| `22F42F` | Топливо (литры) | `FuelLevelCommandImpl` | `(A × 100) / 255` | ✅ | ❌ |
| `2203E6` | Уровень масла | `OilLevelCommandImpl` | `A` | ✅ | ❌ |
| `2203F2` | Объём масла | `OilVolumeCommandImpl` | `A / 10` | ✅ | ❌ |

Легенда:
- ✅ — реализована
- ⚪ — заглушка (stub, без реализации `calculate()`)
- — — не применимо (команда управления, не датчик)
