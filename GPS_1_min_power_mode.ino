/* ---- Declaration of librairies ---- */
#include <TinyGPS++.h>
#include "boards.h"

/* ---- Declaration of variables ---- */
/*
    In this sheet, we use one variable :
   - message_gps, the string will receive the GPS position of the ESP32.
*/
String message_gps = "";

/* ---- Procedure for sending UBX message ---- */
void sendUBX(uint8_t *MSG, uint8_t len) {
  for (int i = 0; i < len; i++) {
    Serial1.write(MSG[i]);
  }
}

/* ---- Procedure for configurating the ESP32 ---- */
void configurar_tramas() {

  /* --- Activate and configurate the Power Save Mode --- */
  /* STEP 1 : Activate */
  uint8_t setRXM[] = {0xB5, 0x62, 0x06, 0x11, 0x02, 0x00, 0x08, 0x01, 0x22, 0x92};
  sendUBX(setRXM, sizeof(setRXM) / sizeof(uint8_t));
  /*
    {0xB5, 0x62, //SYNC CHAR 1, SYNC CHAR 2
    0x06, 0x11, //ID, CLASS
    0x02, 0x00, //LENGTH
    0x08, //RESERVED 1
    0x01, //LP MODE (POWER SAVE MODE)
    0x22, 0x92} //CK_A, CK_B
  */
  
  /* STEP2 : Configurate */
  uint8_t setPM2[] = {0xB5, 0x62, 0x06, 0x3B, 0x2C, 0x00, 0x01, 0x06, 0x00, 0x00, 0x0E, 0x81, 0x40, 0x01, 0x70, 0x17, 0x00, 0x00, 0x30, 0x75, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x4F, 0xC1, 0x03, 0x00, 0x87, 0x02, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x64, 0x40, 0x01, 0x00, 0xDE, 0x4F};
  /*
    ON/OFF Mode of operation
    updatetePeriod : 60000ms
    searchPeriod : 30s
    minAcqTime : 0s (thus the acquisition timeout is determined by the card.) 
    onTime : 1s
    maxStartupStateDur : 0s
    waitTimeFix : no (the gps antenna will not be inactive as long as the card can't get a time fix.)
    doNotEnterOff : no
    updateRTC : no
    updateEPH : no
    EXTINT : 0
    exintWake : no
    extintBackup : no
    gridOffset : 0s
    peakCurrent : yes
  */
  sendUBX(setPM2, sizeof(setPM2) / sizeof(uint8_t));

  /* --- Desactivate trames --- */
  uint8_t setGSA[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x32};//trame GSA
  sendUBX(setGSA, sizeof(setGSA) / sizeof(uint8_t));
  uint8_t setGLL[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x2B};//trame GLL
  sendUBX(setGLL, sizeof(setGLL) / sizeof(uint8_t));
  uint8_t setGSV[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x39};//trame GSV
  sendUBX(setGSV, sizeof(setGSV) / sizeof(uint8_t));
  uint8_t setRMC[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x04, 0x40};//trame RMC
  sendUBX(setRMC, sizeof(setRMC) / sizeof(uint8_t));
  uint8_t setVTG[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x46};//trame VTG
  sendUBX(setVTG, sizeof(setRMC) / sizeof(uint8_t));
  uint8_t setINF[] = {0xB5, 0x62, 0x06, 0x02, 0x0A, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0xF0};//trame TXT
  sendUBX(setINF, sizeof(setINF) / sizeof(uint8_t));

  /* Save or Clear the current configuration */
  uint8_t setCFG[] = {0xB5, 0x62, 0x06, 0x09, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x1B, 0xA9};//save configuration
  //uint8_t setCFG[] = {0xB5, 0x62, 0x06, 0x09, 0x0D, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0x19, 0x98};//clear configuration
  sendUBX(setCFG, sizeof(setCFG) / sizeof(uint8_t));
}

/* ---- Procedure for capturing the GPS position of the ESP32 ---- */
void capturar_datos() {
  message_gps = message_gps + (char)Serial1.read();
}

/* ---- Procedure for printing the GPS position of the ESP32 ---- */
void mostrar_trama(String str) {
  Serial.print(str);
  message_gps = "";
}

/* ---- Setup part ---- */
void setup() {
  initBoard();
  //When the power is turned on, a delay is required.
  delay(1500);
  configurar_tramas();
}

/* ---- Loop part ---- */
void loop() {
  while (Serial1.available() > 0)
    capturar_datos();
  mostrar_trama(message_gps);
}
