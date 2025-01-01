#include "util.h"
#include "sensor.h"
#include "RTC.h"
#include "display.h"

const char* TOTP_KEY = "Ihaveahouse";
rgb_lcd lcd;
Keypad keypad = Keypad(makeKeymap(KEYPAD), rowPins, colPins, ROWS, COLS);

void setup() {
  pinMode(12,INPUT);
  lcd.begin(16, 2);
  Serial.begin(9600);
}
void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Password");
  lcd.setCursor(0, 1);
  long x = keypad_get(6, lcd, keypad);
  Serial.print(x);
  delay(1000);
}