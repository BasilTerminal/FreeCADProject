#include <stdlib.h>
#include <string.h>

const int8_t Rele = 2;
const int8_t Buzer = 3;
const int8_t BuzerLED = 4;
const int8_t RFin = 8;

const char pult1cl[] = { 0x2E, 0x62, 0xF9, 0x49, 0xCB, 0xF9, 0x4B, 0 };
const char pult1op[] = { 0x2E, 0x62, 0xF9, 0x49, 0xCB, 0xF9, 0x79, 0 };
const char pult2cl[] = { 0x22, 0x2E, 0xFB, 0xFB, 0x49, 0xF9, 0x4B, 0 };
const char pult2op[] = { 0x22, 0x2E, 0xFB, 0xFB, 0x49, 0xF9, 0x79, 0 };

#define SIZEBUF 16             //розмір вхідний буфер
char inbuf[SIZEBUF] = "";      //вхідний буфер
unsigned int NumRepeat = 0;    // лічильник повторів
unsigned long prevMillis = 0;  // час останньої зміни стану
const long interval = 200;    // інтервал між настискання і відпусканням
unsigned long currMillis = 0;



static void (*STATE)();  // автомат станів

#include <SoftwareSerial.h>
SoftwareSerial mySerial(8, 11);  // RX, TX


void setup() {
  Serial.begin(115200);
  mySerial.begin(2400);

  pinMode(Rele, OUTPUT);
  pinMode(Buzer, OUTPUT);
  pinMode(BuzerLED, OUTPUT);
  pinMode(RFin, INPUT);

  sound1();
  sound2();

  STATE = &state01;

  mySerial.setTimeout(5);
}

void loop() {
  (*STATE)();
  //delay(100);
}

// state01. Очікуємо натискання кнопки пульта
// Розпізнаємо кнопку і реагуємо
// Після подачі живлення ми тут
void state01() {
  char* p;

  if (mySerial.available() > SIZEBUF) {
    for (byte i = 0; i < SIZEBUF; i++) {
      inbuf[i] = mySerial.read();
    }

    p = strstr(inbuf, pult1op);
    if (p) {
      NumRepeat++;
      prevMillis = millis();
      STATE = &state02;
      return;
    }
    p = strstr(inbuf, pult2op);
    if (p) {
      NumRepeat++;
      prevMillis = millis();
      STATE = &state02;
      return;
    }

    p = strstr(inbuf, pult1cl);
    if (p) {
      STATE = &state03;
      return;
    }
    p = strstr(inbuf, pult2cl);
    if (p) {
      STATE = &state03;
      return;
    }
  }



  if (millis() - prevMillis >= interval) {
    prevMillis == 0;
    NumRepeat = 0;
  }

  STATE = &state01;
}



// state02.
// очікуємо натискання на К1 протягом 5 сек
//open
void state02() {

  sound1();
  Serial.print(NumRepeat);
  Serial.println(" Pult - open");
  if (prevMillis == 0) prevMillis = millis();  // перший раз написнули
  STATE = &state01;
}

//close
void state03() {
  sound2();
  Serial.println("Pult - close");
  STATE = &state01;
}

void sound1() {
  digitalWrite(Buzer, HIGH);
  delay(20);
  digitalWrite(Buzer, LOW);
}

void sound2() {
  digitalWrite(BuzerLED, HIGH);
  delay(20);
  digitalWrite(BuzerLED, LOW);
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
