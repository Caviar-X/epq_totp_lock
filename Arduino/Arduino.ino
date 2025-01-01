#include "util.h"
#include "RTC.h"
#include "display.h"
#include "TOTP.h"
uint8_t TOTP_KEY[] = {0x4d, 0x79, 0x4c, 0x65, 0x67, 0x6f, 0x44, 0x6f, 0x6f, 0x72};
rgb_lcd lcd;
RTCTime rtime;
Keypad keypad = Keypad(makeKeymap(KEYPAD), rowPins, colPins, ROWS, COLS);
TOTP totp = TOTP(TOTP_KEY,10);
void setup() {
  pinMode(12,INPUT);
  lcd.begin(16, 2);
  Serial.begin(9600);
  RTC.begin();
  Serial.print("Get time...");
  get_current_time(rtime,"","");
  RTC.setTime(rtime);
}
void loop() {
  if (!is_people_nearby()) {
    lcd.noDisplay();
    Serial.println("No people nearby");
  } else {
    Serial.println("People nearby");
    lcd.display();
    lcd.setRGB(255,255,255);
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
      lcd.setRGB(0,128,0);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Password Correct!");
      delay(200);
    } else {
      lcd.setRGB(255,0,0);
      lcd.clear();
      lcd.setCursor(0,0);
      delay(200);
    }
  }
  delay(250);

}