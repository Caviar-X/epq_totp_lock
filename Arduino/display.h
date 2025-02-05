#ifndef DISPLAY_H
#define DISPLAY_H

#include "Keypad.h"
#include "rgb_lcd.h"
#include <stdlib.h>

const static int ROWS = 4, COLS = 3;

static char KEYPAD[4][3] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};

// NOTE: Change the pins if physical pins are changed
static byte rowPins[ROWS] = { 9, 8, 7, 6 };
static byte colPins[COLS] = { 5, 4, 3 };


/**
 * Get the data from keypad and display them on the screen
 * default from line 2
 * -1 : failed to get value
 */
String keypad_get(size_t len, rgb_lcd lcd, Keypad keypad);

void clear_line(uint8_t line,rgb_lcd lcd);
#endif