#include "mlib/pin.h"
#include "mlib/pow.h"
#include "mlib/sync.h"
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
  Context *ctx = (Context *)args;
  for (int i = 0; !ctx->canceld; i = !i) {
    digitalWrite(RedPin, i);
    digitalWrite(GreenPin, !i);
    delay(LIGHT_SWAP_MS);
    SPINLOCK_LOCK(ctx->wait);
  }
}
void *voice_thread(void *args) {
  Context *ctx = (Context *)args;
  const beep beps[] = {{1000, 500}, {500, 500}};
  for (int i = 0; !ctx->canceld; i = (i + 1) % len(beps)) {
    softToneWrite(BeepPin, beps[i].freq);
    delay(beps[i].delay_ms);
    SPINLOCK_LOCK(ctx->wait);
  }
}
#define TURNDIRCL 1
#define TURNDIRCR 0
#define TURN90SPEED 30
#define TURN90DURA 300
void turn_dirc90(int turn_dirc) {
  pow_drive(MODEPOWUP, turn_dirc ? DIRCPOWLEFT : DIRCPOWRIGHT, TURNMODEREV,
            TURN90SPEED, TURN90DURA, COMBONONE);
}
int main() {
  // Context *ctx = make_ctx();
  // pthread_t light_th, voice_th;
  // pthread_create(&light_th, NULL, light_thread, (void *)ctx);
  // pthread_create(&voice_th, NULL, voice_thread, (void *)ctx);
  // pow_drive(MODEPOWUP, DIRCPOWLINE, TURNMODEREV, 30, 6000, COMBONONE);
  // pow_drive(MODEPOWUP, DIRCPOWLEFT, TURNMODEREV, 30, 300, COMBONONE);
  // pow_drive(MODEPOWUP, DIRCPOWLINE, TURNMODEREV, 30, 3000, COMBONONE);
  // pow_drive(MODEPOWUP, DIRCPOWLINE, TURNMODEREV, 30, 3000, COMBONONE);
  // pow_drive(MODEPOWSTOP, DIRCPOWLINE, TURNMODEREV, 0, 6000, COMBONONE);
}