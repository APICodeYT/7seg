#include "7seg.h"

#define SEG_PIN_CONNECTED(x) (x != SEG_PIN_NC)

seg7::SegDisplay::SegDisplay(const seg7::SegType &seg_type,
                                                         const uint8_t &pin_digit_1,
                                                         const uint8_t &pin_digit_2,
                                                         const uint8_t &pin_digit_3,
                                                         const uint8_t &pin_digit_4,
                                                         const uint8_t &pin_digit_5,
                                                         const uint8_t &pin_digit_6,
                                                         const uint8_t &pin_a,
                                                         const uint8_t &pin_b,
                                                         const uint8_t &pin_c,
                                                         const uint8_t &pin_d,
                                                         const uint8_t &pin_e,
                                                         const uint8_t &pin_f,
                                                         const uint8_t &pin_g,
                                                         const uint8_t &pin_p) {
  seg_type_ = seg_type;
  pin_a_ = pin_a;
  pin_b_ = pin_b;
  pin_c_ = pin_c;
  pin_d_ = pin_d;
  pin_e_ = pin_e;
  pin_f_ = pin_f;
  pin_g_ = pin_g;
  pin_p_ = pin_p;
  if(SEG_PIN_CONNECTED(pin_digit_1)) {
    pin_digits[digit_count_++] = pin_digit_1;
  }
  if(SEG_PIN_CONNECTED(pin_digit_2)) {
    pin_digits[digit_count_++] = pin_digit_2;
  }
  if(SEG_PIN_CONNECTED(pin_digit_3)) {
    pin_digits[digit_count_++] = pin_digit_3;
  }
  if(SEG_PIN_CONNECTED(pin_digit_4)) {
    pin_digits[digit_count_++] = pin_digit_4;
  }
  if(SEG_PIN_CONNECTED(pin_digit_5)) {
    pin_digits[digit_count_++] = pin_digit_5;
  }
  if(SEG_PIN_CONNECTED(pin_digit_6)) {
    pin_digits[digit_count_++] = pin_digit_6;
  }
  digit_data_ = new uint8_t[digit_count_];
}

void seg7::SegDisplay::setFlags(const uint8_t& flags) {
  flags_ = flags;
}

void seg7::SegDisplay::begin() {
  if(seg_type_ == SegType::Anode) {
    for(uint8_t idx = 0; idx < digit_count_; idx++) {
      pinMode(pin_digits[idx], OUTPUT);
    }
  }
  clear();
#ifdef SEG_SPI_SLAVE
  setupSlaveDevice();
#endif
}

void seg7::SegDisplay::writeDigit(const uint8_t& index, const uint8_t& data) {
  if(index >= digit_count_) {
    return;
  }
  digit_data_[index] = data;
}

uint8_t seg7::SegDisplay::readDigit(const uint8_t& index) {
  if(index >= digit_count_) {
    return 0;
  }
  return digit_data_[index];
}

uint8_t seg7::SegDisplay::digitCount() const {
  return digit_count_;
}

void seg7::SegDisplay::set(int value) {
  convertFormatNumberToData(value, digit_data_, digit_count_);
}

void seg7::SegDisplay::clear() {
  for(uint8_t idx = 0; idx < digit_count_; idx++) {
    digit_data_[idx] = SEG_EMPTY;
  }
}

void seg7::SegDisplay::setDot(const uint8_t& index, bool state) {
  if(index >= digit_count_) {
    return;
  }
  if(state) {
    digit_data_[index] = SEG_SET_DOT(digit_data_[index]);
    return;
  }
  digit_data_[index] = SEG_UNSET_DOT(digit_data_[index]);
}

void seg7::SegDisplay::doUpdate() {
  if(flags_ & SEG_FLAG_HIDDEN) {
    return;
  }
  for(uint8_t idx = 0; idx < digit_count_; idx++) {
    renderDigit(idx, digit_data_[idx]);
  }
}

void setPin(const seg7::SegType& type, const uint8_t& pin, bool state) {
  if(!SEG_PIN_CONNECTED(pin)) {
    return;
  }
  switch (type) {
    case seg7::SegType::Anode:
      pinMode(pin, state ? OUTPUT : INPUT);
      digitalWrite(pin, LOW);
      break;
    case seg7::SegType::Cathode:
      pinMode(pin, OUTPUT);
      digitalWrite(pin, state ? HIGH : LOW);
      break;
    default:break;
  }
}

void seg7::SegDisplay::renderDigit(const uint8_t& index, const uint8_t& data) {
  const uint8_t& pin = flags_ & SEG_FLAG_REVERSED ? pin_digits[digit_count_ - 1 - index] : pin_digits[index];
  setPin(seg_type_, pin_a_, data & 0x01);
  setPin(seg_type_, pin_b_, data & 0x02);
  setPin(seg_type_, pin_c_, data & 0x04);
  setPin(seg_type_, pin_d_, data & 0x08);
  setPin(seg_type_, pin_e_, data & 0x10);
  setPin(seg_type_, pin_f_, data & 0x20);
  setPin(seg_type_, pin_g_, data & 0x40);
  setPin(seg_type_, pin_p_, data & 0x80);
  if(seg_type_ == SegType::Anode) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    delay(2);
    digitalWrite(pin, LOW);
  } else {
    digitalWrite(pin, LOW);
    pinMode(pin, OUTPUT);
    delay(2);
    pinMode(pin, INPUT);
  }
}
