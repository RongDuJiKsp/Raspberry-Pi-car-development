#include "mlib/pin.h"
#include "mlib/pow.h"
#include "mlib/sync.h"
#include "mlib/utils.h"
#include <pthread.h>
#include <signal.h>
#include <softTone.h>
#include <unistd.h>

#define LIGHT_SWAP_MS 200
typedef struct {
  int freq;
  int delay_ms;
} beep;

void init() {
  wiringPiSetup();
  const menum pins[8] = {POWPWML, POWIN1L, POWIN2L,  POWPWMR,
                         POWIN1R, POWIN2R, GreenPin, RedPin};
  pinset(pins, OUTPUT);
  softToneCreate(BeepPin);
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
  const beep beps[] = {{1000, 300}, {500, 300}};
  for (int i = 0; !ctx->canceld; i = (i + 1) % len(beps)) {
    softToneWrite(BeepPin, beps[i].freq);
    delay(beps[i].delay_ms);
  }
}
#define TURNDIRCL 1
#define TURNDIRCR 0

#define TURN90SPEED 50
#define TURN90DURA 580
void turn_dirc90(int turn_dirc) {
  pow_drive(MODEPOWUP, turn_dirc ? DIRCPOWLEFT : DIRCPOWRIGHT, TURNMODEREV,
            TURN90SPEED, TURN90DURA, COMBONONE);
}
#define LINESPEED 50
#define LINEDURA 1200
void line() {
  pow_drive(MODEPOWUP, DIRCPOWLINE, TURNMODEREV, LINESPEED, LINEDURA,
            COMBONONE);
}
void stop() {
  pow_drive(MODEPOWSTOP, DIRCPOWLINE, TURNMODEREV, 30, 5, COMBONONE);
}
void run(Context *ctx) {
  // 1左 2右 3右 4左 5右 6右 7左 8右 9右 10左 11右
  // 跑出来就是一个十字
  const int dirc[11] = {TURNDIRCL, TURNDIRCR, TURNDIRCR, TURNDIRCL,
                        TURNDIRCR, TURNDIRCR, TURNDIRCL, TURNDIRCR,
                        TURNDIRCR, TURNDIRCL, TURNDIRCR};
  for (int i = 0; i < len(dirc); i++) {
    line();
    ctx->wait = BTrue;
    turn_dirc90(dirc[i]);
    ctx->wait = BFalse;
  }
  line();
}
void handle_sigint() {
  stop();
  exit(0);
}
int main() {
  init();
  Context *ctx = make_ctx();
  pthread_t light_th, voice_th;
  pthread_create(&light_th, NULL, light_thread, (void *)ctx);
  pthread_create(&voice_th, NULL, voice_thread, (void *)ctx);
  signal(SIGINT, handle_sigint);
  run(ctx);
  stop();
}