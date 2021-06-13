# ArduinoE3DC

## Wichtige Information!!!
Dieser Repository wird nicht weitergeführt.  
Bitte neue Repository beachten: [E3DC-Modbus-Display](https://github.com/nischram/EMD_1)  

***
***

Verbindung eines S10 Hauskraftwerk von E3DC mit einem Arduino (ESP) per Modbus  
Stand: V1.00 12.11.2020

Diese Software ermöglicht die Datenabfrage eines S10 Hauskraftwerk von E3DC mit z.B. einem ESP8266 oder ESP32 per Modbus.

**Diese Anleitung und Software ist noch im Aufbau!**  
_Angaben über Lizenzen muss ich noch prüfen oder erstellen._

### Board
Ich habe die Software einmal mit einem **ESP8266 NodeMcu V3** und mit einem **ESP32 NodeMCU** getestet. Wegen der unterschiedlichen W-LAN-Library muss in der "_parameter.h_" das jeweilige Board ausgewählt werden indem das andere Auskommentiert wird.

### Einstellungen
Alle Einstellungen werden in der Datei "_parameter.h_" gemacht.
Wichtig sind die W-Lan Zugangsdaten und die IP-Adresse vom E3DC Hauskraftwerk.

### Modbus E3DC
Die Firma E3DC hat im Kundenportal/Download für uns User die Anleitung für den Modbus. Es muss die Modbusfunktion am Display aktiviert werden. Dies geht aus der Anleitung hervor auch die entsprechenden Register sind in der Anleitung zu finden. Ich habe nicht alle Register umgesetzt, aber das Prinzip kann ohne viel Aufwand erweitert werden.

### Modbus ESP
Die Library für Modbus habe ich hier gefunden.  
Link: [emelianov / modbus-esp8266](https://github.com/emelianov/modbus-esp8266)
Diese Library muss installiert werden.

### Anbindung Homematic
Über einen "Curl-Befehl" kann die Software die Daten zur Homematic senden. Zuvor musst du die Parameter für die IP-Adresse oder die verschiedenen ISE-IDs anpassen. In der Homematic müssen die entsprechenden Systemvariablen angelegt werden, die ISE-ID der Variablen kann man z.B. mit "HQ-WebUI" auslesen. Da die ISE-ID zum senden genutzt wird, muss der Name der Variable nicht eindeutig sein, den kannst du frei wählen. Für die Anbindung wird das "XML-API CCU Addon" benötigt.  
Link: [jens-maus / XML-API](https://github.com/jens-maus/XML-API)   

### Alternative Nutzung
Hier sind in der Arduino- und ESP-Welt wohl fast keine Grenzen gesetzt. Du musst den Code nur entsprechend anpassen. z.B.:
- OLED-Display
- LED-Matrix  

Um Andere Anwendungen zu ergänzen müssen die Variablen genommen werden die vom Modbus-Request gefüllt werden. Hier das Beispiel zur Batterie-Leistung:  
`mbCalcInt32(&batPowerReg1, &batPowerReg2, &batPower);`  
Die Variable "_batPower_" ist (int) und enthält sowohl positiv die Batterieladung sowie negativ die Entladung.  
Werte die als "Int16" abgefragt werden, können mit dieser Zeile direkt eine Variable versorgen, hier der Batterie-Füllstand (SOC):  
`mbCalcInt16(&batSocReg, &batSoc);`

### OTA - Over the Air
Dank AZ-Delivery habe ich auch die **Over the Air** update Funktion eingebaut.  
Link: [AZ-Delivery / OTA](https://www.az-delivery.de/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/ota-over-the-air-esp-programmieren-uber-wlan-entwurf)

### Wünsche / Anregungen
Bitte erstelle für deine Wünsche und Anregungen einen Issue.
Auch Fehler bitte als Issue eintragen.  
Gerne auf Deutsch, auch wenn Programmierer viel in Englisch schreiben.

### License
The code in this repo is licensed under the BSD New License. See LICENSE.txt for more info.  

### Changelog
V1.00 12.11.2020 Modbus überarbeitet + Issue #1
- Komplette Anpassung der modbus.h für [Issue #1](https://github.com/nischram/ArduinoE3DC/issues/1)
- Schalter für Sommer- Winetzeit eingebaut (ntp-Zeit war falsch)

V0.02 22.03.2020 Lizenz ergänzt + Issue #1
- LICENSE.txt
- Anpassung für [Issue #1](https://github.com/nischram/ArduinoE3DC/issues/1)

V0.01 15.03.2020 Repository neu angelegt
- Software neu erstellt und hochgeladen

