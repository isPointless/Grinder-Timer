#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "RotaryEncoder.h"
#include "Definitions.h"
#include "Display.h"
#include "Dosage.h"
#include "Grinder.h"
#include <JC_Button.h>
#include <math.h>

double lastLedState;
RotaryEncoder *encoder;
Display *display;
Dosage *dosage;
Grinder *grinder;
enum displayState {SINGLE, DOUBLE, MENU};
enum menuState {WEIGHT, RESET, EXIT};
uint8_t displayState = DOUBLE;
uint8_t menuState = WEIGHT;

// State Machine
enum States {NORMAL, SETTINGS, SET_DOSE, GRINDING, CONTINUOUS, SET_RESET, WEIGHTMODE};
uint8_t state = NORMAL;

bool reset = false;
unsigned long startTime;
float GrinderSpeed;
float TargetTime;
bool lastGrind; //last singleDoseSelected full timed grind

void setup() {
  startTime = millis();
  encoder = new RotaryEncoder();
  display = new Display();
  dosage = new Dosage();
  grinder = new Grinder();
}

void(* resetFunc) (void) = 0;

void loop() {
  switch(state)
  {

  // NORMAL
    case NORMAL:
      // set new dose

      if (encoder->button1State() == true) { 
        if (displayState == MENU) {
        state = SETTINGS; // enter menu
        break;
        } 
        else {    // enter dose settings
        state = SET_DOSE;
        break;
        }
      }

      // start grinding
      if (encoder->button2State() == true && displayState != MENU) {
        state = GRINDING;
        break;
      }
      // continuous grinding
      if (encoder->button3State() == true) {
        state = CONTINUOUS;
        break;
        }

      // Menu cycling
      if (encoder->wasTurnedLeft() ) {
        if (displayState <= 0) { 
          displayState = 2;
        } else { 
        displayState --;
        }
        startTime = millis();
      } 
      if (encoder->wasTurnedRight() ) { 
        if (displayState >= 2) { 
          displayState = 0;
        } else { 
        displayState ++;
        }
        startTime = millis();
      }

      // Display Single, Double or Menu
      if (displayState == 0) { 
        dosage->singleDoseSelected = true;
        display->printSingleDose();
      }
      if (displayState == 1) { 
        dosage->singleDoseSelected = false;
        display->printDoubleDose();
      }
      
      if (displayState == 2) { 
        display->printMenu();
      }
      while (startTime + 60000 < millis() ) {    //display off after 60s idling to prevent OLED burn-in
        display->off();
        encoder->ledFade();
        if (encoder->action() ) { 
          startTime = millis();
          break;
        }
      }

    encoder->ledFade();
      break;

// MENU / SETTINGS
    case SETTINGS:
    // display
    startTime = millis();
    while (encoder->button1State() == false) {
    display->printMenuState(menuState);
    
    if (encoder->wasTurnedLeft()) {
      if (menuState == 0) { 
        menuState = 2;
      } else {
      menuState --;
        }
      startTime = millis();
    }
      if (encoder->wasTurnedRight()) {
      menuState ++;
      if (menuState == 3) { 
         menuState = 0;
        }
      startTime = millis();
      }
      // make sure menu doesnt exit criteria
      if (menuState > 2 || menuState < 0) {
        menuState = 1;
      }
      // exit after 20s idle
      if (startTime + 20000 < millis()) { 
        menuState = EXIT;
        break;
        }
    
      encoder->ledFade();
    }

      if (menuState == WEIGHT) {
        state = WEIGHTMODE;
      }
      if (menuState == RESET) {
        state = SET_RESET;
      } 
      if (menuState == EXIT) {
        state = NORMAL;
      }
      startTime = millis();
      break;


// SET DOSE 
    case SET_DOSE:
      startTime = millis();
      encoder->ledOn();
      while (encoder->button1State() == false) { 

      if (encoder->Value() != 0) {
      (dosage->singleDoseSelected ? dosage->singleDoseTime : dosage ->doubleDoseTime) -= (encoder->encValue/100.0);
      startTime = millis();
      }

        // time < 0 = 0
      if (dosage->singleDoseTime < 0) { 
        dosage->singleDoseTime = 0;
      }
      if (dosage->doubleDoseTime < 0) { 
        dosage->doubleDoseTime = 0;
      }

        // print & write EEPROM
      display->printTime(dosage->singleDoseSelected ? dosage->singleDoseTime : dosage->doubleDoseTime);
      dosage->writeToEEPROM();

      // quit if takes too long (10s)
      if (startTime + 10000 < millis()) { 
        encoder->ledReset();
        break;
      }
      }
      startTime = millis();
      state = NORMAL;
      encoder->ledReset();
      break;

// TIMED GRINDING MODE
    case GRINDING:
    startTime = millis();
      grinder->on(dosage->singleDoseSelected ? dosage->singleDoseTime : dosage->doubleDoseTime);
      while (millis() < grinder->getTargetTime()) {
        display->printTime((grinder->getTargetTime() - millis()) / 1000.0);
        if (encoder->checkstop() == true) { 
          grinder->off();
          encoder->ledOn();
          delay (1000); // hold this screen for a while - also to allow button unpress (holding makes it start again..!)
          encoder->button2State(); //update button, so it doesnt start again automagicly
          state = NORMAL;
          break;
        }

        encoder->ledFlash(true);
      };

      if (state == NORMAL) { 
        encoder->ledReset();
        break;
      }

      grinder->off();
      encoder->ledOn();
      display->printTime(0.0);
      delay(500); // show 0.0 on display for a longer time
      state = NORMAL;
      lastGrind = dosage->singleDoseSelected;
      startTime = millis();
      encoder->ledReset();
      break;

// Continuous grinding
    case CONTINUOUS:
    startTime = millis();
    grinder->continuous();
    while (encoder->button3State() == true) { 
      display->printTime((millis() - grinder->startTime)/ 1000.0);
      encoder->ledFlash(false);
    }
    grinder->off();
    encoder->ledOn();
    delay(500); // hold total run time for a while
    state = NORMAL;
    startTime = millis();
    encoder->ledReset();
    break;


// RESET MODE
    case SET_RESET:
    encoder->ledOn();
    startTime = millis();
    display->resetState = false;
    
    while (encoder->button1State() == false) {
    display->printreset();
    if (encoder->wasTurnedRight() || encoder->wasTurnedLeft()) { 
      display->resetState = !display->resetState;
      startTime = millis();
     }
     if (startTime + 10000 < millis()) {  // 10s cancel  
        display->resetState = false;
        break;
      }
    }
    if (display->resetState == true) { 
      dosage->singleDoseTime = SINGLEDOSE_DEFAULT_TIME;
      dosage->doubleDoseTime = DOUBLEDOSE_DEFAULT_TIME;
      dosage->writeToEEPROM();
      resetFunc();
    }
    // back to NORMAL if NO is selected
    if (display->resetState == false) { 
      state = NORMAL;
      startTime = millis();
      encoder->ledReset();
      break;
    }


// WEIGHT MODE
    case WEIGHTMODE:
    // mode
    encoder->ledOn();
    display->targetWeightMode = false;
    dosage->dosageWeight = 15;
    dosage->targetWeight = 15;
    display->acceptNewYES = false;
    
    while (encoder->button1State() == false) { 
      // enter last dose weight
      if (encoder->Value() != 0) { 
        dosage->dosageWeight -= (encoder->encValue/10.0);
      }
      // weight < 0 = 0 && >100 = 99.9
      if (dosage->dosageWeight < 0) { 
        dosage->dosageWeight = 0;
      }
      if (dosage->dosageWeight >= 100) {
        dosage->dosageWeight = 99.9;
      }
      // update display
      display->printWeight(dosage->dosageWeight);
    } 

    // to use same function 
    display->targetWeightMode = true;

    while (encoder->button1State() == false) { 
      // enter target weight
    if (encoder->Value() != 0) { 
        dosage->targetWeight -= (encoder->encValue/10.0);
      }
      // weight < 0 = 0
      if (dosage->targetWeight < 0) { 
        dosage->targetWeight = 0;
      }
      if (dosage->targetWeight >= 100) { 
        dosage->targetWeight = 99.9;
      }

      // update display
      display->printWeight(dosage->targetWeight);
      }

      // calculate stats
    GrinderSpeed = dosage->dosageWeight / (lastGrind ? dosage->singleDoseTime : dosage->doubleDoseTime);
    TargetTime = dosage->targetWeight / GrinderSpeed;

      // show stats
      while (encoder->button1State() == false) { 
        display->printSpeed(GrinderSpeed, TargetTime);
      }

      // accept new time?
      while (encoder->button1State() == false) { 
        if (encoder->wasTurnedLeft() || encoder->wasTurnedRight() ) { 
          display->acceptNewYES = !display->acceptNewYES;
        }
        display->acceptNew(TargetTime);
      }

      if (display->acceptNewYES == true) { 
        (lastGrind ? dosage->singleDoseTime : dosage->doubleDoseTime) = TargetTime;
      }

      // exit to menu displaying last grind;
      dosage->singleDoseSelected = lastGrind;
        if (lastGrind == true) { 
        displayState = 0;
      } else {
        displayState = 1;
      }
      startTime = millis();
      state = NORMAL;
      encoder->ledReset();
      break;
    }
  }
