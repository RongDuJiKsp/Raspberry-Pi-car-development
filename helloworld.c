#include <stdio.h>
#include <wiringPi.h>
#define LEDL 21
#define LEDR 22
int main() {
  int err = wiringPiSetup();
  if (err == -1) {
    return err;
  }
  pinMode(LEDL, OUTPUT);
  pinMode(LEDR, OUTPUT);
  int swaper = 0;
  while (1) {
    if (swaper) {
      open_light(LEDL);
      close_light(LEDR);
    } else {
      open_light(LEDR);
      close_light(LEDL);
    }
    swaper = !swaper;
    delay(100);
  }
}
void open_light(int pin) { digitalWrite(pin, HIGH); }
void close_light(int pin) { digitalWrite(pin, LOW); }