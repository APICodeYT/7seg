#include <Arduino.h>
#include <7seg.h>
#include <SPI.h>

using namespace seg7;

#define PIN_CS 10

SegSPIMasterDisplay display(PIN_CS);

void setup() {
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV64);
  display.begin(true); //if auto_set enabled: digit count will be set automatically
}

int count = 0;

void loop() {
  delay(1000);
  if(count >= 100) {
    display.setFlags(SEG_FLAG_HIDDEN);
    return;
  }
  display.set(count++);
}