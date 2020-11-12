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
IPAddress mbIP_E3DC;
int mbDelay = 45;

uint16_t magicbyte = 0;
uint16_t solarPowerReg1        = 0;
uint16_t solarPowerReg2        = 0;
uint16_t gridPowerReg1         = 0;
uint16_t gridPowerReg2         = 0;
uint16_t batPowerReg1          = 0;
uint16_t batPowerReg2          = 0;
uint16_t homePowerReg1         = 0;
uint16_t homePowerReg2         = 0;
uint16_t extPowerReg1          = 0;
uint16_t extPowerReg2          = 0;
uint16_t batSocReg             = 0;
uint16_t autarkieReg           = 0;
int batPower       = 0;
int gridPower      = 0;
int solarPower     = 0;
int homePower      = 0;
int extPower       = 0;
int batSoc         = 0;
int autarkie       = 0;
int eigenverbrauch = 0;
int mbDebugCounter = 0;

bool cbConn(IPAddress ip) {
  Serial.println(ip);
  return true; // Return 'true' to allow connection or 'false' to drop connection
}

void initModbus(const char * ipAdress){
  mbIP_E3DC.fromString(ipAdress);
  Serial.print("Modbus Init      :  wait...");
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
    modbusTimeout = true;
}
void firstReadMagicByte(IPAddress ip){
  if(modbusTimeout == true)return;
  char out[24];
    while(magicbyte != 0xE3DC ) {  
    mb.readHreg(ip, REG_MAGIC -1, &magicbyte);
    delay(40);
    mb.task();
    delay(40);
  }
  sprintf (out, "0x%X",magicbyte);  
  Serial.print("Magic byte       :  ");
  Serial.println(out);
}
void mainTaskMbRead(){
    if(INTERVALL_MODBUS >= 10 ) {
      mb.connect(mbIP_E3DC);
      delay(mbDelay);
    }
    mb.readHreg(mbIP_E3DC, REG_SOLAR +REG_OFFSET, &solarPowerReg1);delay(mbDelay);
    mb.task();
    mb.readHreg(mbIP_E3DC, REG_SOLAR +REG_OFFSET +1, &solarPowerReg2);delay(mbDelay);
    mb.task();
    mb.readHreg(mbIP_E3DC, REG_GRID +REG_OFFSET, &gridPowerReg1);delay(mbDelay);
    mb.task();
    mb.readHreg(mbIP_E3DC, REG_GRID +REG_OFFSET +1, &gridPowerReg2);delay(mbDelay);
    mb.task();
    mb.readHreg(mbIP_E3DC, REG_BAT +REG_OFFSET, &batPowerReg1);delay(mbDelay);
    mb.task();
    mb.readHreg(mbIP_E3DC, REG_BAT +REG_OFFSET +1, &batPowerReg2);delay(mbDelay);
    mb.task();
    mb.readHreg(mbIP_E3DC, REG_CON +REG_OFFSET, &homePowerReg1);delay(mbDelay);
    mb.task();
    #ifdef EXT_LM_USE
      mb.readHreg(mbIP_E3DC, REG_EXT_LM +REG_OFFSET, &homePowerReg1);delay(mbDelay);
      mb.task();
    #endif       
    mb.readHreg(mbIP_E3DC, REG_CON +REG_OFFSET +1, &homePowerReg2);delay(mbDelay);
    mb.task();
    mb.readHreg(mbIP_E3DC, REG_BATSOC +REG_OFFSET, &batSocReg);delay(mbDelay);
    mb.task();
    mb.readHreg(mbIP_E3DC, REG_AUTARKIE +REG_OFFSET, &autarkieReg);delay(mbDelay);
    mb.task();
    mbDebugCounter++;
    if(mbDebugCounter > 65530) mbDebugCounter = 0;
    if(INTERVALL_MODBUS >= 10 ) {
      mb.disconnect(mbIP_E3DC);
    }
}
void mbCalcInt16(uint16_t *reg1, int *value){
    *value = *reg1; 
    *reg1 = 0;
}
void mbCalcInt32(uint16_t *reg1, uint16_t *reg2, int *value){
    int positiv, negativ;
    if(*reg2 < 32768){
      positiv = *reg2 * 65536 + *reg1;
      negativ = 0;
    } else {
      negativ = 4294967296 - *reg2 * 65536 - *reg1; 
      positiv = 0;
    }
    *value = positiv - negativ;
    *reg1 = 0;
    *reg2 = 0;
}
void mbCalcAutarkieEigenv(uint16_t *reg, int *Autarkie, int *Eigenverbrauch){
    *Autarkie = *reg / 256;
    *Eigenverbrauch = *reg % 256;
    *reg = 0;
}
#endif // __MODBUS_H_
