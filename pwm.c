#include "mlib/pow.h"
void init() {
  wiringPiSetup();
  const menum pins[6] = {POWPWMLPin, POWIN1LPin, POWIN2LPin,
                         POWPWMRPin, POWIN1RPin, POWIN2RPin};
  pinset(pins, OUTPUT);
  softPwmCreate(POWPWMLPin, 0, 100);
  softPwmCreate(POWPWMRPin, 0, 100);
}

void test();
int main() {
  init();
  test();
}
void test() {
  float speedcomb = 1.2;
  pow_drive(MODEPOWUP, DIRCPOWLINE, TURNMODEREV, 20 * speedcomb, 3000,
            COMBONONE);
  pow_drive(MODEPOWDOWN, DIRCPOWLINE, TURNMODEREV, 20 * speedcomb, 1000,
            COMBONONE);
  pow_drive(MODEPOWUP, DIRCPOWLEFT, TURNMODEREV, 40 * speedcomb, 500,
            COMBONONE);
  pow_drive(MODEPOWUP, DIRCPOWRIGHT, TURNMODEREV, 40 * speedcomb, 500,
            COMBONONE);
  pow_drive(MODEPOWUP, DIRCPOWLEFT, TURNMODERUN, 36 * speedcomb, 3000, 0.4);
  pow_drive(MODEPOWUP, DIRCPOWRIGHT, TURNMODERUN, 36 * speedcomb, 3000, 0.4);
  pow_drive(MODEPOWDOWN, DIRCPOWLEFT, TURNMODERUN, 36 * speedcomb, 1000, 0.4);
  pow_drive(MODEPOWDOWN, DIRCPOWRIGHT, TURNMODERUN, 36 * speedcomb, 1000, 0.4);
  pow_drive(MODEPOWSTOP, DIRCPOWLINE, TURNMODEREV, 0, 3000, COMBONONE);
}