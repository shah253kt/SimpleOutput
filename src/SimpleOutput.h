#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

enum SimpleOutputMode
{
  OUTPUT_MODE_NORMAL,
  OUTPUT_MODE_BLINK,
  OUTPUT_MODE_SINGLE_SHOT
};

class SimpleOutput
{
  private:
    uint8_t mPin, mOperatingMode;
    uint16_t mOnDuration, mOffDuration;
    uint32_t mCycleStartsAt;
    bool mInverted;

#if defined(ARDUINO_STM_NUCLEO_F103RB) || defined(ARDUINO_GENERIC_STM32F103C)
    WiringPinMode mMode;
#else
    uint8_t mMode;
#endif

  public:
#if defined(ARDUINO_STM_NUCLEO_F103RB) || defined(ARDUINO_GENERIC_STM32F103C)
    SimpleOutput(uint8_t pin, WiringPinMode mode = OUTPUT, bool inverted = false);
#else
    SimpleOutput(uint8_t pin, bool inverted = false);
#endif

    void invert();
    void update();
    void onFor(uint16_t onDuration);
    void blink(uint16_t onDuration, uint16_t offDuration);
    void setState(bool ledState);
    void on();
    void off();
    void setPWM(uint8_t pwm);

#if defined(ARDUINO_STM_NUCLEO_F103RB) || defined(ARDUINO_GENERIC_STM32F103C)
    void setHighResPWM(uint16_t pwm);
#endif
};
