/*
  HCSR04 - Library for arduino, for HC-SR04 ultrasonic distance sensor.
  Created by Martin Sosic, June 11, 2016.
*/

#include "Arduino.h"
#include "HCSR04_C.h"

UltraSonicDistanceSensor::UltraSonicDistanceSensor(
        uint8_t triggerPin, uint8_t echoPin) {
    this->triggerPin = triggerPin;
    this->echoPin = echoPin;
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

double UltraSonicDistanceSensor::measureDistanceCm() {
    unsigned long durationMicroSec = measureTimeMicrosecond();
    //measured time is divided by 2 - because it travels in both directions, and then is scaled 
    double distanceCm = (durationMicroSec / 2.0) * 1e-4 * speedofSound ;
    
    if (distanceCm == 0.0 || distanceCm > maxDist) {
        return -1.0 ;
    } else {
        return distanceCm;
    }
}
 
unsigned long  UltraSonicDistanceSensor::measureTimeMicrosecond(){
    // Make sure that trigger pin is LOW.
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    // Hold trigger for 10 microseconds, which is signal for sensor to measure distance.
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    // Measure the length of echo signal, which is equal to the time needed for sound to go there and back.
    unsigned long durationMicroSec = pulseIn(echoPin, HIGH,1e5*maxDist/speedofSound);
    return durationMicroSec;
}