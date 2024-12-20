#include "converters.h"
#include "types.h"
#include "math.h"

char* intToString(int64 number, const uint8 base){
  static char rappresentation[] = "0123456789ABCDEF";
  static char buffer[64];
  char* ptr = &buffer[63];
  *ptr = '\0';
  
  bool isNegative = false;
  if (number < 0){
    isNegative = true;
    number = (number == MININT32) ? MININT32 : -number;
  }
  
  do {
    *--ptr = rappresentation[number % base];
    number /= base;
  } while (number != 0);
  
  if (isNegative)
    *--ptr = '-';
  
  return ptr;
}

char* decimalToString(double number, int8 precision) {
  static char buffer[128];
  char* ptr = buffer;

  if (number < 0) {
    *ptr++ = '-';
    number = -number;
  }

  int intPart = (int)number;
  char* intStr = intToString(intPart, 10);
  
  while (*intStr) {
    *ptr++ = *intStr++;
  }

  double fracPart = number - intPart;
  if (precision > 0) {
    *ptr++ = '.';
    
    for (int i = 0; i < precision; ++i) {
      fracPart *= 10;
      int fracInt = (int)fracPart;
      *ptr++ = '0' + fracInt;
      fracPart -= fracInt;
    }
  }

  *ptr = '\0';
  return buffer;
}

int stringToInt(const char* str, const int length){
  int number = 0;
  
  for(int i = 0; i < length; i++)
    number += (str[i] - '0') * (int)pow(10, length - i - 1);
  return number;
}

double stringToDouble(const char* str, const int length){
  int isDecimal = 0;
  
  int intpart = 0; double decimalpart = 0;
  int icounter = 0, dcounter = 0;
  for(int i = 0; i < length; i++){
    if (str[i] == '.'){
      isDecimal = 1;
      continue;
    }
  
    if (!isDecimal) {
      intpart = intpart * 10 + (str[i] - '0');
      icounter++;
    }
      
    else { 
      decimalpart += (str[i] - '0') * pow(10, -(dcounter));
      dcounter++;
    }
  }
  return intpart + decimalpart / 10;
}