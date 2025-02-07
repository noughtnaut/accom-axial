#ifndef ACCOM_PIN
#define ACCOM_PIN

#include <Arduino.h>

class Pin {
private:
  int number; // Pin number on the Teensy board, as passed to `pinMode()`
  int mode; // Pin input/output mode, as passed to `pinMode()`
  int actState; // Pass (HIGH|LOW) to specify active signal level

public:

  Pin(int numberThis, int modeThis, int actStateThis, bool initiallyActive) {
    String modeStr = String(
      modeThis==INPUT?"INPUT"
      :modeThis==OUTPUT?"OUTPUT"
      :modeThis==INPUT_PULLUP?"INPUT_PULLUP"
      :String(modeThis).c_str()
    );
    number = numberThis;
    mode = modeThis;
    actState = actStateThis;

    pinMode(number, mode);
    if (mode == OUTPUT) {
      setActive(initiallyActive);
    }
  }

  int getNumber() {
    return number;
  }

  bool isActive() {
    return actState == digitalReadFast(number);
  }

  void setState(int newState) {
    digitalWriteFast(number, newState==LOW ? LOW : HIGH);
  }

  void setActive(bool activate) {
    if (LOW!=actState) {
      setState(activate?HIGH:LOW);
    } else {
      setState(activate?LOW:HIGH);
    }
  }

  void on() {
    setActive(true);
  }

  void off() {
    setActive(false);
  }

  void blink(int onDuration, int offDuration) {
    on(); delay(onDuration);
    off(); delay(offDuration);
  }

  static Pin getLed() {
    return Pin(LED_BUILTIN, OUTPUT, HIGH, true);
  }
};

#endif