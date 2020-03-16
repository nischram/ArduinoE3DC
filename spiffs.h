#ifndef __SPIFFS_H_
#define __SPIFFS_H_

#ifdef ESP8266
  #include <FS.h>           // Include the SPIFFS library
  
  void SPIFFS_begin(){
    if(!SPIFFS.begin()){
      Serial.println("An Error has occurred while mounting SPIFFS");
    }
  }
  
  void writeSPIFFS(float value, char const *path){
      File f = SPIFFS.open(path, "w");
    if (!f) {
        Serial.println("file open failed");
    }
    //Serial.print("set ");Serial.print(value);Serial.print(" to ");Serial.println(path);
    f.println(value);
    f.close();
  }
  float readSPIFFS(char const *path){
    float ret;
    File f = SPIFFS.open(path, "r");
    if (!f) {
        Serial.println("file open failed");
        ret = -200;
    }
    String str = f.readStringUntil('\n');
    f.close();
    ret = str.toFloat();
    //Serial.print("get ");Serial.print(ret);Serial.print(" from ");Serial.println(path);
    return ret;
  }
  void writeReboot(float value){
      writeSPIFFS(value, "/reboot.txt");
  }
  int readReboot(){
      return readSPIFFS("/reboot.txt");
  }
#endif //ESP8266

#endif // __SPIFFS_H_
