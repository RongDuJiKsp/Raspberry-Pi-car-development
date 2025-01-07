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
  menum turning;
  int unhead_tick;
} cup_ctx;
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
#define P_delay 8
#define P_transdelay 5
#define P_trydelay 3
#define P_tryspeed 25
#define P_turncombo 0.2
#define P_speed 29
#define P_fastspeed 34
#define P_close_tick 5
#define P_unhead_max_tick 120
menum shouldCtx(byte status) {
  if ((status & LeftBit) && (!(status & RightBit)))
    return CtxTurnLeft;
  if ((!(status & LeftBit)) && (status & RightBit))
    return CtxTrunRight;
  return CtxLine;
}
void exec(menum sport) {
  switch (sport) {
  case CtxLine:
    pow_drive(MODEPOWUP, DIRCPOWLINE, TURNMODEREV, P_tryspeed, P_trydelay,
              COMBONONE);
    break;
  case CtxTurnLeft:
    pow_drive(MODEPOWUP, DIRCPOWLEFT, TURNMODEREV, P_tryspeed, P_trydelay,
              COMBONONE);
    break;
  case CtxTrunRight:
    pow_drive(MODEPOWUP, DIRCPOWRIGHT, TURNMODEREV, P_tryspeed, P_trydelay,
              COMBONONE);
    break;
  }
}
void cpu(cup_ctx *ctx, byte status) {
  if (status == 0) {
    if (ctx->turning == CtxLine) {
      ctx->unhead_tick += 1;
    }
    if (ctx->unhead_tick > P_unhead_max_tick) {
      ctx->turning = CtxTurnLeft;
      ctx->unhead_tick = 0;
    }
    exec(ctx->turning);
    ctx->sport = ctx->turning;
  } else if (shouldCtx(status) == CtxTurnLeft && ctx->sport == CtxLine) {
    ctx->turning = CtxTurnLeft;
    ctx->unhead_tick = 0;
    exec(ctx->sport);
  } else if (shouldCtx(status) == CtxTrunRight && ctx->sport == CtxLine) {
    ctx->turning = CtxTrunRight;
    ctx->unhead_tick = 0;
    exec(ctx->sport);
  } else if (status == 2 && ctx->sport != CtxLine) {
    pow_drive(MODEPOWUP, DIRCPOWLINE, TURNMODEREV, P_fastspeed, P_delay,
              COMBONONE);
    ctx->sport = CtxLine;
    ctx->turning = CtxLine;
    ctx->unhead_tick = 0;
  } else {
    exec(ctx->sport);
  }
}
void mainloop(int dbg) {
  cup_ctx *ctx = New(cup_ctx);
  ctx->sport = CtxLine;
  ctx->turning = CtxLine;
  ctx->unhead_tick = 0;
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