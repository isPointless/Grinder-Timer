#pragma once

class Adafruit_SSD1306;

class Display{
  private:
    Adafruit_SSD1306* display;
    int menuState;
    float weight;

  public:
    Display();
    void printSingleDose();
    void printDoubleDose();
    void printTime(double time);
    void printreset();
    bool resetState = true;
    void printWeight(float weight);
    bool targetWeightMode;
    void printSpeed(float Speed, float targetTime);
    float targetTime;
    float Speed;
    bool acceptNewYES;
    void acceptNew(float targetTime);
    void printMenu();
    void printMenuState(int menuState);
    void off(); 
};