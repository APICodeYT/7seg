#include "7seg.h"

uint8_t seg7::convertDigitToData(uint8_t digit) {
  switch (digit) {
    case 0:
      return SEG_DIGIT_0;
    case 1:
      return SEG_DIGIT_1;
    case 2:
      return SEG_DIGIT_2;
    case 3:
      return SEG_DIGIT_3;
    case 4:
      return SEG_DIGIT_4;
    case 5:
      return SEG_DIGIT_5;
    case 6:
      return SEG_DIGIT_6;
    case 7:
      return SEG_DIGIT_7;
    case 8:
      return SEG_DIGIT_8;
    case 9:
      return SEG_DIGIT_9;
    default:
      return 0;
  }
}

void seg7::convertFormatNumberToData(int value, uint8_t *digit_buf, uint8_t digit_count) {
  if(digit_count == 0) {
    return;
  }
  bool negative = value < 0;
  if(negative) {
    value *= -1;
  }
  for(uint8_t d = digit_count; d > 0; d--) {
    uint8_t index = d - 1;
    if(value <= 0) {
      if(negative) {
        digit_buf[index] = SEG_CHAR_MINUS;
        negative = false;
        continue;
      }
      digit_buf[index] = d == digit_count ? SEG_DIGIT_0 : SEG_EMPTY;
      continue;
    }
    uint8_t data = convertDigitToData(value % 10);
    digit_buf[index] = data;
    value /= 10;
  }
}