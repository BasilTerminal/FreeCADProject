#include <stdlib.h>

const int8_t Rele = 2;
const int8_t Buzer = 3;
const int8_t BuzerLed = 4;
const int8_t RFin = 8;

//const int16_t VoltageLevelWhenBatteryPluged = 900;

static void (*STATE)();

#include <SoftwareSerial.h>

SoftwareSerial mySerial(8, 11);  // RX, TX


void setup() {
  Serial.begin(115200);


  mySerial.begin(1200);
  mySerial.println("Hello, world?");


  pinMode(Rele, OUTPUT);
  pinMode(Buzer, OUTPUT);
  pinMode(BuzerLed, OUTPUT);
  pinMode(RFin, INPUT);

  digitalWrite(Rele, HIGH);
  delay(500);
  digitalWrite(Rele, LOW);

  digitalWrite(Buzer, HIGH);
  delay(200);
  digitalWrite(Buzer, LOW);

  digitalWrite(BuzerLed, HIGH);
  delay(200);
  digitalWrite(BuzerLed, LOW);


  STATE = &state01;
}

void loop() {

  //if (mySerial.available()) {

    //char a=mySerial.read();


    Serial.println(mySerial.available());//(a, HEX);
    mySerial.println(0x55);//(a, HEX);
  //}

  (*STATE)();
  delay(100);
}

// state01. Очікуємо натискання кнопки пульта
// Розпізнаємо кнопку і реагуємо
// Після подачі живлення ми тут
void state01() {

  //if (currentOfCharging() > 0) {
  STATE = &state02;
}

// state02.
// очікуємо натискання на К1 протягом 5 сек
void state02() {

  STATE = &state01;
}





void updateIndicationByInnerState() {
  Serial.print(F("cur: "));
  //Serial.print(currentOfCharging());
  Serial.print(F("\tconn: "));
  //Serial.print(connectedBattery());
  Serial.print(F("\tstate: "));
  //Serial.print(STATE == &state01 ? 1 : 2);
  Serial.println("");
}
