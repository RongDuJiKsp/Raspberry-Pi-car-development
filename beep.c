#include <softTone.h>
#include <wiringPi.h>
#define BuzPin 0
#define NIL 0
#define LEDL 21
#define LEDR 22
#define len(x) (sizeof(x) / sizeof(*x))
void open_light(int pin) { digitalWrite(pin, HIGH); }
void close_light(int pin) { digitalWrite(pin, LOW); }
typedef unsigned int uint32_t;
typedef struct {
  int freq;
  int delay_ms;
} beep;
#include "melody.h"
void playsound(const beep *arr, int size) {
  for (int i = 0; i < size; i++) {
    softToneWrite(BuzPin, arr[i].freq);
    delay(arr[i].delay_ms);
    digitalWrite(LEDL, i & 1);
    digitalWrite(LEDR, !(i & 1));
  }
}

int init() {
  wiringPiSetup();
  softToneCreate(BuzPin);
  pinMode(LEDL, OUTPUT);
  pinMode(LEDR, OUTPUT);
  return 0;
}
int main() {
  if (init() != NIL) {
    return -1;
  }
  playsound(melody, len(melody));
}
