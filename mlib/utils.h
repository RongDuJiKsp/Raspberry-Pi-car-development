#ifndef KSP_UTILS
#define KSP_UTILS
typedef int i32;
typedef unsigned int u32;
typedef long long int i64;
typedef unsigned long long int u64;
typedef float f32;
typedef double f64;
typedef char byte;
typedef i32 menum;
typedef int boolen;
#define BTrue 1
#define BFalse 0
#include <stdlib.h>
#include <wiringPi.h>
#define New(type) ((type *)calloc(1, sizeof(type)))
// len() 计算数组的长度
#define len(x) (sizeof(x) / sizeof(*(x)))
// bitvis()
#define bitvis(num, shf) ((((u64)(num)) >> (shf)) & 1)
#define irev(cond, expr) ((cond) ? (!(expr)) : (expr))
#define icombo(cond, num, combo) ((cond) ? (num * combo) : (num))
#define pinset(pins, mode)                                                     \
  do {                                                                         \
    for (int i = 0; i < len((pins)); i++) {                                    \
      pinMode((pins)[i], (mode));                                              \
    }                                                                          \
  } while (0)
#endif