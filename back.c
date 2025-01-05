#include "mlib/pin.h"
#include "mlib/pow.h"
#include "mlib/utils.h"
#include <signal.h>
void handle_sigint() {
  pow_drive(MODEPOWSTOP, DIRCPOWLINE, TURNMODERUN, 0, 5, COMBONONE);
  exit(0);
}
int main() {
  wiringPiSetup();
  const menum pins[8] = {POWPWMLPin, POWIN1LPin, POWIN2LPin,
                         POWPWMRPin, POWIN1RPin, POWIN2RPin};
  pinset(pins, OUTPUT);
  softPwmCreate(POWPWMLPin, 0, 100);
  softPwmCreate(POWPWMRPin, 0, 100);
  signal(SIGINT, handle_sigint);
  pow_drive(MODEPOWDOWN, DIRCPOWLINE, TURNMODERUN, 40, 50000, COMBONONE);
}
