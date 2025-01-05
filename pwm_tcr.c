#include "mlib/dbg.h"
#include "mlib/pin.h"
#include "mlib/pow.h"
#include "mlib/utils.h"
#include <signal.h>
#include <stdio.h>
#define CtxTurnLeft 1
#define CtxTrunRight 2
#define CtxLine 0
typedef struct {
  menum sport;
  int reset_tick;
} cup_ctx;
void updatectx(cup_ctx *ctx, menum sport) {
  ctx->sport = sport;
  ctx->reset_tick = 0;
}
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
#define LeftBit 4
#define MidBit 2
#define RightBit 1
byte sense_status() {
  // 检测到黑线为高电平
  return ((digitalRead(LeftTCRPin) & 1) << 2) |
         ((digitalRead(MidTCRPin) & 1) << 1) |
         ((digitalRead(RightTCRPin) & 1) << 0);
}
#define P_delay 10
#define P_transdelay 10
#define P_trydelay 5
#define P_turncombo 0.2
#define P_speed 28
#define P_fastspeed 38
#define P_close_tick 5
void exec(menum sport) {
  switch (sport) {
  case CtxLine:
    pow_drive(MODEPOWUP, DIRCPOWLINE, TURNMODEREV, P_speed, P_trydelay,
              COMBONONE);
    break;
  case CtxTurnLeft:
    pow_drive(MODEPOWUP, DIRCPOWLEFT, TURNMODEREV, P_speed, P_trydelay,
              COMBONONE);
    break;
  case CtxTrunRight:
    pow_drive(MODEPOWUP, DIRCPOWRIGHT, TURNMODEREV, P_speed, P_trydelay,
              COMBONONE);
    break;
  }
}
void cpu(cup_ctx *ctx, byte status) {
  if (ctx->reset_tick <= 3) {
    exec(ctx->sport);
    ctx->reset_tick += 1;
    return;
  }
  if (status == 2) { // 010
    updatectx(ctx, CtxLine);
    pow_drive(MODEPOWUP, DIRCPOWLINE, TURNMODEREV, P_fastspeed, P_delay,
              COMBONONE);
  } else if (status & MidBit) {
    if (status & LeftBit) {
      pow_drive(MODEPOWUP, DIRCPOWLEFT, TURNMODEREV, P_speed, P_transdelay,
                COMBONONE);
      ctx->sport = CtxTurnLeft;
      ctx->reset_tick = -P_close_tick;
    } else if (status & RightBit) {
      pow_drive(MODEPOWUP, DIRCPOWRIGHT, TURNMODEREV, P_speed, P_transdelay,
                COMBONONE);
      ctx->sport = CtxTrunRight;
      ctx->reset_tick = -P_close_tick;
    }
  } else if (status == 5 || status == 7) {
    exec(ctx->sport);
    ctx->reset_tick += 1;
  } else if (status & LeftBit) {
    pow_drive(MODEPOWUP, DIRCPOWLEFT, TURNMODEREV, P_speed, P_transdelay,
              COMBONONE);
    updatectx(ctx, CtxTurnLeft);
  } else if (status & RightBit) {
    pow_drive(MODEPOWUP, DIRCPOWRIGHT, TURNMODEREV, P_speed, P_transdelay,
              COMBONONE);
    updatectx(ctx, CtxTrunRight);
  } else {
    pow_drive(MODEPOWUP, DIRCPOWLINE, TURNMODEREV, P_speed, P_delay, COMBONONE);
    ctx->reset_tick += 1;
  }
}
void mainloop(int dbg) {
  cup_ctx *ctx = New(cup_ctx);
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
    cpu(ctx, status);
  }
}
void handle_sigint() {
  pow_drive(MODEPOWSTOP, DIRCPOWLINE, TURNMODERUN, P_speed, P_delay, COMBONONE);
  exit(0);
}
int main(int argc, char **argv) {
  init();
  signal(SIGINT, handle_sigint);
  mainloop(argc > 1);
}