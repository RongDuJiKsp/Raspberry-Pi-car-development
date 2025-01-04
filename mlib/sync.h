#ifndef SYNC_LIB
#define SYNC_LIB
#include "utils.h"
#include <pthread.h>
#include <stdlib.h>
typedef struct {
  pthread_mutex_t mutex;
  boolen canceld;
} Context;
Context *make_ctx() {
  Context *ctx = New(Context);
  ctx->mutex = PTHREAD_MUTEX_INITIALIZER;
  ctx->canceld = BFalse;
  return ctx;
}

#endif