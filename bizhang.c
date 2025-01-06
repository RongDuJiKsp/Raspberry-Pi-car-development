#include "mlib/dbg.h"
#include "mlib/pin.h"
#include "mlib/pow.h"
#include "mlib/utils.h"
#include <signal.h>
#include <stdio.h>
void init() {
  wiringPiSetup();
  const menum inputs[2] = {LeftOAPin, RightOAPin},
              output[2] = {RedPin, GreenPin};
  pinset(inputs, INPUT);
  pinset(output, OUTPUT);
}
int main() {
  init();
  while (1) {
    // 有障碍物为低电平
    digitalWrite(GreenPin, !digitalRead(LeftOAPin));
    digitalWrite(RedPin, !digitalRead(RightOAPin));
  }
}