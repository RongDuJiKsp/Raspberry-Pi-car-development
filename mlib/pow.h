#ifndef POW_DRIVE
#define POW_DRIVE
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
#endif