/*
  HCSR04 - Library for arduino, for HC-SR04 ultrasonic distance sensor.
  Created by Martin Sosic, June 11, 2016.
*/

#ifndef HCSR04_C_H
#define HCSR04_C_H

#include "Arduino.h"

class UltraSonicDistanceSensor {
 public:
    /**
     * @param triggerPin  Digital pin that is used for controlling sensor (output).
     * @param echoPin  Digital pin that is used to get information from sensor (input).
     */
    UltraSonicDistanceSensor(uint8_t triggerPin, uint8_t echoPin);

    /**
     * Measures distance by sending ultrasonic waves and measuring time it takes them
     * to return.
     * @returns Distance in centimeters, or negative value if distance is greater than 400cm.
     */
    double measureDistanceCm();


    /**
     * Measures time it takes ultrasonic waves 
     * to return.
     * @returns time in microseconds.
     */
    unsigned long  measureTimeMicrosecond();
   
   /**
    *sets the speed of sound used to calculate distance 
    */
    void setSpeedOfSound(double newspeed){
       speedofSound = newspeed;
    }
 private:
    
    uint8_t triggerPin, echoPin;
    double speedofSound = 343.0;
    double maxDist = 200.0;
};

#endif // HCSR04_H
