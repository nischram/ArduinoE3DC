#ifndef __PARAMETER_H_
#define __PARAMETER_H_

//#######################################
// Software Parameter
#define SW_VERSION                  1.00
#define SW_DATE                    "12.11.2020"

//#define DEBUG                                             // Aktiv DEBUG

//#######################################
// ESP Parameter
#define ESP8266
/* NodeMcu V3 Module ESP8266
 * Board: "NodeMCU 1.0 (ESP-12E Module)"
 * CPU Frequency: "80 Mhz"
 * Flash Size: "4M (3M SPIFFS)" */
 //or
/* ESP-01 mit DHT PiggyBack
 * Board: "Generic ESP8266 Modul"
 * CPU Frequency: "80 Mhz"
 * Flash Size: "512KB 8FS_64KB OTA:214KB)"
 * Flash Mode: QID (fast)
 * Reset Mode: "no drt (aka ck)" */
//#define ESP32
/* Einstellungen Arduiono IDE
 * Board:         "ESP32 Dev Modul"
 * CPU Frequency: "80 Mhz"
 * Flash Mode:    "DIO"
 * Flash Size:    "4MB (32Mb)"
 */
#define SERIALSPEED                 115200

//#######################################
// W-LAN Parameter
#define STASSID                    "W-LAN Name"             // W-LAN SSID
#define STAPSK                     "W-LAN Passwort"         // W-LAN Schlüssel
#define WIFI_TIMEOUT                6                       // Sekunden

//#######################################
// Modbus Parameter
#define IP_E3DC                    "192.168.123.123"        // IP-Adresse vom E3DC
#define INTERVALL_MODBUS            5                       // Abfrageintervall in Sekunden
#define MODBUS_TIMEOUT              6                       // Sekunden
//#define EXT_LM_USE                                          // Aktiv = externe Leistungsmesser sind verbaut

#define REG_OFFSET                  -1
#define REG_MAGIC                   40001                   // E3DC Modbus Register
#define REG_SOLAR                   40068                   // E3DC Modbus Register
#define REG_BAT                     40070                   // E3DC Modbus Register
#define REG_CON                     40072                   // E3DC Modbus Register
#define REG_GRID                    40074                   // E3DC Modbus Register
#define REG_AUTARKIE                40082                   // E3DC Modbus Register
#define REG_BATSOC                  40083                   // E3DC Modbus Register
#define REG_EXT_LM                  40076                   // E3DC Modbus Register

//#######################################
// Homematic Parameter
//#define HM_USE                                            // Aktivieren wenn senden der Daten zur HM genutzt werden soll
#define IP_HM                      "192.168.123.456"        // IP-Adresse von der Homematic
#define INTERVALL_HM                30                      // Abfrageintervall, keinste Empfehlung 15 Sekunden

#define ISE_SEND_AKTIV              3082                    // ISE_ID für Status, ein Program anlegen welches nach Auslösen diser Sytemvariabl z.B. nach 3 Minuten die Variable zurücksetzt
#define ISE_SEND_INFO               2878                    // ISE_ID Startzeitpunkt
#define ISE_SEND_WIFI               3135                    // ISE_ID WiFi Empfangsstärke

#define ISE_S10_SOLAR               1769                    // ISE_ID aus der HM
#define ISE_S10_BAT                 3184                    // ISE_ID aus der HM
#define ISE_S10_CON                 1768                    // ISE_ID aus der HM
#define ISE_S10_GRID                3183                    // ISE_ID aus der HM
#define ISE_S10_AUTARKIE            3185                    // ISE_ID aus der HM
#define ISE_S10_EIGEN               3186                    // ISE_ID aus der HM
#define ISE_S10_BATSOC              1770                    // ISE_ID aus der HM
#define ISE_S10_EXT_LM              3187                    // ISE_ID aus der HM

//#######################################
// W-LAN Parameter
//#define OAT_USE                                           // Aktivieren wenn OAT genutzt werden soll, Librarie muss installiert werden
#define OTA_HOSTNAME               "OTA-ModbusE3DC"         // Hostname für OverTheAir Update

//#######################################
// NTP & Time Parameter
#define NTP_SERVER_1               "time.google.com"        // alternativ: "fritz.box"
#define NTP_SERVER_2               "pool.ntp.org"
#define TIMEZONE                    1
#define DST                         0
#define SOMMER_WINTER               WINTER                  // Sommer- Winterzeit Einstellung Sommer=SOMMER Winter=WINTER

#endif // __PARAMETER_H_
