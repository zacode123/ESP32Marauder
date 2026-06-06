#include "Switches.h"

Switches::Switches() {
	this->pin = 0;
	this->pin = false;
	this->pressed = false;
	this->hold_lim = 2000;
	this->cur_hold = 0;
	this->isheld = false;
	
	pinMode(this->pin, INPUT);
	
	return;
}

Switches::Switches(int pin, uint32_t hold_lim, bool pullup) {
	this->pin = pin;
	this->pullup = pullup;
	this->hold_lim = hold_lim;
	this->cur_hold = 0;
	this->isheld = false;
	
  if (pullup)
  	pinMode(this->pin, INPUT_PULLUP);
  else
    pinMode(this->pin, INPUT_PULLDOWN);

	this->pressed = this->getButtonState();
	
	
	return;
}

int Switches::getPin() {
	return this->pin;
}

bool Switches::getPullup() {
	return this->pullup;
}

bool Switches::isHeld() {
	return this->isheld;
}

bool Switches::getButtonState() {
	int buttonState = digitalRead(this->pin);
	
	if ((this->pullup) && (buttonState == LOW))
		return true;
	else if ((!this->pullup) && (buttonState == HIGH))
		return true;
	else
		return false;
}

bool Switches::justPressed() {
    bool btn_state = this->getButtonState();

    if (btn_state && !this->pressed) {
        if ((millis() - lastDebounceTime) < debounceDelay)
            return false;

        lastDebounceTime = millis();

        this->hold_init = millis();
        this->pressed = true;
        return true;
    }
    else if (btn_state) {
        if ((millis() - this->hold_init) < this->hold_lim)
            this->isheld = false;
        else
            this->isheld = true;

        this->pressed = true;
        return false;
    }
    else {
        this->pressed = false;
        this->isheld = false;
        return false;
    }
}

bool Switches::justReleased() {
    bool btn_state = this->getButtonState();

    if (!btn_state && this->pressed) {

        if ((millis() - lastDebounceTime) < debounceDelay)
            return false;

        lastDebounceTime = millis();

        this->isheld = false;
        this->pressed = false;
        return true;
    }

    this->pressed = btn_state;
    return false;
}
