#ifndef DISCODIAG_OBDPARSER_H
#define DISCODIAG_OBDPARSER_H

#include <QString>
#include <QStringList>

class ObdParser {
  // ELM327 responses can contain the command echo, the ECU header and the
  // trailing ">" prompt. Token positions therefore must not be hard-coded
  // without normalising the frame first.
  //
  // Examples of the same response:
  //   22F405 7E8 04 62 F4 05 5C
  //   22F405
  //   7E8 04 62 F4 05 5C
  //
  // All sensor commands use OBD mode 22 (Read Data By Identifier), so the
  // positive response marker 0x62 followed by the two PID bytes is a reliable
  // anchor for the payload.
public:
  static int A(const QString &data) { return payloadByte(data, 0); }

  static int B(const QString &data) { return payloadByte(data, 1); }

  static int C(const QString &data) { return payloadByte(data, 2); }

  // Reinterprets an unsigned byte 0..255 as a two's-complement signed value
  // -128..127. Needed for sensors whose most-significant byte is signed
  // (e.g. brake pressure 222B0D, longitudinal acceleration 222B12).
  static int SIGNED(int v) { return v > 127 ? v - 256 : v; }

  static int getIntAtPos(const QString &data, int pos) {
    const auto tokens = tokenize(data);
    return intAt(tokens, pos);
  }

  static QString extractCommandId(const QString &data) {
    const auto tokens = tokenize(data);
    return tokens.isEmpty() ? QString() : tokens.first();
  }

  // Returns true when the frame contains a positive mode 22 response and at
  // least one data byte.
  static bool hasPayload(const QString &data) {
    const auto tokens = tokenize(data);
    return payloadStart(tokens) >= 0;
  }

private:
  static QStringList tokenize(const QString &data) {
    QString normalized = data;
    normalized.remove(QLatin1Char('>'));
    return normalized.simplified().split(QLatin1Char(' '), Qt::SkipEmptyParts);
  }

  static int intAt(const QStringList &tokens, int pos) {
    if (pos < 0 || pos >= tokens.size()) {
      return -1;
    }
    bool ok = false;
    const auto value = tokens.at(pos).toInt(&ok, 16);
    return ok ? value : -1;
  }

  // Index of the first data byte after "62 <pid hi> <pid lo>".
  static int payloadStart(const QStringList &tokens) {
    for (int i = 0; i + 3 < tokens.size(); ++i) {
      if (intAt(tokens, i) == 0x62) {
        return i + 3;
      }
    }
    return -1;
  }

  static int payloadByte(const QString &data, int index) {
    const auto tokens = tokenize(data);
    const int start = payloadStart(tokens);
    if (start < 0) {
      return -1;
    }
    return intAt(tokens, start + index);
  }
};

#endif // DISCODIAG_OBDPARSER_H
