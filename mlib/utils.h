#include <wiringPi.h>
#define len(x) (sizeof(x) / sizeof(*x))
#define bitvis(num, shf) ((num >> shf) & 1)
#define irev(cond, expr) (cond ? !expr : expr)
void pinModeSetter(int mode, const int *pins, int size) {
  for (int i = 0; i < size; i++) {
    pinMode(pins[i], mode);
  }
}