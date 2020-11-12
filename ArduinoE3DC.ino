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
    mainTaskMbRead();
    mbCalcInt32(&solarPowerReg1, &solarPowerReg2, &solarPower);
    mbCalcInt32(&gridPowerReg1, &gridPowerReg2, &gridPower);
    mbCalcInt32(&batPowerReg1, &batPowerReg2, &batPower);
    mbCalcInt32(&homePowerReg1, &homePowerReg2, &homePower);
    mbCalcInt16(&batSocReg, &batSoc);
    mbCalcAutarkieEigenv(&autarkieReg, &autarkie, &eigenverbrauch);
    #ifdef EXT_LM_USE
      mbCalcInt32(&extPowerReg1, &extPowerReg2, &extPower);
    #endif       
    Serial.println("______________________________ ");
    serialPrintClock();
    #ifdef DEBUG
      Serial.printf("Reboot counter   : %6d\n",readRebootCounter());
      Serial.printf("Loop   counter   : %6d\n",loopCounter);
    #endif       
    Serial.printf("WiFi strength    : %6d dBm\n",WiFi.RSSI());
    Serial.printf("Power Solar      : %6d W\n",solarPower);
    Serial.printf("Power Grid       : %6d W\n",gridPower);
    Serial.printf("Power Batterie   : %6d W\n",batPower);
    Serial.printf("Power Home       : %6d W\n",homePower);
    #ifdef EXT_LM_USE
      Serial.printf("Power Ext-LM     : %6d W\n",extPower);
    #endif       
    Serial.printf("Batterie SOC     : %6d %%\n",batSoc);
    Serial.printf("Autarkie         : %6d %%\n",autarkie);
    Serial.printf("Eigenverbrauch   : %6d %%\n",eigenverbrauch);
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
      #ifdef EXT_LM_USE
        sendHM(NO_PRINT, ISE_S10_EXT_LM, "EXT_LM", "%i", extPower);
      #endif       
      sendHM(NO_PRINT, ISE_S10_BATSOC, "S10_SOC", "%i", batSoc);
      sendHM(NO_PRINT, ISE_S10_AUTARKIE, "S10_Autarkie", "%i", autarkie);
      sendHM(NO_PRINT, ISE_S10_EIGEN, "S10_Eigenverbrauch", "%i", eigenverbrauch);
    }
  #endif

  mb.task();
  delay(100);
}
