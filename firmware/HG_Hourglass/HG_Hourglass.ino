/*
  Softera Lab · Hourglass Solder Kit
  Demo: dual 8×8 MAX7219 matrices + MPU-6050 gravity sand

  Arduino IDE:
    Board …… Arduino Nano / ATmega328P (or matching core)
    Port ……… your USB-UART / ISP as used on the kit

  Matrix SoftSPI (change if your PCB routing differs):
    DIN = D11 · CS = D10 · CLK = D13 · 2 devices daisy-chained
    Device 0 = top chamber · Device 1 = bottom chamber

  MPU-6050 on I2C (Wire):
    SDA / SCL — board default (A4 / A5 on classic Nano pinout)

  Buttons (optional, active LOW with INPUT_PULLUP):
    BTN_A = D2 · BTN_B = D3
    Short press A = refill sand · B = toggle speed

  Buzzer (optional):
    BUZZ = D4
*/

#include <Wire.h>
#include "SoftteraMax7219.h"

static const uint8_t PIN_DIN = 11;
static const uint8_t PIN_CS  = 10;
static const uint8_t PIN_CLK = 13;
static const uint8_t NUM_DEVICES = 2;

static const uint8_t PIN_BTN_A = 2;
static const uint8_t PIN_BTN_B = 3;
static const uint8_t PIN_BUZZ  = 4;

static const uint8_t MPU_ADDR = 0x68;

SoftteraMax7219 matrix(PIN_DIN, PIN_CS, PIN_CLK, NUM_DEVICES);

// Sand: up to 64 grains total across both chambers
static const uint8_t MAX_GRAINS = 48;
struct Grain {
  int8_t x;   // 0..7 within chamber
  int8_t y;   // 0..7 within chamber
  uint8_t chamber; // 0 top, 1 bottom
  bool alive;
};
Grain grains[MAX_GRAINS];

uint8_t fallDelayMs = 55;
int8_t gravitySign = 1; // +1 = sand falls toward chamber 1 visually "down"
uint32_t lastFall = 0;
uint32_t lastOrient = 0;

bool mpuOk = false;

void buzz(uint16_t ms) {
  pinMode(PIN_BUZZ, OUTPUT);
  digitalWrite(PIN_BUZZ, HIGH);
  delay(ms);
  digitalWrite(PIN_BUZZ, LOW);
}

bool mpuBegin() {
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); // PWR_MGMT_1
  Wire.write(0x00); // wake
  if (Wire.endTransmission() != 0) return false;
  delay(10);
  return true;
}

bool mpuReadAccel(int16_t &ax, int16_t &ay, int16_t &az) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom((int)MPU_ADDR, 6) != 6) return false;
  ax = (Wire.read() << 8) | Wire.read();
  ay = (Wire.read() << 8) | Wire.read();
  az = (Wire.read() << 8) | Wire.read();
  return true;
}

void clearGrains() {
  for (uint8_t i = 0; i < MAX_GRAINS; i++) grains[i].alive = false;
}

void spawnTopPile(uint8_t count) {
  clearGrains();
  uint8_t n = 0;
  // Fill lower half of top chamber (toward the neck)
  for (int8_t y = 4; y < 8 && n < count; y++) {
    for (int8_t x = 0; x < 8 && n < count; x++) {
      // diamond-ish pile: denser near center bottom of top matrix
      int8_t dx = x - 3;
      if (dx < 0) dx = -dx;
      if (dx <= (y - 3)) {
        grains[n].x = x;
        grains[n].y = y;
        grains[n].chamber = 0;
        grains[n].alive = true;
        n++;
      }
    }
  }
  while (n < count && n < MAX_GRAINS) {
    grains[n].x = (int8_t)(n % 8);
    grains[n].y = 7;
    grains[n].chamber = 0;
    grains[n].alive = true;
    n++;
  }
}

bool cellOccupied(uint8_t chamber, int8_t x, int8_t y, uint8_t self) {
  if (x < 0 || x > 7 || y < 0 || y > 7) return true;
  for (uint8_t i = 0; i < MAX_GRAINS; i++) {
    if (i == self || !grains[i].alive) continue;
    if (grains[i].chamber == chamber && grains[i].x == x && grains[i].y == y) return true;
  }
  return false;
}

void render() {
  matrix.clear();
  for (uint8_t i = 0; i < MAX_GRAINS; i++) {
    if (!grains[i].alive) continue;
    // Map chamber → device index; x across devices for SoftteraMax7219
    int16_t gx = grains[i].x + (int16_t)grains[i].chamber * 8;
    matrix.setPixel(gx, grains[i].y, true);
  }
  matrix.show();
}

void tryFallGrain(uint8_t i) {
  Grain &g = grains[i];
  if (!g.alive) return;

  // Gravity toward +Y when gravitySign > 0 (normal upright)
  int8_t dy = gravitySign;
  int8_t nx = g.x;
  int8_t ny = g.y + dy;
  uint8_t ch = g.chamber;

  if (dy > 0) {
    // falling toward bottom of current chamber
    if (ny > 7) {
      if (ch == 0) {
        // pass through neck into bottom chamber (top rows)
        ch = 1;
        ny = 0;
        nx = 3 + (int8_t)(random(0, 2)); // slight scatter
      } else {
        return; // rest at bottom
      }
    }
  } else {
    // inverted: falling toward y=0, then into other chamber
    if (ny < 0) {
      if (ch == 1) {
        ch = 0;
        ny = 7;
        nx = 3 + (int8_t)(random(0, 2));
      } else {
        return;
      }
    }
  }

  if (!cellOccupied(ch, nx, ny, i)) {
    g.chamber = ch;
    g.x = nx;
    g.y = ny;
    return;
  }

  // slide left/right
  int8_t side = (random(0, 2) == 0) ? -1 : 1;
  if (!cellOccupied(ch, nx + side, ny, i)) {
    g.chamber = ch;
    g.x = (int8_t)(nx + side);
    g.y = ny;
    return;
  }
  if (!cellOccupied(ch, nx - side, ny, i)) {
    g.chamber = ch;
    g.x = (int8_t)(nx - side);
    g.y = ny;
  }
}

void updateOrientation() {
  if (!mpuOk) return;
  int16_t ax, ay, az;
  if (!mpuReadAccel(ax, ay, az)) return;

  // Board vertical: dominant axis depends on mounting.
  // Use the axis with largest magnitude in the board plane; default ay.
  int16_t primary = ay;
  if (abs(ax) > abs(ay) && abs(ax) > abs(az)) primary = ax;
  else if (abs(az) > abs(ay)) primary = az;

  int8_t newSign = (primary >= 0) ? 1 : -1;
  if (newSign != gravitySign) {
    gravitySign = newSign;
    buzz(25);
  }
}

bool buttonPressed(uint8_t pin) {
  if (digitalRead(pin) == LOW) {
    delay(25);
    if (digitalRead(pin) == LOW) {
      while (digitalRead(pin) == LOW) delay(5);
      return true;
    }
  }
  return false;
}

void setup() {
  pinMode(PIN_BTN_A, INPUT_PULLUP);
  pinMode(PIN_BTN_B, INPUT_PULLUP);
  pinMode(PIN_BUZZ, OUTPUT);
  digitalWrite(PIN_BUZZ, LOW);

  matrix.begin(3);
  matrix.clear();

  mpuOk = mpuBegin();
  spawnTopPile(40);
  render();
  buzz(40);
}

void loop() {
  uint32_t now = millis();

  if (now - lastOrient > 120) {
    lastOrient = now;
    updateOrientation();
  }

  if (buttonPressed(PIN_BTN_A)) {
    spawnTopPile(40);
    render();
    buzz(30);
  }
  if (buttonPressed(PIN_BTN_B)) {
    fallDelayMs = (fallDelayMs <= 35) ? 90 : (uint8_t)(fallDelayMs - 20);
    buzz(15);
  }

  if (now - lastFall >= fallDelayMs) {
    lastFall = now;
    // Random order so piles settle naturally
    for (uint8_t k = 0; k < MAX_GRAINS; k++) {
      tryFallGrain((uint8_t)random(0, MAX_GRAINS));
    }
    render();
  }
}
