#include "mlib/pin.h"
#include <stdio.h>
#include <wiringPi.h>
void init() {
  wiringPiSetup();
  pinMode(RedPin, OUTPUT);
  pinMode(GreenPin, OUTPUT);
  pinMode(BtnPin, INPUT);
  digitalWrite(RedPin, LOW);
  digitalWrite(GreenPin, LOW);
  delay(1000);
}
void btnLoop() {
  while (1) {
    int spin = digitalRead(BtnPin);
    delay(20);
    if (spin != digitalRead(BtnPin)) {
      continue;
    }
    digitalWrite(RedPin, !spin);
    digitalWrite(GreenPin, spin);
  }
}
int main() {
  init();
  btnLoop();
}
