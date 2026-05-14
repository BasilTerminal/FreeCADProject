#include <stdlib.h>

const int8_t Rele = 2;
const int8_t Buzer = 3;
const int8_t BuzerLed = 4;
const int8_t RFin = 8;

//const int16_t VoltageLevelWhenBatteryPluged = 900;

static void (*STATE)();


void setup() {
  Serial.begin(115200);
  STATE = &state01;

  pinMode(Rele, OUTPUT);
  pinMode(Buzer, OUTPUT);
  pinMode(BuzerLed, OUTPUT);

  digitalWrite(Rele, HIGH);
  delay(500);
  digitalWrite(Rele, LOW);

  digitalWrite(Buzer, HIGH);
  delay(500);
  digitalWrite(Buzer, LOW);

  digitalWrite(BuzerLed, HIGH);
  delay(500);
  digitalWrite(BuzerLed, LOW);
}

void loop() {

  (*STATE)();
  delay(200);
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
