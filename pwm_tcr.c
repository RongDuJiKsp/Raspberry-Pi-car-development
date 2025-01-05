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
#define StatusLeft 4
#define StatusRight 1
byte sense_status() {
  return ((digitalRead(LeftTCRPin) & 1) << 2) |
         ((digitalRead(MidTCRPin) & 1) << 1) |
         ((digitalRead(RightTCRPin) & 1) << 0);
}
#define P_delay 30
#define P_turncombo 0.2
#define P_speed 30
void mainloop() {
  while (1) {
    byte status = sense_status();
    digitalWrite(GreenPin, status & StatusLeft);
    digitalWrite(RedPin, status & StatusRight);
    if (!(status & StatusLeft)) {
      pow_drive(MODEPOWUP, DIRCPOWLEFT, TURNMODERUN, P_speed, P_delay,
                P_turncombo);
    } else if (!(status & StatusRight)) {
      pow_drive(MODEPOWUP, DIRCPOWRIGHT, TURNMODERUN, P_speed, P_delay,
                P_turncombo);
    } else {
      pow_drive(MODEPOWUP, DIRCPOWLINE, TURNMODERUN, P_speed, P_delay,
                COMBONONE);
    }
    delay(P_delay);
  }
}
void handle_sigint() {
  pow_drive(MODEPOWSTOP, DIRCPOWLINE, TURNMODERUN, P_speed, P_delay, COMBONONE);
  exit(0);
}
int main() {
  init();
  signal(SIGINT, handle_sigint);
  mainloop();
}