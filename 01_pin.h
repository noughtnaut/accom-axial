#ifndef ACCOM_PIN
#define ACCOM_PIN

#include <Arduino.h>

class Pin {
private:
  int number;     // Pin number on the Teensy board, as passed to `pinMode()`
  int mode;       // Pin input/output mode, as passed to `pinMode()`
  int actState;   // Pass (HIGH|LOW) to specify active signal level

public:

//  Pin() {
//    Serial.println("Pin()");
//    Pin(99, INPUT, HIGH, false);
//  }
//
//  Pin(int numberThis) {
//    Serial.println("Pin(1)");
//    Pin(numberThis, INPUT, HIGH, false);
//  }
//
//  Pin(int numberThis, int modeThis, int actStateThis) {
//    Serial.println("Pin(1,2,3)");
//    Pin(numberThis, modeThis, actStateThis, false);
//  }

  Pin(int numberThis, int modeThis, int actStateThis, bool initiallyActive) {
    String modeStr = String(
      modeThis==INPUT?"INPUT"
      :modeThis==OUTPUT?"OUTPUT"
      :modeThis==INPUT_PULLUP?"INPUT_PULLUP"
      :String(modeThis).c_str()
    );
//    Serial.printf("New Pin: %i, %s, %s, %s\n", numberThis, modeStr.c_str(), actStateThis==LOW?"LOW":"HIGH", initiallyActive?"on":"off");
    number = numberThis;
    mode = modeThis;
    actState = actStateThis;

    pinMode(number, mode);
    if (mode == OUTPUT) {
      setActive(initiallyActive);
    }
  }

  void setState(int newState) {
    digitalWriteFast(number, newState==LOW ? LOW : HIGH);
    //Serial.printf("setState(%i,%i) -> %s\n", number , newState, newState?"HIGH":"LOW");
  }

  void setActive(bool activate) {
    //Serial.printf("setActive(%i,%s) -> ", number , activate?"true":"false");
    setState(activate == (HIGH==actState)); // XNOR
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