#ifndef TEENSY_PIN_H
#define TEENSY_PIN_H

/**
 * Represents an input/output pin on an Arduino-compatible board.
**/
class TeensyPin {
private:
	char _number; // the pin number on the microcontroller board
	char _activeState; // Whether `LOW` or `HIGH` TTL voltage signifies an active signal
public:
	/**
	 * @return the pin number on the microcontroller board
	**/
	char getNumber() const;
	/**
	 * @return `LOW` or `HIGH`
	**/
	char getActiveState() const;
	/**
	 * @return `TRUE` if the pin is in its active state
	**/
	bool isActive() const;
	TeensyPin()= default;
	/**
	 * Connects to an input/output pin.
	 * @param mode `INPUT` or `OUTPUT`
	 * @param number the pin number on the microcontroller board
	 * @param activeState `LOW` or `HIGH`
	**/
	explicit TeensyPin(char mode, char number, char activeState);
};

/**
 * Represents an input pin on an Arduino-compatible board.
**/
class InputPin : public TeensyPin {
public:
	InputPin()= default;
	/**
	 * Connects to an input pin.
	 * @param number the pin number on the microcontroller board
	 * @param activeState `LOW` or `HIGH`
	**/
	InputPin(char number, char activeState);
};

/**
 * Represents an output pin on an Arduino-compatible board.
**/
class OutputPin : public TeensyPin {
public:
	OutputPin()= default;
	/**
	 * Connects to an output pin.
	 * @param number the pin number on the microcontroller board
	 * @param activeState `LOW` or `HIGH`
	**/
	OutputPin(char number, char activeState);
	/**
	 * @return the microcontroller board's bulit-in LED
	**/
	static OutputPin getLED();
	/**
	 * Switches the pin to its active state.
	**/
	void on() const;
	/**
	 * Switches the pin to its inactive state.
	**/
	void off() const;
	/**
	 * Switches the pin to its active state if TRUE is specified.
	 * @param activate whether to activate the pin
	**/
	void setActive(bool activate) const;
	/**
	 * Switches the pin's state between active and inactive.
	**/
	void toggle() const;
	/**
	 * Switches the pin's state active for a moment, then inactive.
	**/
	void blink(unsigned int onDuration, unsigned int offDuration) const;
	/**
	 * Switches the pin's state inactive for a moment, then active.
	**/
	void occlude(unsigned int offDuration, unsigned int onDuration) const;
};

#endif