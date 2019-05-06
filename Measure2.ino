#include <TM1638lite.h>
#include "HCSR04_C.h"
#include <math.h>
#include "Aggregator.h"
#include "ftoa.h"

//Setup Measure Settings
#define MENU_DELAY 50
#define BUF_SIZE 140
#define CALLIBRATE_DISTANCE 100
#define MASURE_PAUSE 30
double buffer[BUF_SIZE];
  Aggregator firstMean;


//https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance

//setup sensor connections
uint8_t strobePin = 4, clockPin = 7, dataPin = 8;
TM1638lite tm(strobePin, clockPin, dataPin);

uint8_t triggerPin = 13, echoPin = 12;
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);

//program global variables - so it is not reallocatd and dealloated
char charbuff[9], charbuff2[9];

enum state
{
  IDLE = 1,
  MEASURE = 2,
  SHOW = 3,
  ERROR = 5,
};
state current = MEASURE;

void setup()
{
  // Serial.begin(9600);
  tm.reset();
}

void loop()
{
  uint8_t buttons = tm.readButtons();
  switch (current)
  {
  case IDLE:
    tm.displayText("--------");

    if ((buttons & 1) != 0) //button 1
    {
      current = MEASURE;
    }
   
    delayMicroseconds(MENU_DELAY);
    return;

  case MEASURE:
    tm.setLED(0, 1);
    measureMode();
    tm.setLED(0, 0);
    return;
  case ERROR:
    tm.setLED(1, 1);
    //tm.displayText("Error");
  case SHOW:
    if ((buttons & 2) != 0) //button 2
    {
      current = IDLE;
    }
    delay(MENU_DELAY);
    current = MEASURE;
    return;
  }
}

bool isInRange(double mean, double std, double data)
{
  return abs(mean - data) < 1.7 * std;
}

double calculateMean(int N, double *data_buffer, double calculate(void))
{
  firstMean.reset();
  double mean = 0;
  int j = 0;
  //Collect Data
  double min = -1, max = -1;
  for (int i = 0; i < N; i++)
  {

    data_buffer[i] = (*calculate)();

    delayMicroseconds((MASURE_PAUSE));
  }
  for (int i = 0; i < N; i++)
  {
    //reject faulty sensor data
    if (data_buffer[i] > 0)
    {
      mean += data_buffer[i];
      j++;
      firstMean.addData(data_buffer[i]);

      if (min == -1 || data_buffer[i] < min)
        min = data_buffer[i];

      if (max == -1 || data_buffer[i] > max)
        max = data_buffer[i];
    }
  }

  double std = firstMean.getStd();
  // if(isInRange(mean, std,firstMean.getMean())
  // double

  mean /= j;
  // Serial.println("::::Mean:");
  // Serial.println(mean, 3);
  // Serial.println(firstMean.getMean(), 3);
  // Serial.println(mean - firstMean.getMean(), 11);
  // Serial.println(std, 11);
  // Serial.println(j);
  // mean = firstMean.getMean();

  firstMean.reset();

  //Use only good quality Data
  for (int i = 0; i < N; i++)
  {
    if (isInRange(mean, std, data_buffer[i]) && data_buffer[i] > min + (max - min) / 10 && data_buffer[i] < max - (max - min) / 10)
    {
      firstMean.addData(data_buffer[i]);
      // Serial.print(data_buffer[i]);
      // Serial.print(",");
      // Serial.println(firstMean.getMean(), 3);
    }
  }

  // Serial.println(firstMean.getStd(), 3);
  // Serial.println(firstMean.getMean()-mean, 3);
  //   Serial.println(firstMean.getCount()/(float)j);
  // Serial.println(":::::Mean:::");

  return firstMean.getMean();
}

void displayNumber(double data)
{
  ftoa8_1(charbuff2, data);
  pad(charbuff2, charbuff, 8, ' ');
  tm.displayText(charbuff);
}
double getDistanceCm()
{
  return distanceSensor.measureDistanceCm();
}

void measureMode()
{
  //tm.reset();
  double distance = calculateMean(BUF_SIZE, buffer, &getDistanceCm);
  tm.setLED(1, 0);
  //measurement error
  if (distance <= 2.0)
  {
    current = ERROR;
    return;
  }

  displayNumber(distance);
  delayMicroseconds(1500);

  current = SHOW;
  return;
}
