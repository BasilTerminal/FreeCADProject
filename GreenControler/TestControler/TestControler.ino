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

    Serial.print(" Ch# ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.print(analogRead(BatPins[i]));
    Serial.print('\t');

    digitalWrite(ChPins[1], HIGH);
    delay(1000);
    // digitalWrite(OkPins[i], HIGH);
    // delay(100);
    // digitalWrite(ChPins[i], LOW);
    // delay(100);
    // digitalWrite(OkPins[i], LOW);
    // delay(100);
  }

  Serial.print(" I_sens= ");
  Serial.println(Curent());
}

// void ChanelOn(byte n) {
//   for (byte i = 0; i < 5; i++) {
//     digitalWrite(ChPins[i], LOW);
//   }
//   digitalWrite(ChPins[n], HIGH);
//   //delay(100);
// }

// void ChanelOff(byte n) {
//   digitalWrite(ChPins[n], LOW);
//   //delay(100);
// }

// void OkayOn(byte n) {
//   digitalWrite(OkPins[n], HIGH);
// }

// void OkayOff(byte n) {
//   digitalWrite(OkPins[n], LOW);
// }

// boolean isBattery(byte n) {  // presence battery
//   if (val > 512) return true;
//   return false;
// }


int Curent() {  // return curent of battery in mA
  long sum = 0;
  float average;
  for (int i = 0; i < 1000; i++) {
    sum = sum + analogRead(Isens);
  }
  average=(float)sum/1000.0;
  //Serial.print(average);
  return int(((average - 512.0) * 48.2));
  // int16_t a, b, c, i, i0, i1, i2;
  // i0 = analogRead(Isens);
  // i1 = analogRead(Isens);
  // i2 = analogRead(Isens);
  // a = min(i0, i1);
  // b = min(i0, i1);
  // c = min(i1, i2);

  //  i=max(max(a, b), c);
  //   return (i-512)*50;



  // float average = 0;
  // for(int i = 0; i < 1000; i++) {
  //   average = average + (.19 * analogRead(Isens) -25) / 1000;//this is
  //   //for the 5A mode, if 20A or 30A mode, need to modify this formula to
  //   //(.19 * analogRead(A0) -25) for 20A mode and
  //   //(.044 * analogRead(A0) -3.78) for 30A mode
  //   delay(1);
  //   return average;
}
