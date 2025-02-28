#include "util.h"
#include "RTC.h"
#include "display.h"
#include "TOTP.h"
#include <math.h>
#include "Servo.h"
uint8_t TOTP_KEY[] = {77, 89, 76, 69, 71, 79, 68, 79, 79, 81};
rgb_lcd lcd;
RTCTime rtime, estimated;
Keypad keypad = Keypad(makeKeymap(KEYPAD), rowPins, colPins, ROWS, COLS);
TOTP totp = TOTP(TOTP_KEY, 10);
Servo servo;
unsigned char tries = 0, expon = 9;
void setup() {
  pinMode(12, INPUT);
  lcd.begin(16, 2);
  Serial.begin(9600);
  servo.attach(10);
  RTC.begin();
  Serial.print("Get time...");
  get_current_time(rtime, "Caviar-X", "Yu2008..@");
  RTC.setTime(rtime);
  estimated.setUnixTime(0);
}
void loop() {
  RTC.getTime(rtime);
  long long lapse = estimated.getUnixTime() - rtime.getUnixTime();
  if (lapse > 0) {
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
  if (!is_people_nearby() && keypad.getKey() == NO_KEY) {
    lcd.setRGB(0, 0, 0);
  } else {
    lcd.display();
    lcd.setRGB(255, 255, 255);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Password");
    lcd.setCursor(0, 1);
    RTC.getTime(rtime);
    long timestamp = rtime.getUnixTime();
    String expected_result = String(totp.getCode(timestamp));
    Serial.print("totp password is:");
    Serial.println(expected_result);
    String x = keypad_get(6, lcd, keypad);
    if (x == expected_result) {
      lcd.setRGB(0, 128, 0);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Correct Password!");
      servo.write(80);
      tries = 0;
      delay(750);
      servo.write(90);
      for(int i = 10; i > 0;i --) {
        lcd.setCursor(0, 1);
        lcd.print("Open for ");
        lcd.print(String(i));
        if (i == 1) {
          lcd.print(" sec");
        } else {
          lcd.print(" secs");
        }
        delay(1000);
        clear_line(1,lcd);
      }
      delay(250);
      lcd.clear();
      servo.write(100);
      delay(1000);
      servo.write(90);
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