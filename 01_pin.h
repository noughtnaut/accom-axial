#ifndef ACCOM_PIN
#define ACCOM_PIN

#include <Arduino.h>

class Pin {
private:
  int number;     // Pin number on the Teensy board, as passed to `pinMode()`
  int mode;       // Pin input/output mode, as passed to `pinMode()`
  int actState;   // Pass (HIGH|LOW) to specify active signal level
  int initState;  // Pass (HIGH|LOW) to specify initial signal level

public:
  Pin(int numberThis, int actStateThis) {
    number = numberThis;
    mode = INPUT;
    actState = actStateThis;

    pinMode(number, INPUT_PULLUP);
  }

  Pin(int numberThis, int modeThis, int actStateThis, int initStateThis) {
    number = numberThis;
    mode = modeThis;
    actState = actStateThis;
    initState = initStateThis;

    pinMode(number, mode);
    if (mode == OUTPUT) {
      setState(initState);
    }
  }

  void setState(int newState) {
    digitalWriteFast(number, newState==LOW ? LOW : HIGH);
    //Serial.printf("setState(%i,%i) -> %s\n", number , newState, newState?"HIGH":"LOW");
  }

  void setActive(bool activate) {
    //Serial.printf("setActive(%i,%s) -> ", number , activate?"true":"false");
    if (LOW == actState)
      if (activate)
        setState(LOW);
      else
        setState(HIGH);
    else if (activate)
      setState(HIGH);
    else
      setState(LOW);
  }

  bool isActive() {
    //Serial.printf("isActive(%i) -> %s\n", number, actState == digitalReadFast(number)?"true":"false");
    return actState == digitalReadFast(number);
  }

  bool isHigh() {
    //Serial.printf("isHigh(%i) -> %s\n", number, digitalReadFast(number)?"true":"false");
    return LOW != digitalReadFast(number);
  }

  int getNumber() {
    return number;
  }
};

#endif