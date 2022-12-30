#include <RotaryEncoder.h>
#include <Definitions.h>
#include <Encoder.h>
#include <JC_Button.h>
#include "Grinder.h"

Button button1(ENC_SW);
Button button2(START_BTN, 100, true);
Button button3(RUN_BTN, 100, true);

RotaryEncoder::RotaryEncoder() {
    encoder = new Encoder(ENC_CLK, ENC_DT);
    button1.begin();
    button2.begin();
    button3.begin();
    pinMode(RUN_BTN_LED, OUTPUT);
    pinMode(START_BTN_LED, OUTPUT);
}

// Return round value of encoder with tolerance -- 1 click = 1 value increase (depends on encoder type)
signed int RotaryEncoder::Value() {  
    if (encoder->read() >= ENC_TOL || encoder->read() <= (-ENC_TOL)) {
    this->encValue = round(encoder->read() / ENC_TOL);
    encoder->write(0);
    return encValue;
    }
    return 0;
}

bool RotaryEncoder::wasTurnedRight() {
    if (encoder->read() <= (- ENC_TOL)) {
        encoder->write(0);
        return true;
    }
    return false;
}

bool RotaryEncoder::wasTurnedLeft() {
    if (encoder->read() >= (ENC_TOL)) {
        encoder->write(0);
        return true;
    }
    return false;
}

bool RotaryEncoder::checkstop() { 
    button2.read(), button3.read();
    if (button2.isPressed() || button3.isPressed()) { 
    return true;
}
    return false;
}

bool RotaryEncoder::button1State() {        // ENC_SW
    button1.read();
    if (button1.wasReleased()) { 
        return true;
    }
    return false;
}

bool RotaryEncoder::button2State() {       // START_BTN
    button2.read();
    if (button2.wasReleased()) {
        return true;
    } 
    return false;
}

bool RotaryEncoder::button3State() {        // RUN_BTN
    button3.read();
    if (button3.isPressed()) {
    return true;
    }
    return false;
}

bool RotaryEncoder::action() { 
    button1.read(), button2.read(), button3.read();
    if (button1.wasReleased() || button2.wasReleased() || button3.wasReleased() || encoder->read() != 0) { 
        return true;
    }
    return false;
}

void RotaryEncoder::ledOff() { 
    analogWrite(START_BTN_LED, 0);
    analogWrite(RUN_BTN_LED, 0);
}

void RotaryEncoder::ledOn() { 
    analogWrite(START_BTN_LED, 255);
    analogWrite(RUN_BTN_LED, 255);
}

void RotaryEncoder::ledFade() {         // use sine wave for ledFade - cos(x) = 0 at pi and 3 pi. switching at low brightness looks better.
    if (this->sinValue >= 3*PI) { 
    this->sinValue = PI;
  }

  this->sinValue += ((millis() - this->lastFade) / fadeTime);  
  this->ledState = ((cos(this->sinValue) + 1) * 255) / 2;
  this->lastFade = millis();

    if (this->ledState != this->lastLedState) {   // writing takes a long time. only write if necessary.
    analogWrite(START_BTN_LED, this->ledState);
    analogWrite(RUN_BTN_LED, this->ledState);
    this->lastLedState = this->ledState;
    }
}

void RotaryEncoder::ledFlash(bool whatButton) {  // whatButton? -> True = START_BTN, false = RUN_BTN
    if ((millis() - this->lastFlash) > flashTime) {                 
        this->ledON = !this->ledON;
        this->lastFlash = millis();
            analogWrite(whatButton ? START_BTN_LED : RUN_BTN_LED, this->ledON ? 255 : 0);           // only writes every flashTime interval
            analogWrite(whatButton ? RUN_BTN_LED : START_BTN_LED, 0);                               // turn the other one off. 
    }
}

void RotaryEncoder::ledReset() {        // resets LEDfade to max brightness 
    this->sinValue = 2*PI;                  
    this->lastFade = millis();
}