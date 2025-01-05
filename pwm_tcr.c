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
// Heigh Heigh Low  应该左转
#define StatusLeft 6
// Low Heigh Heigh 应该右转
#define StatusRight 3
byte sense_status() {
  // 检测到黑线为高电平
  return ((digitalRead(LeftTCRPin) & 1) << 2) |
         ((digitalRead(MidTCRPin) & 1) << 1) |
         ((digitalRead(RightTCRPin) & 1) << 0);
}
#define P_delay 10
#define P_transdelay 260
#define P_turncombo 0.4
#define P_speed 30
void mainloop(int dbg) {
  while (1) {
    byte status = sense_status();
    digitalWrite(GreenPin, (status & 4));
    digitalWrite(RedPin, (status & 1));
    if (dbg) {
      printf("%s%s%s\n", toStringBool(status & 4), toStringBool(status & 2),
             toStringBool(status & 1));
      delay(30);
      continue;
    }
    if (status == StatusLeft) {
      pow_drive(MODEPOWUP, DIRCPOWLEFT, TURNMODERUN, P_speed, P_transdelay,
                P_turncombo);
    } else if (status == StatusLeft) {
      pow_drive(MODEPOWUP, DIRCPOWRIGHT, TURNMODERUN, P_speed, P_transdelay,
                P_turncombo);
    } else {
      pow_drive(MODEPOWUP, DIRCPOWLINE, TURNMODEREV, P_speed, P_delay,
                COMBONONE);
    }
  }
}
void handle_sigint() {
  pow_drive(MODEPOWSTOP, DIRCPOWLINE, TURNMODERUN, P_speed, P_delay, COMBONONE);
  exit(0);
}
int main() {
  init();
  signal(SIGINT, handle_sigint);
  mainloop(BFalse);
}