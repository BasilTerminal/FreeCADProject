#include <SoftwareSerial.h>
SoftwareSerial mySerial(8, 10);  // RX, TX

#include <GTimer.h>
GTimer<millis> tmrWaitPressKey;      // чекаємо 5 скунд натискання кнопки
GTimer<millis> tmrFlash(500, true);  // для блимання

// pin Arduino
const int8_t Rele = 2;
const int8_t Buzer = 3;
const int8_t BuzerLED = 4;
const int8_t RFin = 8;
const int8_t Key = 11;

// pults
const char pult1cl[] = { 0x2E, 0x62, 0xF9, 0x49, 0xCB, 0xF9, 0x4B, 0 };
const char pult1op[] = { 0x2E, 0x62, 0xF9, 0x49, 0xCB, 0xF9, 0x79, 0 };
const char pult2cl[] = { 0x22, 0x2E, 0xFB, 0xFB, 0x49, 0xF9, 0x4B, 0 };
const char pult2op[] = { 0x22, 0x2E, 0xFB, 0xFB, 0x49, 0xF9, 0x79, 0 };

#define SIZEBUF 16           //розмір вхідний буфер
char inbuf[SIZEBUF] = "";    //вхідний буфер
unsigned int NumRepeat = 0;  // лічильник повторів
// unsigned long prevMillis = 0;  // час останньої зміни стану
// const long interval = 200;     // інтервал між настискання і відпусканням
// unsigned long currMillis = 0;
// bool flag_wait = 0;   // флаг що чекаємо кнопку
// bool flag_close = 0;  //
// bool flag_open = 0;   //


//unsigned long beep_prevMs = 0;   // для біпера
//unsigned long beep_curMs = 0;    // для біпера

//unsigned long beep_timer = 0;     // для біпера
//const long beep_interval = 1000;  // інтервал біпера


static void (*STATE)();  // автомат станів


void setup() {
  Serial.begin(115200);
  mySerial.begin(2400);

  pinMode(Rele, OUTPUT);
  pinMode(Buzer, OUTPUT);
  pinMode(BuzerLED, OUTPUT);
  pinMode(RFin, INPUT);
  pinMode(Key, INPUT_PULLUP);

  SoundStop();
  SoundStart();

  STATE = &state01;

  tmrWaitPressKey.setMode(GTMode::Timeout);
  tmrWaitPressKey.setTime(5000);

  mySerial.setTimeout(5);
}

void loop() {
  (*STATE)();
}

// =================== state01 ====================
// Очікуємо натискання кнопки пульта
// Розпізнаємо і реагуємо
// Після подачі живлення ми тут
void state01() {

  switch (KeyPultPressed()) {

    case 0:
      STATE = &state01;
      NumRepeat = 0;
      return;

    case 1:  // OPEN
      NumRepeat++;
      if (!digitalRead(Rele)) {  // якщо раніше не натиснули ВІДКРИТИ
        tmrWaitPressKey.start();
        STATE = &state02;
        return;
      }

    case 2:  //CLOSE
      NumRepeat = 0;
      STATE = &state03;
      return;
  }
}


//================== state02 ===========================
// очікуємо натискання на К1 протягом 5 сек
//open
void state02() {
  //блимаємо
  if (tmrFlash) digitalWrite(BuzerLED, !digitalRead(BuzerLED));

  //перевіряємо настискання Key
  if (!digitalRead(Key)) {        // Key при натисканні дає 0
    digitalWrite(BuzerLED, LOW);  // вимикаємо щоб не пищало і не блимало
    STATE = &state04;
    SoundStart();
    return;
  }

  if (tmrWaitPressKey) {  // час очікування вийшов
    //Stop();
    SoundStop();
    digitalWrite(BuzerLED, LOW);  // вимикаємо щоб не пищало і не блимало
    STATE = &state01;
    return;
  }

  if (KeyPultPressed() == 1) NumRepeat++;

  if (KeyPultPressed() == 2) {
    NumRepeat = 0;
    digitalWrite(BuzerLED, LOW);  // вимикаємо щоб не пищало і не блимало
    STATE = &state03;
    return;
  }

  STATE = &state02;
  Serial.println(NumRepeat);
  if (NumRepeat == 20) {
    digitalWrite(BuzerLED, LOW);  // вимикаємо щоб не пищало і не блимало
    NumRepeat = 0;
    STATE = &state04;
  }
}



// Паркуємось
void state03() {
  MotorStop();
  SoundStop();
  STATE = &state01;
}

//Їдемо
void state04() {
  MotorStart();
  if (KeyPultPressed() == 2) {  // STOP
    NumRepeat = 0;
    SoundStop();
    STATE = &state03;
    return;
  }

  STATE = &state04;
}




void sound1() {
  digitalWrite(Buzer, HIGH);
  delay(40);
  digitalWrite(Buzer, LOW);
  delay(40);
}

void sound2() {
  digitalWrite(BuzerLED, HIGH);
  delay(40);
  digitalWrite(BuzerLED, LOW);
  delay(40);
}

// можна їхати- вмикаємо двигун
void MotorStart() {
  digitalWrite(Rele, HIGH);
}
// і пікаємо на старті
void SoundStart() {
  sound1();
  sound1();
}

// вимикаємо двигун
void MotorStop() {
  digitalWrite(Rele, LOW);
}

void SoundStop() {
  sound2();
  sound2();
  sound2();
}

byte KeyPultPressed() {  // чи натискали кнопку пульта
  // вертає 0 - не натискали
  // вертає 1 - натиснули ВІДКРИТИ
  // вертає 2 - натиснули ЗАКРИТИ
  byte flag = 0;

  //mySerial.begin(2400);
   pinMode(RFin, INPUT);
  delay(50);

  if (mySerial.available() > SIZEBUF) {
    for (byte i = 0; i < SIZEBUF; i++) {
      inbuf[i] = mySerial.read();
    }
    if (strstr(inbuf, pult1op)) flag = 1;
    if (strstr(inbuf, pult2op)) flag = 1;
    if (strstr(inbuf, pult1cl)) flag = 2;
    if (strstr(inbuf, pult2cl)) flag = 2;
  }


  if (flag) {
    Serial.print("Pult Key ");
    Serial.println(flag);
  }

   pinMode(RFin, OUTPUT);
  //mySerial.end();
  return flag;
}