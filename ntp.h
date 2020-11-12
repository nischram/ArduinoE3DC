#ifndef __NTP_H_
#define __NTP_H_

char timeStamp[100];
char datum[100];
char zeit[100];

long millisNTP = 0;

int timeDiff = SOMMER_WINTER;

void cTime(int TM, char * var){
  if(TM < 10)
    sprintf (var, "0%i", TM);
  else
    sprintf (var, "%i", TM);
}
void makeClock(time_t tm, char *url, char *datum, char *zeit){
  // digital clock display of the time
  char HOUR[4], MIN[4], SEK[4], DAY[4], MON[4], YEAR[6];
  char tStamp[100];
  cTime(hour(tm), HOUR);
  cTime(minute(tm), MIN);
  cTime(second(tm), SEK);
  cTime(day(tm), DAY);
  cTime(month(tm), MON);
  cTime(year(tm), YEAR); 
  sprintf (datum, "%s.%s.%s",DAY,MON,YEAR);
  sprintf (zeit, "%s:%s:%s",HOUR,MIN,SEK);
  sprintf (url, "%s:%s:%s__%s.%s.%s",HOUR,MIN,SEK,DAY,MON,YEAR);
}
void serialPrintClock(){
    makeClock(time(nullptr) + timeDiff, timeStamp, datum, zeit);
    Serial.print("Datum            :  ");Serial.println(datum);
    Serial.print("Uhrzeit          :  ");Serial.println(zeit);
}
void initNTP(){
  configTime(TIMEZONE * 3600, DST * 0, NTP_SERVER_1, NTP_SERVER_2);
  Serial.println("Time init        :  wait...");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(500);
  }
  delay(500);
  serialPrintClock();
}


#endif // __NTP_H_
