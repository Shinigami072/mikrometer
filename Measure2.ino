#include <TM1638lite.h>
#include <math.h>
#include "Aggregator.h"
#include "ftoa.h"
#include "NewPing.h"

//Setup Measure Settings
#define MENU_DELAY 50
#define BUF_SIZE 140
#define MIN_DIST 2.0
#define MAX_DIST 200.0
#define MAX_MEASURE 5000000
#define SPEED_OF_SOUND 343.4

//setup measure
#define STD_RANGE 1.7
#define MINMAX_RANGE 0.1

//display connections
#define PIN_STROBE 4
#define PIN_CLOCK 7
#define PIN_DATA 8
//sensor connections
#define PIN_TRIGGER 13
#define PIN_ECHO 12
double buffer[BUF_SIZE];
//https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance

//program global variables - so it is not reallocatd and dealloated
TM1638lite tm(PIN_STROBE, PIN_CLOCK, PIN_DATA);
NewPing sonar(PIN_TRIGGER, PIN_ECHO, MAX_DIST);
char charbuff[9], charbuff2[9];

enum state
{
  IDLE = 1,
  MEASURE = 2,
  SHOW = 3,
  ERROR = 5,
};
state current = IDLE;

void setup()
{
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
    tm.reset();
    tm.setLED(0, 1);
    measureMode();
    tm.setLED(0, 0);
    return;
  case ERROR:
    tm.setLED(1, 1);
  case SHOW:
    if ((buttons & 2) != 0) //button 2
    {
      tm.setLED(1, 0);
      current = IDLE;
    }
    delay(MENU_DELAY);
    return;
  }
}

bool isInRange(double mean, double std, double min, double max, double data)
{
  double minmax = (max - min) * MINMAX_RANGE;
  return (abs(mean - data) < STD_RANGE * std) && (data > min + minmax) && (data < max - minmax);
}

double calculateMean(int N, double *data_buffer, double calculate(void))
{
  unsigned long t = micros();
  Aggregator firstMean;

  //Collect Data
  double min = -1, max = -1;
  for (int i = 0; i < N; i++)
  {

    data_buffer[i] = (*calculate)();
    //reject faulty sensor data
    if (data_buffer[i] > 0)
    {
      firstMean.addData(data_buffer[i]);

      if (min == -1 || data_buffer[i] < min)
        min = data_buffer[i];

      if (max == -1 || data_buffer[i] > max)
        max = data_buffer[i];
    }
  }

  double std = firstMean.getStd();
  double mean = firstMean.getMean();

  firstMean.reset();

  //Use only good quality Data
  for (int i = 0; i < N; i++)
  {
    if (isInRange(mean, std, min, max, data_buffer[i]))
    {
      firstMean.addData(data_buffer[i]);
    }
  }

  //get as much additional data as possible
  while (micros() - t < MAX_MEASURE)
  {
    double d = (*calculate)();
    if (isInRange(mean, std, min, max, d))
    {
      firstMean.addData(d);
    }
  }

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
  double d = sonar.ping() / 2 * 1.0e-4 * SPEED_OF_SOUND;

  if (d <= MIN_DIST)
    return -1;
  else
    return d;
}
void measureMode()
{

  double distance = calculateMean(BUF_SIZE, buffer, &getDistanceCm);
  //measurement error
  if (distance <= MIN_DIST)
  {
    current = ERROR;
    return;
  }

  displayNumber(distance);
  current = SHOW;
  return;
}
