#include "7seg.h"

#ifdef SEG_SPI_SLAVE

uint8_t spi_counter_ = 0;
uint8_t spi_cmd_id_ = 0;
uint8_t spi_cmd_sk_ = 0;
uint8_t spi_value_ = 0;
uint8_t spi_result_ = 0;
seg7::SegDisplay* seg_display_ptr_;

void seg7::SegDisplay::setupSlaveDevice() {
  seg_display_ptr_ = this;
  pinMode(MISO, OUTPUT);
  pinMode(MOSI, INPUT);
  pinMode(SCK, INPUT);
  pinMode(SS, INPUT);
  SPCR |= _BV(SPE);
  SPCR |= _BV(SPIE);
}

uint8_t processCommandResult(const uint8_t &id, const uint8_t &secret_key) {
  if(id == SEG_SPI_CMD_GET_COUNT) {
    return seg_display_ptr_->digitCount();
  }
  if(id == SEG_SPI_CMD_GET_DIGIT) {
    return seg_display_ptr_->readDigit(secret_key);
  }
  return 0;
}

void processCommand(const uint8_t &id, const uint8_t &secret_key, const uint8_t &value) {
  switch (id) {
    case SEG_SPI_CMD_SET_DIGIT:
      seg_display_ptr_->writeDigit(secret_key, value);
      break;
    case SEG_SPI_CMD_SET_FLAGS:
      seg_display_ptr_->setFlags(value);
      break;
    case SEG_SPI_CMD_CLEAR:
      seg_display_ptr_->clear();
      break;
    default:break;
  }
}

void resetSlave() {
  spi_counter_ = spi_cmd_id_ = spi_cmd_sk_ = spi_value_ = spi_result_ = 0;
}

ISR(SPI_STC_vect) {
  uint8_t input = SPDR;
  if(spi_counter_ == 0) {
    spi_counter_++;
    spi_cmd_id_ = input & 0xF;
    spi_cmd_sk_ = input >> 4;
    spi_result_ = processCommandResult(spi_cmd_id_, spi_cmd_sk_);
    SPDR = spi_result_;
    return;
  }
  if(spi_counter_ == 1) {
    spi_counter_++;
    spi_value_ = input;
    SPDR = spi_result_;
    processCommand(spi_cmd_id_, spi_cmd_sk_, spi_value_);
    return;
  }
  if(input == 0xFF) {
    SPDR = spi_result_;
    resetSlave();
    return;
  }
  SPDR = spi_result_;
}

#endif
