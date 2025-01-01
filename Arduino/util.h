/**
 * The utility function
 */

#ifndef UTIL_H
#define UTIL_H
#include "RTC.h"
/**
 * The purpose of this function is to sync the time using the wifi feature of
 * the board. A bit cheating I guess 
 * return value: 
 * 0 - fine 
 * 1 - wifi failed 
 * 2 - after 5 tries still unable to connect to wifi
 */
int get_current_time(RTCTime & rtime, const char *ssid, const char *pass);

bool is_people_nearby();
#endif  // UTIL_H