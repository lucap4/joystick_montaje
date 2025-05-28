#include <BleGamepad.h>

#define NUM_BUTTONS 16

int buttonPins[NUM_BUTTONS] = {
  2, 3, 4, 5,      // XYAB
  6, 7, 8, 9,      // Cruceta
  10, 11,          // Select, Options
  12, 13, 14, 15,  // L1, R1, L2, R2
  16, 17           // L3, R3
};

#define JOY_L_X 18
#define JOY_L_Y 19
#define JOY_R_X 20
#define JOY_R_Y 21

BleGamepad bleGamepad;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE Gamepad");

  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  bleGamepad.begin();
}

void loop() {
  if (bleGamepad.isConnected()) {
    // Leer botones
    for (int i = 0; i < NUM_BUTTONS; i++) {
      bool pressed = digitalRead(buttonPins[i]) == LOW;
      if (pressed) {
        bleGamepad.press(i + 1);  // BUTTON_1 to BUTTON_16
      } else {
        bleGamepad.release(i + 1);
      }
    }

    // Leer ejes
    int lx = analogRead(JOY_L_X);
    int ly = analogRead(JOY_L_Y);
    int rx = analogRead(JOY_R_X);
    int ry = analogRead(JOY_R_Y);

    // Mapear a rango -32767 a 32767
    lx = map(lx, 0, 4095, -32767, 32767);
    ly = map(ly, 0, 4095, -32767, 32767);
    rx = map(rx, 0, 4095, -32767, 32767);
    ry = map(ry, 0, 4095, -32767, 32767);

    // Enviar a BLE
    bleGamepad.setAxes(lx, ly, 0, rx, ry, 0, 0, 0); // X, Y, Z, RX, RY, RZ, slider1, slider2

    delay(10);
  }
}
