#include <Arduino.h>
#include <7seg.h>

#define PIN_A    14
#define PIN_B    15
#define PIN_C    16
#define PIN_D    17
#define PIN_E    18
#define PIN_F    19
#define PIN_G    9
#define PIN_D0   8
#define PIN_D1   7
#define PIN_D2   6
#define PIN_D3   5

using namespace seg7;

//
// Make sure that the definition `SEG_SPI_SLAVE` exists in 7seg.h
//

//display with 4 digits without dot pin
SegDisplay display(SegType::Anode,
                   PIN_D0, PIN_D1, PIN_D2,PIN_D3,
                   SEG_PIN_NC, SEG_PIN_NC,
                   PIN_A, PIN_B, PIN_C, PIN_D,
                   PIN_E, PIN_F, PIN_G, SEG_PIN_NC);

void setup() {
  display.begin();
}

void loop() {
  delay(1);
  display.doUpdate();
}