#include "util.h"
#include "RTC.h"
#include "display.h"
#include "TOTP.h"
#include <math.h>
uint8_t TOTP_KEY[] = { 0x4d, 0x79, 0x4c, 0x65, 0x67, 0x6f, 0x44, 0x6f, 0x6f, 0x72 };
rgb_lcd lcd;
RTCTime rtime, estimated(0);
Keypad keypad = Keypad(makeKeymap(KEYPAD), rowPins, colPins, ROWS, COLS);
TOTP totp = TOTP(TOTP_KEY, 10);
unsigned char tries = 0, expon = 9;
void setup() {
  pinMode(12, INPUT);
  lcd.begin(16, 2);
  Serial.begin(9600);
  RTC.begin();
  Serial.print("Get time...");
  get_current_time(rtime, "Caviar-X", "Yu2008..@");
  RTC.setTime(rtime);
}
void loop() {
  RTC.getTime(rtime);
  long long lapse = estimated.getUnixTime() - rtime.getUnixTime();
  Serial.print("Lapse: ");
  Serial.print(lapse);
  Serial.print(" tries: ");
  Serial.println(tries);
  if (lapse > 0) {
    Serial.println("Lapse Triggered");
    lcd.setRGB(255, 0, 0);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Device Locking");
    lcd.setCursor(0, 1);
    String s((unsigned int)lapse);
    lcd.print(s);
    if (s.length() <= 11) {
      if (s == "1") {
        lcd.print(" sec");
      } else {
        lcd.print(" secs");
      }
    }
    delay(500);
    return;
  }
  if (!is_people_nearby()) {
    lcd.noDisplay();
    Serial.println("NO people Nearby");
  } else {
    lcd.display();
    lcd.setRGB(255, 255, 255);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Password");
    lcd.setCursor(0, 1);
    RTC.getTime(rtime);
    String expected_result = String(totp.getCode(rtime.getUnixTime()));
    Serial.print("totp password is:");
    Serial.println(expected_result);
    String x = keypad_get(6, lcd, keypad);
    if (x == expected_result) {
      lcd.setRGB(0, 128, 0);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Correct Password!");
      tries = 0;
      delay(500);
    } else {
      tries++;
      if (tries >= 5) {
        estimated.setUnixTime(rtime.getUnixTime() + time_t(pow(2, expon)));
        expon++;
        tries = 0;
      } else {
        lcd.setRGB(255, 0, 0);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Wrong Password!");
        delay(500);
      }
    }
  }
  delay(750);
}