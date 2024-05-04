#include "7seg.h"
#include <SPI.h>

seg7::SegSPIMasterDisplay::SegSPIMasterDisplay(uint8_t pin_cs) {
  pin_cs_ = pin_cs;
}

uint8_t seg7::SegSPIMasterDisplay::sendCommand(const uint8_t &id, const uint8_t &secret_key,
                                               const uint8_t &value) const {
#ifdef SPI_HAS_TRANSACTION
  SPI.beginTransaction(SPISettings(SEG_SPI_TRANSACTION_CLOCK, MSBFIRST, SPI_MODE0));
#endif
  uint8_t first = secret_key << 4;
  first |= id;
  digitalWrite(pin_cs_, LOW);
  SPI.transfer(first);
  asm volatile("nop");
  SPI.transfer(value);
  asm volatile("nop");
  uint8_t result = SPI.transfer(0xFF);
  asm volatile("nop");
  digitalWrite(pin_cs_, HIGH);
#ifdef SPI_HAS_TRANSACTION
  SPI.endTransaction();
  delayMicroseconds(SEG_SPI_TRANSFER_DELAY_MICROSECONDS);
#endif
  return result;
}

void seg7::SegSPIMasterDisplay::begin(bool auto_set) {
  digitalWrite(pin_cs_, HIGH);
  pinMode(pin_cs_, OUTPUT);
  if(auto_set) {
    delayMicroseconds(100);
    uint8_t count = requestDigitCount();
    setDigitCount(count);
    setFlags(0);
  }
}

void seg7::SegSPIMasterDisplay::setDigitCount(const uint8_t &count) {
  digit_count_ = count;
}

uint8_t seg7::SegSPIMasterDisplay::requestDigitCount() {
  return sendCommand(SEG_SPI_CMD_GET_COUNT, 0, 0);
}

void seg7::SegSPIMasterDisplay::setFlags(const uint8_t &flags) const {
  sendCommand(SEG_SPI_CMD_SET_FLAGS, 0, flags);
}

void seg7::SegSPIMasterDisplay::writeDigit(const uint8_t &index, const uint8_t &data) const {
  sendCommand(SEG_SPI_CMD_SET_DIGIT, index, data);
}

uint8_t seg7::SegSPIMasterDisplay::digitCount() const {
  return digit_count_;
}

void seg7::SegSPIMasterDisplay::set(int value) const {
  const uint8_t& count = digitCount();
  if(count > 10) {
    return;
  }
  auto* arr = new uint8_t[count];
  convertFormatNumberToData(value, arr, digit_count_);
  for(uint8_t idx = 0; idx < count; idx++) {
    writeDigit(idx, arr[idx]);
  }
  delete[] arr;
}

void seg7::SegSPIMasterDisplay::clear() const {
  sendCommand(SEG_SPI_CMD_CLEAR, 0, 0);
}

void seg7::SegSPIMasterDisplay::setDot(const uint8_t &index, bool state) const {
  sendCommand(SEG_SPI_CMD_SET_DOT, index, state);
}

uint8_t seg7::SegSPIMasterDisplay::readDigit(const uint8_t &index) const {
  return sendCommand(SEG_SPI_CMD_GET_COUNT, index, 0);
}
