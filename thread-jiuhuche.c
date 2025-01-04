#include "mlib/pin.h"
#include "mlib/pow.h"
#include "mlib/utils.h"
#include <pthread.h>
#include <unistd.h>

#define LIGHT_SWAP_MS 200
typedef struct {
  int freq;
  int delay_ms;
} beep;
void init() {
  wiringPiSetup();
  const menum pins[6] = {POWPWML, POWIN1L, POWIN2L,  POWPWMR,
                         POWIN1R, POWIN2R, GreenPin, RedPin};
  pinset(pins, OUTPUT);
  softPwmCreate(POWPWML, 0, 100);
  softPwmCreate(POWPWMR, 0, 100);
}
void *light_thread(void *args) {
  int swap_flg = 0;
  while (1) {
    digitalWrite(RedPin, swap_flg);
    digitalWrite(GreenPin, !swap_flg);
    delay(LIGHT_SWAP_MS);
    swap_flg = !swap_flg;
  }
}
void *voice_thread(void *args) {
  const beep beps[] = {{1000, 500}, {500, 500}};
  for (int i = 0;; i = (i + 1) % len(beps)) {
    softToneWrite(BeepPin, beps[i].freq);
    delay(beps[i].delay_ms);
  }
}
int main() {
  pthread_t light_th, voice_th;
  pthread_create(&light_th, NULL, light_thread, NULL);
  // pthread_create(&voice_th, NULL, light_thread, NULL);
  pow_drive(MODEPOWUP, DIRCPOWLINE, TURNMODEREV, 30, 6000, COMBONONE);
  pow_drive(MODEPOWUP, DIRCPOWLEFT, TURNMODEREV, 30, 300, COMBONONE);
  pow_drive(MODEPOWUP, DIRCPOWLINE, TURNMODEREV, 30, 3000, COMBONONE);
  pow_drive(MODEPOWSTOP, DIRCPOWLINE, TURNMODEREV, 0, 6000, COMBONONE);
}