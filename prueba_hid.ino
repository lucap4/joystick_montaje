// Requiere tener instalada la librería TinyUSB para Arduino
#include <Adafruit_TinyUSB.h>

// Configuración de pines (modificá estos según tu plaqueta)
const int buttons[] = {2, 3, 4, 5,   // XYBA
                       6, 7, 8, 9,   // Cruceta
                       10, 11,       // Select, Start
                       12, 13, 14, 15}; // L1, R1, L2, R2

const int numButtons = sizeof(buttons) / sizeof(buttons[0]);

// Simulación de sticks analógicos en los pines 16 (X) y 17 (Y)
const int stickX = 16;
const int stickY = 17;

// HID Joystick report (32 bytes deberían ser más que suficiente para este caso)
uint8_t report[8] = {0};

Adafruit_USBD_HID usb_hid;

void setup() {
  Serial.begin(115200);

  // Inicializar botones como entrada con pull-up
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
  }

  // Inicializar pines de joystick
  pinMode(stickX, INPUT);
  pinMode(stickY, INPUT);

  // Iniciar HID con un descriptor de joystick genérico
  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(NULL, 0); // Por defecto es teclado, redefinimos más abajo
  usb_hid.begin();

  // Esperar a que esté montado
  while (!TinyUSBDevice.mounted()) delay(10);
}

void loop() {
  // Leer botones
  for (int i = 0; i < numButtons; i++) {
    bitWrite(report[0], i, !digitalRead(buttons[i])); // Botones presionados = 1
  }

  // Leer joystick (simulamos con valores digitales por ahora)
  int x = analogRead(stickX) / 4;  // escalar a 0-255
  int y = analogRead(stickY) / 4;
  report[1] = x;
  report[2] = y;

  // Enviar reporte
  usb_hid.sendReport(0, report, sizeof(report));
  delay(10);
}
