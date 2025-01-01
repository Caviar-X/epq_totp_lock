
// Include the RTC library
#include "RTC.h"

//Include the NTP library
#include <NTPClient.h>

#if defined(ARDUINO_PORTENTA_C33)
#include <WiFiC3.h>
#elif defined(ARDUINO_UNOWIFIR4)
#include <WiFiS3.h>
#endif

#include <WiFiUdp.h>

int get_current_time(RTCTime &rtime,const char* ssid,const char* pass) {
  int wifiStatus = WL_IDLE_STATUS;
  WiFiUDP Udp;
  NTPClient timeClient(Udp);
  if (WiFi.status() == WL_NO_MODULE) {
    return -1;
  }
  int tries = 0;
  while (wifiStatus != WL_CONNECTED) {
    if (tries >= 5) {
      break;
    }
    wifiStatus = WiFi.begin(ssid, pass);
    delay(10000);
    tries++;
  }
  if(tries >= 5) {
    return -2;
  }
  timeClient.begin();
  timeClient.update();
  rtime = RTCTime(timeClient.getEpochTime());
  return 0;
}

bool is_people_nearby() {
    return digitalRead(12) == HIGH;
}