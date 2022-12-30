#include <Grinder.h>
#include <Arduino.h>
#include "Definitions.h"

Grinder::Grinder() {
    pinMode(SSR, OUTPUT);
}

unsigned long Grinder::getTargetTime() {
    return targetTime;
}

void Grinder::on(double targetTime) {
    this->targetTime = millis() + targetTime * 1000;
    this->startTime = millis();
    digitalWrite(SSR, HIGH);
}

void Grinder::off() {
    digitalWrite(SSR, LOW);
}

void Grinder::continuous() { 
    this->startTime = millis();
    digitalWrite(SSR, HIGH);
}