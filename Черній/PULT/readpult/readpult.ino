#include <stdlib.h>

const int8_t Rele = 2;
const int8_t Buzer = 3;
const int8_t BuzerLed = 4;
const int8_t RFin = 8;

#define SIZEBUF 16
char inbuf[SIZEBUF] = "";

#include <SoftwareSerial.h>
SoftwareSerial mySerial(8, 11);  // RX, TX

void setup() {
  Serial.begin(115200);
  mySerial.begin(2400);

  pinMode(Rele, OUTPUT);
  pinMode(Buzer, OUTPUT);
  pinMode(BuzerLed, OUTPUT);
  pinMode(RFin, INPUT);

  digitalWrite(BuzerLed, HIGH);
  delay(50);
  digitalWrite(BuzerLed, LOW);


  mySerial.setTimeout(5);
}

void loop() {

  if (mySerial.available() > SIZEBUF) {

    for (byte i = 0; i < SIZEBUF; i++) {
      Serial.print(F("0x"));
      Serial.print(mySerial.read(), HEX);
      Serial.print(F(","));
    }
    Serial.println();
  }

  //delay(100);
}
