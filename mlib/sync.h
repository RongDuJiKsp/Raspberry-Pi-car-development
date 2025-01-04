#ifndef SYNC_LIB
#define SYNC_LIB
#include "utils.h"
#include <pthread.h>
#include <stdlib.h>
typedef struct {
  pthread_mutex_t mutex;
  boolen canceld;
  boolen wait;
} Context;
Context *make_ctx() {
  Context *ctx = New(Context);
  ctx->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
  ctx->canceld = BFalse;
  ctx->wait = BFalse;
  return ctx;
}
#define SPINLOCK_LOCK_TIME 50
#define SPINLOCK_LOCK(spinlock)                                                \
  do {                                                                         \
    while (spinlock) {                                                         \
      delay(SPINLOCK_LOCK_TIME);                                               \
    }                                                                          \
  } while (0)
#endif