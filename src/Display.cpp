#include "Definitions.h"
#include "Display.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

static const unsigned char PROGMEM cup[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC1, 0xC1, 0x80, 0x00, 0x01, 0x83, 0x83, 0x00, 0x00, 0x01,
    0x03, 0x03, 0x00, 0x00, 0x01, 0x03, 0x03, 0x00, 0x00, 0x01, 0x81, 0x83, 0x80, 0x00, 0x00, 0xC1,
    0xC1, 0x80, 0x00, 0x00, 0x60, 0xE0, 0xC0, 0x00, 0x00, 0x30, 0x60, 0x60, 0x00, 0x00, 0x30, 0x30,
    0x20, 0x00, 0x00, 0x30, 0x30, 0x20, 0x00, 0x00, 0x70, 0x60, 0xE0, 0x00, 0x00, 0xE0, 0xC1, 0xC0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xE0, 0x0F,
    0xFF, 0xFF, 0xFF, 0xF0, 0x0F, 0xFF, 0xFF, 0xFE, 0x18, 0x0F, 0xFF, 0xFF, 0xFE, 0x08, 0x0F, 0xFF,
    0xFF, 0xFE, 0x08, 0x0F, 0xFF, 0xFF, 0xFE, 0x08, 0x0F, 0xFF, 0xFF, 0xFE, 0x08, 0x0F, 0xFF, 0xFF,
    0xFE, 0x08, 0x0F, 0xFF, 0xFF, 0xFE, 0x18, 0x0F, 0xFF, 0xFF, 0xFF, 0xF0, 0x0F, 0xFF, 0xFF, 0xF8,
    0x00, 0x0F, 0xFF, 0xFF, 0xF8, 0x00, 0x0F, 0xFF, 0xFF, 0xF8, 0x00, 0x0F, 0xFF, 0xFF, 0xF8, 0x00,
    0x0F, 0xFF, 0xFF, 0xF8, 0x00, 0x07, 0xFF, 0xFF, 0xF0, 0x00, 0x07, 0xFF, 0xFF, 0xF0, 0x00, 0x03,
    0xFF, 0xFF, 0xE0, 0x00, 0x01, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x7F,
    0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

Display::Display() {
    display = new Adafruit_SSD1306(128, 64, &Wire, 4);

    if(!display->begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDR)) {
        for(;;);
    }

    display->cp437(true);
    display->clearDisplay();
    display->display();
}

void Display::printSingleDose() {
    display->clearDisplay();
    display->drawBitmap(
        (display->width()  - 40 ) / 2,
        (display->height() - 40) / 2,
        cup, 40, 40, 1);
    display->display();
}

void Display::printDoubleDose() {
    display->clearDisplay();
    display->drawBitmap(
        (display->width()  - 94 ) / 2,
        (display->height() - 40) / 2,
        cup, 40, 40, 1);
    display->drawBitmap(
        (display->width() + 14 ) / 2,
        (display->height() - 40) / 2,
        cup, 40, 40, 1);
    display->display();
}

void Display::printTime(double time) {
    display->clearDisplay();

    display->setTextColor(WHITE);
    display->setTextSize(4);
    display->setCursor(10, 20);
    time = time < 0 ? 0.0 : time;
    time = time < 100 ? time : 99.9;
    display->print(time, time < 10 ? 2 : 1);

    display->setTextSize(2);
    display->setCursor(110, 34);
    display->print("S");

    display->println();
    display->display();
}

void Display::printreset() { 
    display->clearDisplay();

    display->setTextColor(WHITE);
    display->setTextSize(2);
    display->setCursor(10,10);
    display->print("RESET?");

     display->setCursor(20, 40);
    display->print("YES");
    display->setCursor(80,40);
    display->print("NO");
    if (this->resetState == true) {
        display->drawLine(18,60,50,60,WHITE);
    } else {
        display->drawLine(78,60,110,60,WHITE);
    }
    display->display();
}

void Display::printWeight(float weight) { 
    display->clearDisplay();
   
    display->setTextColor(WHITE);
    display->setTextSize(2);
    
    if (this->targetWeightMode == false) {
    display->setCursor(10,10);     
    display->print("LAST DOSE");
    } else { 
        display->setCursor(25,10);
        display->print("TARGET?");
    }
    display->setCursor(10, 40);
    display->print(weight, 1);
    display->setCursor(70, 40);
    display->print("GRAM");

    display->display();
}
void Display::printSpeed(float Speed, float targetTime) { 
    display->clearDisplay();
    
    display->setTextColor(WHITE);
    display->setTextSize(2);

    display->setCursor(15,10);
    display->print("G/S");
    display->setCursor(10,40);
    display->print("->");
    display->setCursor(100,40);
    display->print("s");

    display->setCursor(70, 10);
    display->print(Speed, 1);

    display->setCursor(50,40);
    display->print(targetTime, targetTime < 10 ? 2 : 1);

    display->display();
}

void Display::acceptNew(float targetTime) { 
    display->clearDisplay();
    
    display->setTextColor(WHITE);
    display->setTextSize(2);

    display->setCursor(10,10);
    display->print("OK?");
    display->setCursor(65,10);
    display->print(targetTime, targetTime < 10 ? 2 : 1);
    
    display->setCursor(15, 40);
    display->print("YES");
    display->setCursor(80,40);
    display->print("NO");
    if (this->acceptNewYES == true) {
        display->drawLine(18,60,50,60,WHITE);
    } else {
        display->drawLine(78,60,110,60,WHITE);
    }
    display->display();
}

void Display::printMenu() { 
    display->clearDisplay();
     display->setTextColor(WHITE);
    display->setTextSize(3);

    display->setCursor(30,25);
    display->print("MENU");
    display->display();
}

void Display::printMenuState(int menuState) { 
    display->clearDisplay();
    display->setTextColor(WHITE);
    display->setTextSize(2);
    if (menuState == 0) { 
        display->setCursor(30,15),
        display->print("WEIGHT");
        display->setCursor(40,40),
        display->print("CALC");

    }
    if (menuState == 1) {
        display->setCursor(30,20),
        display->print("RESET?");
    }
    if (menuState == 2) { 
        display->setCursor(10,20),
        display->print("EXIT MENU");
    }
    display->display();
}

void Display::off() { 
    display->clearDisplay();
    display->display();
}