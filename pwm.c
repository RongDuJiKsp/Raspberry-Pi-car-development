#include "mlib/pow.h"
void init() {
  wiringPiSetup();
  const menum pins[6] = {POWPWML, POWIN1L, POWIN2L, POWPWMR, POWIN1R, POWIN2R};
  pinset(pins, OUTPUT);
  softPwmCreate(POWPWML, 0, 100);
  softPwmCreate(POWPWMR, 0, 100);
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