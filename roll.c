#include "mlib/dbg.h"
#include "mlib/pin.h"
#include "mlib/pow.h"
#include "mlib/utils.h"
#include <signal.h>
#include <stdio.h>
void init() {
  wiringPiSetup();
  const menum sense[3] = {LeftTCRPin, MidTCRPin, RightTCRPin};
  pinset(sense, INPUT);
  const menum pins[8] = {POWPWMLPin, POWIN1LPin, POWIN2LPin, POWPWMRPin,
                         POWIN1RPin, POWIN2RPin, GreenPin,   RedPin};
  pinset(pins, OUTPUT);
  softPwmCreate(POWPWMLPin, 0, 100);
  softPwmCreate(POWPWMRPin, 0, 100);
}
void handle_sigint() {
  pow_drive(MODEPOWSTOP, DIRCPOWLINE, TURNMODERUN, 0, 0, COMBONONE);
  exit(0);
}
int main() {
  init();
  signal(SIGINT, handle_sigint);
  while (1) {
    pow_drive(MODEPOWUP, DIRCPOWRIGHT, TURNMODEREV, 30, 30, COMBONONE);
  }
}