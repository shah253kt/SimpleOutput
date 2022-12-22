#include "SimpleOutput.h"

#if defined(ARDUINO_STM_NUCLEO_F103RB) || defined(ARDUINO_GENERIC_STM32F103C)
SimpleOutput::SimpleOutput(uint8_t pin, WiringPinMode mode, bool inverted)
#else
SimpleOutput::SimpleOutput(uint8_t pin, bool inverted)
#endif
    : mPin(pin),
      mOperatingMode(OUTPUT_MODE_NORMAL),
      mInverted(inverted)
{
    pinMode(mPin, mMode);
    off();
#if defined(ARDUINO_STM_NUCLEO_F103RB) || defined(ARDUINO_GENERIC_STM32F103C)
    mMode = mode;
#else
    mMode = OUTPUT;
#endif
}

void SimpleOutput::invert()
{
    mInverted = !mInverted;
}

void SimpleOutput::update()
{
    if (mOperatingMode == OUTPUT_MODE_NORMAL)
    {
        return;
    }
    
    uint32_t elapsedTime = millis() - mCycleStartsAt;
    bool ledState = elapsedTime < mOnDuration;
    setState(ledState);

    if (mOperatingMode == OUTPUT_MODE_BLINK && elapsedTime >= mOnDuration + mOffDuration)
    {
        mCycleStartsAt = millis();
    }
}

void SimpleOutput::onFor(uint16_t onDuration)
{
    mOperatingMode = OUTPUT_MODE_SINGLE_SHOT;
    mOnDuration = onDuration;
    mCycleStartsAt = millis();
    setState(true);
}

void SimpleOutput::blink(uint16_t onDuration, uint16_t offDuration)
{
    mOperatingMode = OUTPUT_MODE_BLINK;
    mOnDuration = onDuration;
    mOffDuration = offDuration;
    mCycleStartsAt = millis();
}

void SimpleOutput::setState(bool ledState)
{
    if (mMode == OUTPUT)
    {
        digitalWrite(mPin, ledState ^ mInverted);
    }
    else
    {
        setPWM(ledState ? 255 : 0);
    }
}

void SimpleOutput::on()
{
    mOperatingMode = OUTPUT_MODE_NORMAL;
    setState(true);
}

void SimpleOutput::off()
{
    mOperatingMode = OUTPUT_MODE_NORMAL;
    setState(false);
}

void SimpleOutput::setPWM(uint8_t pwm)
{
    analogWrite(mPin, pwm);
}

#if defined(ARDUINO_STM_NUCLEO_F103RB) || defined(ARDUINO_GENERIC_STM32F103C)
void SimpleOutput::setHighResPWM(uint16_t pwm)
{
    pwmWrite(mPin, pwm);
}
#endif
