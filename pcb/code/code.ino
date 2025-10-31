// Define the specific pins for your ESP32-C3 SuperMini
const int dataPin  = 5;  // DIN on MAX7219 (Your MOSI pin)
const int csPin    = 10; // CS/LOAD on MAX7219
const int clockPin = 4;  // CLK on MAX7219 (Your SCK pin)

// --- MAX7219 Command Registers ---
const byte REG_DECODE_MODE   = 0x09;
const byte REG_INTENSITY  = 0x0A;
const byte REG_SCAN_LIMIT    = 0x0B;
const byte REG_SHUTDOWN      = 0x0C;
const byte REG_DISPLAY_TEST  = 0x0F;

const byte DIGITS[] = {
  0x7E, 0x30, 0x6D, 0x79, 0x33,
  0x5B, 0x5F, 0x70, 0x7F, 0x7B
  };

void setup() {
  // Set our control pins as outputs
  pinMode(dataPin, OUTPUT);
  pinMode(csPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  // --- Initialize the MAX7219 ---
  
  // 1. Take the chip out of shutdown mode to turn it on.
  sendCommand(REG_SHUTDOWN, 1); // 1 = Normal Operation

  // 2. Set the scan limit (how many digits to display). 7 means all 8 digits (0-7).
  sendCommand(REG_SCAN_LIMIT, 4);

  // 3. Disable font decoding, as we are controlling it directly.
  sendCommand(REG_DECODE_MODE, 0);
  
  sendCommand(REG_INTENSITY, 0x01);

  // --- GOAL: Put the display in test mode ---
  // This command tells the chip to turn on all LEDs.
  sendCommand(REG_DISPLAY_TEST, 1); // 1 = Test Mode ON
  delay(1000);
  sendCommand(REG_DISPLAY_TEST, 0); // 1 = Test Mode ON
  delay(1000);
}

int i = 0;
int int_s = 0;
void loop() {
  sendCommand(1, DIGITS[(i + 0) % 10]); // 1 is digit 0
  sendCommand(2, DIGITS[(i + 1) % 10]);
  sendCommand(3, DIGITS[(i + 2) % 10]);
  sendCommand(4, DIGITS[(i + 3) % 10]);
  sendCommand(5, DIGITS[(i + 4) % 10]);
  i = (i + 1) % 10;
  delay(1000);
}

// (8-bit address, 8-bit data)
void sendCommand(byte register_addr, byte data) {
  // start command
  digitalWrite(csPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, register_addr);
  shiftOut(dataPin, clockPin, MSBFIRST, data);
  // end command
  digitalWrite(csPin, HIGH);
}
