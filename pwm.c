#include "mlib/pin.h"
#include "mlib/utils.h"
#include <softPwm.h>
#include <wiringPi.h>
#define MODEPOWSTOP 0
#define MODEPOWUP 5
#define MODEPOWDOWN 6
#define DIRCPOWLINE 0
#define DIRCPOWLEFT 2
#define DIRCPOWRIGHT 1

void init() {
  wiringPiSetup();
  const int pins[6] = {POWPWML, POWIN1L, POWIN2L, POWPWMR, POWIN1R, POWIN2R};
  pinModeSetter(OUTPUT, pins, len(pins));
  softPwmCreate(POWPWML, 0, 100);
  softPwmCreate(POWPWMR, 0, 100);
}
void pow_drive(int mode, int dirc, int speed, int time_ms) {
  int pin2 = bitvis(mode, 1);
  int pin1 = bitvis(mode, 0);
  int revl = bitvis(dirc, 1);
  int revr = bitvis(dirc, 0);
  softPwmWrite(POWPWML, speed);
  digitalWrite(POWIN2L, irev(revl, pin2));
  digitalWrite(POWIN1L, irev(revl, pin1));
  softPwmWrite(POWPWMR, speed);
  digitalWrite(POWIN2R, irev(revr, pin2));
  digitalWrite(POWIN1R, irev(revr, pin1));
  delay(time_ms);
}
int main() {
  init();
  test();
}
int test() {
  int dura = 1000;
  int pow = 20;
  pow_drive(MODEPOWUP, DIRCPOWLINE, pow, dura);
  pow_drive(MODEPOWDOWN, DIRCPOWLINE, pow, dura);
  pow_drive(MODEPOWUP, DIRCPOWLEFT, 40, dura);
  pow_drive(MODEPOWUP, DIRCPOWRIGHT, 40, dura);
  pow_drive(MODEPOWSTOP, DIRCPOWLINE, 0, dura);
}