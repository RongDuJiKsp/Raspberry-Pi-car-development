#ifndef KSP_UTILS
#define KSP_UTILS
#include <wiringPi.h>
#define len(x) (sizeof(x) / sizeof(*(x)))
#define bitvis(num, shf) (((num) >> (shf)) & 1)
#define irev(cond, expr) ((cond) ? (!(expr)) : (expr))
#define icombo(cond, num, combo) ((cond) ? (num * combo) : (num))
#define pinset(pins, mode)                                                     \
  do {                                                                         \
    for (int i = 0; i < len((pins)); i++) {                                    \
      pinMode((pins)[i], (mode));                                              \
    }                                                                          \
  } while (0)
#endif