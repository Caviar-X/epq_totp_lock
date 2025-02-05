#include "display.h"
#include <Arduino.h>

void clear_line(uint8_t line, rgb_lcd lcd) {
  // assume length is 16
  lcd.setCursor(0, line);
  for (long _ = 0; _ < 16; _++) {
    lcd.print(" ");
  }
  lcd.setCursor(0, line);
}

String keypad_get(size_t len, rgb_lcd lcd, Keypad keypad) {
  // on line two
  clear_line(1, lcd);
  String s;
  while (s.length() < 6) {
    char c = keypad.getKey();
    if (c == 0 || c == '*') {
      continue;
    } else if (c == '#') {
      clear_line(0, lcd);
      s = "";
    } else {
      s.concat(c);
      lcd.print(String(c).c_str());
    }
  }
  return s;
}