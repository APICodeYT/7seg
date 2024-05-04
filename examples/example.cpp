#include <Arduino.h>
#include <7seg.h>

#define PIN_A    14
#define PIN_B    15
#define PIN_C    16
#define PIN_D    17
#define PIN_E    18
#define PIN_F    19
#define PIN_G    9
#define PIN_P    4
#define PIN_D0   8
#define PIN_D1   7
#define PIN_D2   6
#define PIN_D3   5

using namespace seg7;

//display with 4 digits
SegDisplay display(SegType::Anode,
                   PIN_D0, PIN_D1, PIN_D2,PIN_D3,
                   SEG_PIN_NC, SEG_PIN_NC,
                   PIN_A, PIN_B, PIN_C, PIN_D,
                   PIN_E, PIN_F, PIN_G, PIN_P);

int counter = -100;

void setup() {
  display.begin();
  display.setFlags(SEG_FLAG_REVERSED);
}

void loop() {
  display.doUpdate();
  delay(1000);
  if(counter >= 100) { //show error if counter >= 100
    display.writeDigit(0, SEG_CHAR_E);
    display.writeDigit(1, SEG_CHAR_SMALL_R);
    display.writeDigit(2, SEG_CHAR_SMALL_R);
    display.writeDigit(3, SEG_EMPTY);
    return;
  }
  display.set(counter++);
}