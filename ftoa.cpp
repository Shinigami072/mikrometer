#include "Arduino.h" 
#include"ftoa.h"

char *ftoa(char *buffer, double d, int precision) {

  long wholePart = (long) d;

  // Deposit the whole part of the number.

  itoa(wholePart,buffer,10);

  // Now work on the faction if we need one.

  if (precision > 0) {

    // We do, so locate the end of the string and insert
    // a decimal point.

    char *endOfString = buffer;
    while (*endOfString != '\0') endOfString++;
    *endOfString++ = '.';

    // Now work on the fraction, be sure to turn any negative
    // values positive.

    if (d < 0) {
      d *= -1;
      wholePart *= -1;
    }
    
    double fraction = d - wholePart;
    while (precision > 0) {

      // Multipleby ten and pull out the digit.

      fraction *= 10;
      wholePart = (long) fraction;
      *endOfString++ = '0' + wholePart;

      // Update the fraction and move on to the
      // next digit.

      fraction -= wholePart;
      precision--;
    }

    // Terminate the string.

    *endOfString = '\0';
  }

   return buffer;
}
char* pad(char* input,char*output,int length,char pad_char){
  //find the length of input
  char *endOfString = input;
  while (*endOfString != '\0') endOfString++;
  
  //find how many chars to add
  uint8_t d = length-(endOfString-input);
  for(int i=0;i<length;i++)
  {
    //add characters to output
    if(i<d){
      output[i]= pad_char;
    }else
    {
      output[i]=input[i-d];
    }
    
  }
  return output;
}


char* ftoa8_1(char* buffer,double d){
  long wholePart = (long) round(d*10.0);

  // Deposit the whole part of the number.

  itoa(wholePart,buffer,10);


  if(wholePart!=0){
    char *endOfString = buffer;
    //find length of output
    while (*endOfString != '\0') endOfString++;
    endOfString--;
    
    char c = *(endOfString);//save last digit
    endOfString[0]='.';//replace it with .
    endOfString[1]=c;//put that digit after it
    endOfString[2]='\0';//put null at the end
  }

  return buffer;
}