#ifdef ESP8266
 #include <ESP8266WiFi.h>
 #include <ESP8266HTTPClient.h>
#else
 #include <WiFi.h>
 #include <HTTPClient.h>
#endif
#include <time.h>
#include <TimeLib.h>

#include "parameter.h"
#include "define.h"
#include "spiffs.h"
#include "init.h"
#include "ntp.h"
#include "modbus.h"
#ifdef OAT_USE
  #include "ota.h"
#endif
#ifdef HM_USE
  #include "homematic.h"
#endif
void setup() {
  serialBegin();
  serialPrintSW();
  #ifdef ESP8266
    SPIFFS_begin();
  #else
    initPreferences();
  #endif
  rebootCounter();
  startWifi();
  #ifdef OAT_USE
    ArduinoSetOTA();
    ArduinoOTA.begin();
  #endif
  initNTP();
  initModbus(IP_E3DC);
  firstReadMagicByte(mbIP_E3DC);
  #ifdef HM_USE
    sendHM(PRINT, ISE_SEND_INFO, "Info", "Start:_%s", timeStamp);  //Erst aktivieren wenn IP der Homematic eingtragen ist und ISE-IDs angepasst wurden
  #endif
  delay(1000);
}

void loop() {
  #ifdef OAT_USE
    ArduinoOTA.handle();
  #endif

  if ((millis() - lastMbMillis > IntervalModbus * 1000) || lastMbMillis == 0) {
    lastMbMillis = millis();
    loopCounter++;
    mbReadInt32(mbIP_E3DC, REG_GRID, &free1, &free2, &gridPower);
    mbReadInt32(mbIP_E3DC, REG_BAT, &free1, &free2, &batPower);
    mbReadInt32(mbIP_E3DC, REG_SOLAR, &free1, &free2, &solarPower);
    mbReadInt32(mbIP_E3DC, REG_CON, &free1, &free2, &homePower);
    mbReadInt16(mbIP_E3DC, REG_BATSOC, &batSoc);
    mbReadAutarkieEigenv(mbIP_E3DC, REG_AUTARKIE, &autarkie, &eigenverbrauch);
    Serial.println("______________________________ ");
    serialPrintClock();
    Serial.print("Reboot counter   : ");Serial.printf("%6d\n",readRebootCounter());
    Serial.print("Loop   counter   : ");Serial.printf("%6d\n",loopCounter);
    Serial.print("WiFi strength    : ");Serial.printf("%6d dBm\n",WiFi.RSSI());
    Serial.print("Power Solar      : ");Serial.printf("%6d W\n",solarPower);
    Serial.print("Power Grid       : ");Serial.printf("%6d W\n",gridPower);
    Serial.print("Power Batterie   : ");Serial.printf("%6d W\n",batPower);
    Serial.print("Power Home       : ");Serial.printf("%6d W\n",homePower);
    Serial.print("Batterie SOC     : ");Serial.printf("%6d %%\n",batSoc);
    Serial.print("Autarkie         : ");Serial.printf("%6d %%\n",autarkie);
    Serial.print("Eigenverbrauch   : ");Serial.printf("%6d %%\n",eigenverbrauch);
  }

  #ifdef HM_USE
    if ((millis() - lastHmMillis > INTERVALL_HM * 1000) || lastHmMillis == 0) {
      lastHmMillis = millis();
      sendHM(NO_PRINT, ISE_SEND_AKTIV, "Aktiv", "true");
      sendHM(NO_PRINT, ISE_SEND_WIFI, "Wifi", "%i", WiFi.RSSI());
      sendHM(NO_PRINT, ISE_S10_GRID, "S10_Grid", "%i", gridPower);
      sendHM(NO_PRINT, ISE_S10_BAT, "S10_Bat", "%i", batPower);
      sendHM(NO_PRINT, ISE_S10_SOLAR, "S10_Solar", "%i", solarPower);
      sendHM(NO_PRINT, ISE_S10_CON, "S10_Home", "%i", homePower);
      sendHM(NO_PRINT, ISE_S10_BATSOC, "S10_SOC", "%i", batSoc);
      sendHM(NO_PRINT, ISE_S10_AUTARKIE, "S10_Autarkie", "%i", autarkie);
      sendHM(NO_PRINT, ISE_S10_EIGEN, "S10_Eigenverbrauch", "%i", eigenverbrauch);
    }
  #endif

  mb.task();
  delay(100);
}
