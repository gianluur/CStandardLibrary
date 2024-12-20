#include "types.h"

char* intToString(int64 number, const uint8 base);
char* decimalToString(double number, int8 precision);
int stringToInt(const char* str, const int length);
double stringToDouble(const char* str, const int length);