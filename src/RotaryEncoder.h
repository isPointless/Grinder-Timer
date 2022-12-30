#pragma once 

class Button;
class Encoder;

class RotaryEncoder
{
  private:
    Encoder *encoder;
    int lastLedState;
    unsigned long lastFlash;
    int ledState;
    float sinValue;
    unsigned long lastFade;
    bool ledON = false;
    
  public:
    RotaryEncoder();
    bool wasTurnedLeft();
    bool wasTurnedRight();
    int Value();
    bool checkstop();
    bool button1State();
    bool button2State();
    bool button3State();
    float encValue;
    bool action();
    void ledOff();
    void ledOn();
    void ledFade();
    void ledFlash(bool whatButton);
    void ledReset();
};