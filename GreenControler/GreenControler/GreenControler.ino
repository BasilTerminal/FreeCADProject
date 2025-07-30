const uint8_t ChPins[] = { 3, 5, 7, 10, 12 };      //RED
const uint8_t OkPins[] = { 4, 6, 8, 9, 11 };       //GREEN
const uint8_t BatPins[] = { A4, A3, A2, A1, A0 };  //BATTERY

const uint8_t Isens = 5;

int val;
bool fChargeDone[] = { 0, 0, 0, 0, 0 };

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  for (byte i = 0; i < 5; i++) {
    pinMode(ChPins[i], OUTPUT);
    digitalWrite(ChPins[i], LOW);
    pinMode(OkPins[i], OUTPUT);
    digitalWrite(OkPins[i], LOW);
  }
}


void loop() {
  // put your main code here, to run repeatedly:


  for (byte i = 0; i < 5; i++) {

    Serial.print(" Ch# "); Serial.print(i);Serial.print(" = ");
    Serial.print(analogRead(BatPins[i]));Serial.print('\t');

    analogRead(Isens);


    digitalWrite(ChPins[i], HIGH);
    delay(100);
    digitalWrite(OkPins[i], HIGH);
    delay(100);
    digitalWrite(ChPins[i], LOW);
    delay(100);
    digitalWrite(OkPins[i], LOW);
    delay(100);
  }

  Serial.print(" I_sens= "); Serial.println(analogRead(Isens));
}

void ChanelOn(byte n) {
  for (byte i = 0; i < 5; i++) {
    digitalWrite(ChPins[i], LOW);
  }
  digitalWrite(ChPins[n], HIGH);
  //delay(100);
}

void ChanelOff(byte n) {
  digitalWrite(ChPins[n], LOW);
  //delay(100);
}

void OkayOn(byte n) {
  digitalWrite(OkPins[n], HIGH);
}

void OkayOff(byte n) {
  digitalWrite(OkPins[n], LOW);
}

boolean isBattery(byte n) {  // presence battery
  if (val > 512) return true;
  return false;
}


int16_t Curent() {  // return curent of battery in mA
  return analogRead(Isens);
}