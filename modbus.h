#ifndef __MODBUS_H_
#define __MODBUS_H_

/*
  Modbus-Arduino Example - Master (Modbus IP ESP8266/ESP32)
  Read Holding Register from Slave device

  (c)2018 Alexander Emelianov (a.m.emelianov@gmail.com)
  https://github.com/emelianov/modbus-esp8266
*/

#include <ModbusIP_ESP8266.h>

bool modbusTimeout = false;

ModbusIP mb;  //ModbusIP
uint16_t magicbyte = 0;
IPAddress mbIP_E3DC;

int free1          = 0;
int free2          = 0;
int batPower       = 0;
int gridPower      = 0;
int solarPower     = 0;
int homePower      = 0;
int batSoc         = 0;
int autarkie       = 0;
int eigenverbrauch = 0;

bool cbConn(IPAddress ip) {
  Serial.println(ip);
  return true; // Return 'true' to allow connection or 'false' to drop connection
}

void initModbus(const char * ipAdress){
  mbIP_E3DC.fromString(ipAdress);
  Serial.print("Modbus Init      :  wait...");
  if(INTERVALL_MODBUS > INTERVALL_HM) IntervalModbus = INTERVALL_HM;
  else IntervalModbus = INTERVALL_MODBUS;
  int timeout = 0;
  mb.master();
  delay(100);
  while (!mb.isConnected(mbIP_E3DC) && timeout < MODBUS_TIMEOUT * 10) {
    mb.connect(mbIP_E3DC);
    Serial.print(".");
    delay(100);
    timeout++;
  }
  if (timeout < MODBUS_TIMEOUT * 10){
    Serial.println("");
    Serial.println("Modbus status    :  connected");
      Serial.print("Modbus port      :  ");
    Serial.println(MODBUSIP_PORT);
    modbusTimeout = false;
  }
  else {
    Serial.println("");
    Serial.println("Modbus status    :  failed!");
    modbusTimeout = true;
  }
}
void closeModbus(IPAddress ip){
    Serial.print("Modbus close     :  ");
    if (mb.disconnect(ip)) Serial.println("ready");
    else Serial.println("failed!");
}
void firstReadMagicByte(IPAddress ip){
  char out[24];
    while(magicbyte != 0xE3DC ) {
    mb.readHreg(ip, REG_MAGIC -1, &magicbyte);
    delay(40);
    mb.task();
    delay(40);
  }
  sprintf (out, "0x%X",magicbyte);
  Serial.print("Magic byte       :  ");Serial.println(out);
}
void mbReadInt16(IPAddress ip, const int REG, int *value){
    uint16_t reg1        = 0;
    mb.readHreg(ip, REG +REG_OFFSET, &reg1);
    delay(40);
    mb.task();
    delay(40);
    *value = reg1;
}
void mbReadInt32(IPAddress ip, const int REG, int *positiv, int *negativ, int *value){
    uint16_t reg1        = 0;
    uint16_t reg2        = 0;
    mb.readHreg(ip, REG +REG_OFFSET, &reg1);
    delay(40);
    mb.task();
    delay(40);
    mb.readHreg(ip, REG +REG_OFFSET +1, &reg2);
    delay(40);
    mb.task();
    delay(40);
    if(reg2 < 32768){
      *positiv = reg2 * 65536 + reg1;
      *negativ = 0;
    } else {
      *negativ = 4294967296 - reg2 * 65536 - reg1;
      *positiv = 0;
    }
    *value = *positiv - *negativ;
}
void mbReadAutarkieEigenv(IPAddress ip, const int REG, int *Autarkie, int *Eigenverbrauch){
    uint16_t reg        = 0;
    mb.readHreg(ip, REG +REG_OFFSET, &reg);
    delay(40);
    mb.task();
    delay(40);
    *Autarkie = reg / 256;
    *Eigenverbrauch = reg % 256;
}
#endif // __MODBUS_H_
