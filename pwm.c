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
#define TURNMODEREV 1
#define TURNMODERUN 0
#define COMBONONE 0.0
void init() {
  wiringPiSetup();
  const menum pins[6] = {POWPWML, POWIN1L, POWIN2L, POWPWMR, POWIN1R, POWIN2R};
  pinset(pins, OUTPUT);
  softPwmCreate(POWPWML, 0, 100);
  softPwmCreate(POWPWMR, 0, 100);
}
void pow_drive(menum mode, menum dirc, menum turnmode, i32 speed, i32 time_ms,
               f32 turn_speed_combo) {
  int pin2 = bitvis(mode, 1);
  int pin1 = bitvis(mode, 0);
  int revl = bitvis(dirc, 1);
  int revr = bitvis(dirc, 0);
  softPwmWrite(POWPWML, icombo((!turnmode) && revl, speed, turn_speed_combo));
  digitalWrite(POWIN2L, irev(turnmode && revl, pin2));
  digitalWrite(POWIN1L, irev(turnmode && revl, pin1));
  softPwmWrite(POWPWMR, icombo((!turnmode) && revr, speed, turn_speed_combo));
  digitalWrite(POWIN2R, irev(turnmode && revr, pin2));
  digitalWrite(POWIN1R, irev(turnmode && revr, pin1));
  delay(time_ms);
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