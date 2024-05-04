#ifndef ESP32WORKSPACE_SRC_7SEG_H_
#define ESP32WORKSPACE_SRC_7SEG_H_
#include <Arduino.h>


namespace seg7 {

//add definition for SPI support as slave
//#define SEG_SPI_SLAVE

#define SEG_SPI_TRANSACTION_CLOCK             200000
#define SEG_SPI_TRANSFER_DELAY_MICROSECONDS   150

#define SEG_DIGIT_0 0x3F
#define SEG_DIGIT_1 0x06
#define SEG_DIGIT_2 0x5B
#define SEG_DIGIT_3 0x4F
#define SEG_DIGIT_4 0x66
#define SEG_DIGIT_5 0x6D
#define SEG_DIGIT_6 0x7D
#define SEG_DIGIT_7 0x07
#define SEG_DIGIT_8 0x7F
#define SEG_DIGIT_9 0x6F

#define SEG_CHAR_MINUS      0x40
#define SEG_CHAR_DEGREE     0x63
#define SEG_CHAR_A          0x77
#define SEG_CHAR_C          0x39
#define SEG_CHAR_E          0x79
#define SEG_CHAR_F          0x71
#define SEG_CHAR_SMALL_R    0x50
#define SEG_EMPTY           0x00

#define SEG_SET_DOT(x)      (x | 0x80)
#define SEG_UNSET_DOT(x)    (x & 0x7F)

#define SEG_FLAG_HIDDEN     0x1
#define SEG_FLAG_REVERSED   0x2

#define SEG_PIN_NC 0xFF

#define SEG_SPI_CMD_GET_DIGIT 0
#define SEG_SPI_CMD_GET_COUNT 1
#define SEG_SPI_CMD_SET_DIGIT 2
#define SEG_SPI_CMD_SET_FLAGS 3
#define SEG_SPI_CMD_CLEAR     4
#define SEG_SPI_CMD_SET_DOT   5

enum class SegType {
  Cathode,
  Anode
};


uint8_t convertDigitToData(uint8_t digit);

void convertFormatNumberToData(int value, uint8_t* digit_buf, uint8_t digit_count);

class SegDisplay {
  SegType seg_type_;
  uint8_t pin_a_;
  uint8_t pin_b_;
  uint8_t pin_c_;
  uint8_t pin_d_;
  uint8_t pin_e_;
  uint8_t pin_f_;
  uint8_t pin_g_;
  uint8_t pin_p_;
  uint8_t pin_digits[6]{};
  uint8_t digit_count_ = 0;
  uint8_t flags_ = 0;
  uint8_t* digit_data_;

  void renderDigit(const uint8_t& index, const uint8_t& data);

#ifdef SEG_SPI_SLAVE
  void setupSlaveDevice();
#endif

 public:
  explicit SegDisplay(const SegType& seg_type,
                           const uint8_t& pin_digit_1 = SEG_PIN_NC,
                           const uint8_t& pin_digit_2 = SEG_PIN_NC,
                           const uint8_t& pin_digit_3 = SEG_PIN_NC,
                           const uint8_t& pin_digit_4 = SEG_PIN_NC,
                           const uint8_t& pin_digit_5 = SEG_PIN_NC,
                           const uint8_t& pin_digit_6 = SEG_PIN_NC,
                           const uint8_t& pin_a = SEG_PIN_NC,
                           const uint8_t& pin_b = SEG_PIN_NC,
                           const uint8_t& pin_c = SEG_PIN_NC,
                           const uint8_t& pin_d = SEG_PIN_NC,
                           const uint8_t& pin_e = SEG_PIN_NC,
                           const uint8_t& pin_f = SEG_PIN_NC,
                           const uint8_t& pin_g = SEG_PIN_NC,
                           const uint8_t& pin_p = SEG_PIN_NC
                           );

  void begin();

  void setFlags(const uint8_t& flags);

  void writeDigit(const uint8_t& index, const uint8_t& data);

  uint8_t readDigit(const uint8_t& index);

  uint8_t digitCount() const;

  void set(int value);

  void clear();

  void setDot(const uint8_t& index, bool state);

  void doUpdate();

};

class SegSPIMasterDisplay {
  uint8_t pin_cs_;
  uint8_t digit_count_ = 1;

  uint8_t sendCommand(const uint8_t& id, const uint8_t& secret_key, const uint8_t& value) const;

 public:
  explicit SegSPIMasterDisplay(uint8_t pin_cs);

  void begin(bool auto_set = false);

  void setDigitCount(const uint8_t& count);

  uint8_t requestDigitCount();

  void setFlags(const uint8_t& flags) const;

  uint8_t digitCount() const;

  void writeDigit(const uint8_t& index, const uint8_t& data) const;

  uint8_t readDigit(const uint8_t& index) const;

  void set(int value) const;

  void clear() const;

  void setDot(const uint8_t& index, bool state) const;

};

}

#endif //ESP32WORKSPACE_SRC_7SEG_H_
