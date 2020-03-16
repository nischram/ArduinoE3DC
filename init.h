#ifndef __INIT_H_
#define __INIT_H_

char OUT[12];
const char* ssid     = STASSID;
const char* password = STAPSK;
bool wifiTimeout = false;
long lastHmMillis = 0;
long lastMbMillis = 0;
int rebootCount = 0;
int loopCounter = 0;
int IntervalModbus = 1;

//------------------------------
// Serial
void serialBegin(){
  Serial.begin(SERIALSPEED);
  delay(1200);
  Serial.println("");
  Serial.println("______________________________ ");
  Serial.println("");
}
void serialPrintSW(){
  Serial.print("Software Version : ");Serial.printf(" V%4.2f\n",SW_VERSION);
  Serial.print("Software Datum   : ");Serial.printf(" %s\n",SW_DATE);
}
//------------------------------
// WiFi network
void startWifi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("WiFi init        :  wait...");
  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED && timeout < WIFI_TIMEOUT * 2) {
    Serial.print(".");
    delay(500);
    timeout++;
  }
  if (timeout < WIFI_TIMEOUT * 2){
    Serial.println("");
    Serial.println("WiFi status      :  connected");
    Serial.print("ESP-IP address   :  ");
    Serial.println(WiFi.localIP());
    Serial.print("WiFi strength    :  ");Serial.printf("%6d dBm\n",WiFi.RSSI());
    wifiTimeout = false;
  }
  else {
    Serial.println("");
    Serial.println("WiFi status      :  failed!");
    wifiTimeout = true;
  }
}
//------------------------------
// init Preferences
#ifdef ESP32
  #include <Preferences.h>

  Preferences paraWifi, paraESP;

  void initPreferences(){
    paraESP.begin("ESP");
  }
#endif

//------------------------------
// Zähler für Reboot
void rebootCounter(){
  #ifdef ESP8266
    //writeReboot(0);                //einmalig aktivieren zum zurücksetzen
    rebootCount = readReboot();
    if (rebootCount >= 65634) rebootCount = 0;
    rebootCount++;
    Serial.print("Reboot counter   :  ");Serial.println(rebootCount);
    writeReboot(rebootCount);
  #else
    //paraESP.putUInt("BOOT", 0);   //einmalig aktivieren zum zurücksetzen
    rebootCount = paraESP.getUInt("BOOT");
    if (rebootCount >= 65634) rebootCount = 0;
    rebootCount++;
    Serial.print("Reboot counter   :  ");Serial.println(rebootCount);
    paraESP.putUInt("BOOT", rebootCount);
  #endif
}
int readRebootCounter(){
  #ifdef ESP8266
    return readReboot();
  #else
    return paraESP.getUInt("BOOT");
  #endif
}


#endif // __INIT_H_
