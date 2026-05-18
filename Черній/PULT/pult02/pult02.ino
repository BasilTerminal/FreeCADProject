//#include <stdlib.h>
#include <GTimer.h>

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
const long interval = 200;     // інтервал між настискання і відпусканням
unsigned long currMillis = 0;
bool flag_wait = 0;   // флаг що чекаємо кнопку
bool flag_close = 0;  //
bool flag_open = 0;   //


//unsigned long beep_prevMs = 0;   // для біпера
//unsigned long beep_curMs = 0;    // для біпера

//unsigned long beep_timer = 0;     // для біпера
//const long beep_interval = 1000;  // інтервал біпера


static void (*STATE)();  // автомат станів

#include <SoftwareSerial.h>
SoftwareSerial mySerial(8, 11);  // RX, TX

GTimer<millis> tmrWait;              // чекаємо 5 скунд натискання кнопки
GTimer<millis> tmrFlash(500, true);  // для блимання

void setup() {
  Serial.begin(115200);
  mySerial.begin(2400);

  pinMode(Rele, OUTPUT);
  pinMode(Buzer, OUTPUT);
  pinMode(BuzerLED, OUTPUT);
  pinMode(RFin, INPUT);

  Stop();

  STATE = &state01;

  mySerial.setTimeout(5);

  // beep_timer = millis();

  tmrWait.setMode(GTMode::Timeout);
  tmrWait.setTime(5000);
}

void loop() {

  (*STATE)();
}

// =================== state01 ====================
// Очікуємо натискання кнопки пульта
// Розпізнаємо і реагуємо
// Після подачі живлення ми тут
void state01() {
  flag_close = 0;
  flag_open = 0;

  if (mySerial.available() > SIZEBUF) {
    for (byte i = 0; i < SIZEBUF; i++) {
      inbuf[i] = mySerial.read();
    }
    if (strstr(inbuf, pult1op)) flag_open = 1;
    if (strstr(inbuf, pult2op)) flag_open = 1;
    if (strstr(inbuf, pult1cl)) flag_close = 1;
    if (strstr(inbuf, pult2cl)) flag_close = 1;
  }

  if (flag_open) {
    tmrWait.start();
    NumRepeat++;
    STATE = &state02;
    return;
  }


  if (flag_close) {
    STATE = &state03;
    return;
  }


  // if (millis() - prevMillis >= interval) {
  //   prevMillis == 0;
  //   NumRepeat = 0;
  // }

  STATE = &state01;
}


//================== state02 ===========================
// очікуємо натискання на К1 протягом 5 сек
//open
void state02() {
  //блимаємо
  if (tmrFlash) digitalWrite(BuzerLED, !digitalRead(BuzerLED));

  //чекаємо настискання K1
  //.......  

  // Serial.print(NumRepeat);
  //Serial.println(" Pult - open");
  
  if (tmrWait) {  // час очікування вийшов
    Stop();
    digitalWrite(BuzerLED, LOW);
    STATE = &state01;
    return;
  }
  STATE = &state02;
}





//вимикаємо реле
void state03() {
  Stop();
  Serial.println("Pult - close");
  STATE = &state01;
}





void sound1() {
  digitalWrite(Buzer, HIGH);
  delay(40);
  digitalWrite(Buzer, LOW);
  delay(40);
}

void sound2() {
  digitalWrite(BuzerLED, HIGH);
  delay(20);
  digitalWrite(BuzerLED, LOW);
}

// можна їхати- вмикаємо реле і пікаємо
void Start() {
  digitalWrite(Rele, HIGH);
  sound1();
  sound1();
}

// вимикаємо
void Stop() {
  digitalWrite(Rele, LOW);
  sound1();
  sound1();
  sound1();
  sound1();
}
