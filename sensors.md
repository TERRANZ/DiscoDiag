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
    virtual int calculate(const QString &value) const;
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
inline const QString CMD_TEMP_COOLANT = QStringLiteral("22F405");       // Температура охлаждающей жидкости
inline const QString CMD_TEMP_OIL = QStringLiteral("2203F3");            // Температура масла
inline const QString CMD_TEMP_GB = QStringLiteral("221E69");             // Температура коробки передач
inline const QString CMD_TEMP_TC = QStringLiteral("22D11C");             // Температура раздаточной коробки
inline const QString CMD_TEMP_RD = QStringLiteral("221E8A");             // Температура заднего дифференциала
inline const QString CMD_TEMP_COMPR = QStringLiteral("22D11A");          // Температура компрессора
inline const QString CMD_TEMP_AIR_INTAKE = QStringLiteral("22F40F");     // Температура воздуха на впуске
inline const QString CMD_TEMP_AIR_AMBIENT = QStringLiteral("22F446");    // Температура наружного воздуха
inline const QString CMD_PRESS_FUEL = QStringLiteral("22F423");          // Давление топлива
inline const QString CMD_PRESS_BRAKE = QStringLiteral("222B0D");         // Давление тормозной системы
inline const QString CMD_LVL_OIL_VOL = QStringLiteral("2203F2");         // Объём масла
inline const QString CMD_LVL_OIL = QStringLiteral("2203E6");             // Уровень масла
inline const QString CMD_LVL_FUEL_LITRES = QStringLiteral("22F42F");     // Уровень топлива (литры)
inline const QString CMD_BOOST = QStringLiteral("22033E");               // Наддув
inline const QString CMD_INT_PRES = QStringLiteral("22F40B");            // Абс. давление в коллекторе
inline const QString CMD_SPEED = QStringLiteral("22F40D");               // Скорость автомобиля
inline const QString CMD_ABS_FL = QStringLiteral("222B06");              // ABS скор. перед. лев.
inline const QString CMD_ABS_FR = QStringLiteral("222B07");              // ABS скор. перед. прав.
inline const QString CMD_ABS_RL = QStringLiteral("222B08");              // ABS скор. зад. лев.
inline const QString CMD_ABS_RR = QStringLiteral("222B09");              // ABS скор. зад. прав.
inline const QString CMD_HEIGHT_FL = QStringLiteral("223B03");           // Высота подвески FL
inline const QString CMD_HEIGHT_FR = QStringLiteral("223B04");           // Высота подвески FR
inline const QString CMD_HEIGHT_RL = QStringLiteral("223B05");           // Высота подвески RL
inline const QString CMD_HEIGHT_RR = QStringLiteral("223B06");           // Высота подвески RR
inline const QString CMD_MAIN_PRES = QStringLiteral("223B07");           // Магистральное давление
inline const QString CMD_U_TRCASE = QStringLiteral("22197C");            // Напряжение датч. раздатки
inline const QString CMD_U_ABS = QStringLiteral("22D112");               // Напряжение блока ABS
inline const QString CMD_MAF = QStringLiteral("22F410");                 // Массовый расход воздуха
inline const QString CMD_MILEAGE = QStringLiteral("22DD01");             // Общий пробег
inline const QString CMD_ACCELER = QStringLiteral("222B12");             // Продольное ускорение
inline const QString CMD_RDIF_LOCK = QStringLiteral("22D120");           // Блокировка зад. диф.
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

**Описание:** Устанавливает протокол ELM327 №6 — ISO 15765-4 CAN (11-битный идентификатор, 500 кбит/с). Это штатный протокол для L322 2010+. Отправляется последним в последовательности инициализации; после успешного выполнения начинается циклический опрос датчиков.

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

Где `ObdParser::A(value)` извлекает первый байт данных после маркера `62 <PID hi> <PID lo>`.

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
int CoolantTempCommandImpl::calculate(const QString &value) const {
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
int OilTempCommandImpl::calculate(const QString &value) const {
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
int GbTempCommandImpl::calculate(const QString &value) const {
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
| **Статус** | ✅ Реализована, в цикле опроса |

**Код:**
```cpp
int TCTempCommandImpl::calculate(const QString &value) const {
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
| **Статус** | ✅ Реализована, в цикле опроса |

**Код:**
```cpp
int RDTempCommandImpl::calculate(const QString &value) const {
    return ObdParser::A(value) - 40;
}
```

---

#### 22D11A — Температура компрессора

| Параметр | Значение |
|---|---|
| **Command ID** | `22D11A` |
| **Имя** | `Compressor temp command` |
| **Переменная в ObdResult** | `tempCOMPR` |
| **Реализация** | [`src/command/impl/lr322/temperature/ComprTempCommandImpl.h`](src/command/impl/lr322/temperature/ComprTempCommandImpl.h) · [`ComprTempCommandImpl.cpp`](src/command/impl/lr322/temperature/ComprTempCommandImpl.cpp) |
| **Статус** | ✅ Реализована, в цикле опроса |

**Код:**
```cpp
int ComprTempCommandImpl::calculate(const QString &value) const {
    return ObdParser::A(value) - 40;
}
```

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
int IntakeAirTempCommandImpl::calculate(const QString &value) const {
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
int AmbientAirTempCommandImpl::calculate(const QString &value) const {
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
| **Статус** | ✅ Реализована, в цикле опроса |

**Формула:**

```
result = ((ObdParser::A(value) * 256) + ObdParser::B(value)) / 100
```

Где:
- `ObdParser::A(value)` — первый байт данных (старший байт)
- `ObdParser::B(value)` — второй байт данных (младший байт)

Метод объединяет два байта в 16-битное значение, делит на 100. Результат — давление в kPa (с двумя знаками после запятой, но возвращается как `int`).

**Код:**
```cpp
int FuelPressCommandImpl::calculate(const QString &value) const {
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
| **Статус** | ✅ Реализована, в цикле опроса |

**Формула:**

```
result = ObdParser::SIGNED(ObdParser::A(value)) * 8533.3337 + ObdParser::B(value) * 33.33
```

Где:
- `ObdParser::A(value)` — первый байт данных, **знаковый** (дополнительный код), переведённый через `SIGNED()` в диапазон -128..127
- `ObdParser::B(value)` — второй байт данных (коэффициент масштабирования младшего байта)

Результат — давление в kPa.

**Код:**
```cpp
int BrakePressCommandImpl::calculate(const QString &value) const {
    return ObdParser::SIGNED(ObdParser::A(value)) * 8533.3337 +
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
| **Статус** | ✅ Реализована, в цикле опроса |

**Формула:**

```
result = (ObdParser::A(value) * 105) / 255
```

Где:
- `ObdParser::A(value)` — сырое значение (0–255)

Метод конвертирует 8-битное сырое значение датчика уровня топлива в литры. Диапазон: 0–105 литров (при сырых 0–255) — соответствует ёмкости бака в 105 л.

**Код:**
```cpp
int FuelLevelCommandImpl::calculate(const QString &value) const {
    return (ObdParser::A(value) * 105) / 255;
}
```

---

#### 2203E6 — Уровень масла

| Параметр | Значение |
|---|---|
| **Command ID** | `2203E6` |
| **Имя** | `Oil level command` |
| **Переменная в ObdResult** | `lvlOil` |
| **Реализация** | [`src/command/impl/lr322/OilLevelCommandImpl.h`](src/command/impl/lr322/OilLevelCommandImpl.h) · [`OilLevelCommandImpl.cpp`](src/command/impl/lr322/OilLevelCommandImpl.cpp) |
| **Статус** | ✅ Реализована, в цикле опроса |

**Код:**
```cpp
int OilLevelCommandImpl::calculate(const QString &value) const {
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
| **Статус** | ✅ Реализована, в цикле опроса |

**Код:**
```cpp
int OilVolumeCommandImpl::calculate(const QString &value) const {
    return ObdParser::A(value) / 10;
}
```

---

## Структура OBD2-ответа

Все команды датчиков используют режим OBD2 `22` (Read Data By Identifier) с 2-байтным PID (DID).

### Формат ответа (после `ATH1`)

ELM327 может добавлять эхо команды и переводы строк; `BtBackend` разбивает
поток по терминатору `>`, а `ObdParser` нормализует пробелы и находит маркер
`62`.

```
22F405 7E8 04 62 F4 05 5C
│      │   │  │  │  │  └── 0x5C — первый байт данных (A)
│      │   │  │  │  └───── 0x05 — младший байт PID
│      │   │  │  └──────── 0xF4 — старший байт PID
│      │   │  └─────────── 0x62 — SID положительного ответа на mode 22
│      │   └────────────── 0x04 — количество байт данных
│      └────────────────── 0x7E8 — идентификатор ЭБУ (ECU identifier)
└───────────────────────── эхо отправленной команды (22F405)
```

`ObdParser` не опирается на абсолютные индексы: он находит токен `62`,
пропускает два байта PID и читает следующий байт как `A`, а следующий за ним
как `B`. Поэтому парсер работает и без эха, и с заголовками ЭБУ.

### Разделение позиций

| Токен (по пробелам) | Пример | Содержание |
|---|---|---|
| 0 | `22F405` | Эхо команды (может отсутствовать) |
| 1 | `7E8` | ECU identifier (может отсутствовать) |
| 2 | `04` | Длина данных |
| 3 | `62` | SID (0x62 = положительный ответ) |
| 4 | `F4` | Старший байт PID |
| 5 | `05` | Младший байт PID |
| 6 | `5C` | Первый байт данных (`A`) |
| 7 | … | Второй байт данных (`B`), если есть |

---

## ObdParser — методы извлечения данных

Класс [`src/obd/ObdParser.h`](src/obd/ObdParser.h) — статический утилитарный класс для парсинга hex-строк OBD2-ответов.

### `ObdParser::A(const QString &data)`

Извлекает первый байт данных после `62 <PID hi> <PID lo>`.

```cpp
// Пример:
//   data = "22F405 7E8 04 62 F4 05 5C"
//   ObdParser::A(data) → 0x5C → 92 (в десятичной)
```

Используется всеми температурными командами, уровнем/объёмом масла и как
старший байт в командах давления.

---

### `ObdParser::B(const QString &data)`

Извлекает второй байт данных после `62 <PID hi> <PID lo>`.

```cpp
// Пример:
//   data = "22F423 7E8 04 62 F4 23 03 E8"
//   ObdParser::A(data) → 0x03
//   ObdParser::B(data) → 0xE8 → 232 (в десятичной)
```

Используется как младший байт в командах давления топлива и тормозов.
Если второго байта нет, возвращается `-1`.

---

### `ObdParser::C(const QString &data)`

Извлекает третий байт данных после `62 <PID hi> <PID lo>`.

```cpp
// Пример:
//   data = "22DD01 7E8 04 62 DD 01 01 02 03"
//   ObdParser::C(data) → 0x03 → 3 (в десятичной)
```

Используется пробегом (`22DD01`), где значение занимает три байта. Если
третьего байта нет, возвращается `-1`.

---

### `ObdParser::SIGNED(int v)`

Конвертирует беззнаковый байт `0..255` в знаковое значение `-128..127`
(дополнительный код). Чистая функция, не читает данные ответа.

```cpp
ObdParser::SIGNED(0)   → 0
ObdParser::SIGNED(127) → 127
ObdParser::SIGNED(128) → -128
ObdParser::SIGNED(255) → -1
```

Используется там, где старший байт датчика знаковый: давление тормозной
системы (`222B0D`) и продольное ускорение (`222B12`). Применяется как
`ObdParser::SIGNED(ObdParser::A(value))`.

---

### `ObdParser::getIntAtPos(const QString &data, int pos)`

Общий метод извлечения hex-значения по произвольной позиции (индекс токена,
разделённого пробелами). Для выхода за границы и невалидного hex возвращает
`-1`.

---

### `ObdParser::hasPayload(const QString &data)`

Возвращает `true`, если в ответе найден положительный ответ `62` и после
двух байтов PID есть хотя бы один байт данных.

---

### `ObdParser::extractCommandId(const QString &data)`

Извлекает ID команды из первого непустого токена строки ответа.

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
| `22D11C` | Раздатка | `TCTempCommandImpl` | `A - 40` | ✅ | ✅ |
| `221E8A` | Задний дифференциал | `RDTempCommandImpl` | `A - 40` | ✅ | ✅ |
| `22D11A` | Компрессор | `ComprTempCommandImpl` | `A - 40` | ✅ | ✅ |
| `22F40F` | Воздух на впуске | `IntakeAirTempCommandImpl` | `A - 40` | ✅ | ✅ |
| `22F446` | Наружный воздух | `AmbientAirTempCommandImpl` | `A - 40` | ✅ | ✅ |
| `22F423` | Давление топлива | `FuelPressCommandImpl` | `(A × 256 + B) / 100` | ✅ | ✅ |
| `222B0D` | Тормозная система | `BrakePressCommandImpl` | `SIGNED(A) × 8533.3337 + B × 33.33` | ✅ | ✅ |
| `22F42F` | Топливо (литры) | `FuelLevelCommandImpl` | `(A × 105) / 255` | ✅ | ✅ |
| `2203E6` | Уровень масла | `OilLevelCommandImpl` | `A` | ✅ | ✅ |
| `2203F2` | Объём масла | `OilVolumeCommandImpl` | `A / 10` | ✅ | ✅ |
| `22033E` | Наддув | `BoostCommandImpl` | `A` | ✅ | ✅ |
| `22F40B` | Абс. давление в коллекторе | `IntPressCommandImpl` | `A` | ✅ | ✅ |
| `22F40D` | Скорость автомобиля | `SpeedCommandImpl` | `A` | ✅ | ✅ |
| `222B06` | ABS скор. перед. лев. | `AbsFLCommandImpl` | `A` | ✅ | ✅ |
| `222B07` | ABS скор. перед. прав. | `AbsFRCommandImpl` | `A` | ✅ | ✅ |
| `222B08` | ABS скор. зад. лев. | `AbsRLCommandImpl` | `A` | ✅ | ✅ |
| `222B09` | ABS скор. зад. прав. | `AbsRRCommandImpl` | `A` | ✅ | ✅ |
| `223B03` | Высота подвески FL | `HeightFLCommandImpl` | `(A × 256 + B) / 10 - 20` | ✅ | ✅ |
| `223B04` | Высота подвески FR | `HeightFRCommandImpl` | `(A × 256 + B) / 10 - 20` | ✅ | ✅ |
| `223B05` | Высота подвески RL | `HeightRLCommandImpl` | `(A × 256 + B) / 10 - 20` | ✅ | ✅ |
| `223B06` | Высота подвески RR | `HeightRRCommandImpl` | `(A × 256 + B) / 10 - 20` | ✅ | ✅ |
| `223B07` | Магистральное давление | `MainPressCommandImpl` | `A × 256 + B` | ✅ | ✅ |
| `22197C` | Напряжение датч. раздатки | `UTrCaseCommandImpl` | `A / 4` | ✅ | ✅ |
| `22D112` | Напряжение блока ABS | `UAbsCommandImpl` | `A × 20.4 / 255` | ✅ | ✅ |
| `22F410` | Массовый расход воздуха | `MafCommandImpl` | `A × 25.6 + B × 0.01` | ✅ | ✅ |
| `22DD01` | Общий пробег | `MileageCommandImpl` | `A × 65536 + B × 256 + C` | ✅ | ✅ |
| `222B12` | Продольное ускорение | `AccelerCommandImpl` | `SIGNED(A) × 32.8 / 255 + B / 100` | ✅ | ✅ |
| `22D120` | Блокировка зад. диф. | `RDifLockCommandImpl` | `1 - ((A >> 1) & 1)` | ✅ | ✅ |

Легенда:
- ✅ — реализована
- ⚪ — заглушка (stub, без реализации `calculate()`)
- — — не применимо (команда управления, не датчик)
