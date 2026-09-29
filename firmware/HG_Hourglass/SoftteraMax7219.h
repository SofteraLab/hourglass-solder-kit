/*
  Softtera Lab · SK-65
  Minimal MAX7219 driver for ATmega328 / Arduino Uno / Nano (5 V).

  Default wiring (change pins in the sketch if needed):
    DIN → D11
    CS  → D10
    CLK → D13
    VCC → 5V
    GND → GND

  For a daisy-chain, set deviceCount to the number of modules
  (DOUT of the first board → DIN of the next).
*/

#pragma once
#include <Arduino.h>

class SoftteraMax7219 {
public:
  SoftteraMax7219(uint8_t dinPin, uint8_t csPin, uint8_t clkPin, uint8_t deviceCount = 1)
    : _din(dinPin), _cs(csPin), _clk(clkPin), _devices(deviceCount) {}

  void begin(uint8_t intensity = 4) {
    pinMode(_din, OUTPUT);
    pinMode(_cs, OUTPUT);
    pinMode(_clk, OUTPUT);
    digitalWrite(_cs, HIGH);
    digitalWrite(_clk, LOW);
    digitalWrite(_din, LOW);

    for (uint8_t d = 0; d < _devices; d++) {
      writeAll(0x0C, 0x00); // shutdown while init
    }
    writeAll(0x0F, 0x00); // display test off
    writeAll(0x09, 0x00); // no decode (matrix mode)
    writeAll(0x0B, 0x07); // scan all 8 digits
    setIntensity(intensity);
    clear();
    writeAll(0x0C, 0x01); // normal operation
  }

  void setIntensity(uint8_t value) {
    if (value > 15) value = 15;
    writeAll(0x0A, value);
  }

  uint8_t deviceCount() const { return _devices; }

  void clear() {
    for (uint8_t row = 0; row < 8; row++) {
      for (uint8_t d = 0; d < _devices; d++) {
        _buf[d][row] = 0;
      }
    }
    show();
  }

  void setPixel(int16_t x, int16_t y, bool on) {
    if (y < 0 || y > 7) return;
    if (x < 0) return;
    uint8_t device = x / 8;
    uint8_t col = x % 8;
    if (device >= _devices) return;
    uint8_t mask = 1 << (7 - col);
    if (on) _buf[device][y] |= mask;
    else _buf[device][y] &= ~mask;
  }

  bool getPixel(int16_t x, int16_t y) const {
    if (y < 0 || y > 7 || x < 0) return false;
    uint8_t device = x / 8;
    uint8_t col = x % 8;
    if (device >= _devices) return false;
    return (_buf[device][y] & (1 << (7 - col))) != 0;
  }

  void setRow(uint8_t device, uint8_t row, uint8_t value) {
    if (device >= _devices || row > 7) return;
    _buf[device][row] = value;
  }

  uint8_t getRow(uint8_t device, uint8_t row) const {
    if (device >= _devices || row > 7) return 0;
    return _buf[device][row];
  }

  void fill(bool on = true) {
    uint8_t v = on ? 0xFF : 0x00;
    for (uint8_t d = 0; d < _devices; d++) {
      for (uint8_t r = 0; r < 8; r++) _buf[d][r] = v;
    }
    show();
  }

  void show() {
    for (uint8_t row = 0; row < 8; row++) {
      digitalWrite(_cs, LOW);
      for (int8_t d = _devices - 1; d >= 0; d--) {
        shiftOutByte(row + 1);
        shiftOutByte(_buf[d][row]);
      }
      digitalWrite(_cs, HIGH);
    }
  }

private:
  static const uint8_t kMaxDevices = 8;
  uint8_t _din, _cs, _clk, _devices;
  uint8_t _buf[kMaxDevices][8];

  void shiftOutByte(uint8_t value) {
    for (int8_t i = 7; i >= 0; i--) {
      digitalWrite(_din, (value >> i) & 0x01);
      digitalWrite(_clk, HIGH);
      digitalWrite(_clk, LOW);
    }
  }

  void writeAll(uint8_t reg, uint8_t data) {
    digitalWrite(_cs, LOW);
    for (uint8_t d = 0; d < _devices; d++) {
      shiftOutByte(reg);
      shiftOutByte(data);
    }
    digitalWrite(_cs, HIGH);
  }
};
