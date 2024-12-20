#include <stdarg.h>
#include <stdio.h>
#include "unistd.h"
#undef NULL

#include "../context/os.h"
#include "memory.h"
#include "converters.h"

#ifdef OS_LINUX

void print(const char* format, ...){
  va_list args;
  va_start(args, format);
  
  int temp;
  char* buffer;

  for(const char* traverse = format; *traverse != '\0'; traverse++){
    while (*traverse != '%' && *traverse != '\0'){
      write(STDOUT_FILENO, traverse++, 1);
    }
    
    if (*traverse == '\0')
      break;
      
    switch(*++traverse){
      case 'c':
        temp = va_arg(args, int);
        write(STDOUT_FILENO, (char*)&temp, 1);
        break;
        
      case 's':
        for(buffer = va_arg(args, char*); *buffer != '\0'; buffer++)
          write(STDOUT_FILENO, buffer, 1);
        break;
        
      case 'i':
        for(buffer = intToString(va_arg(args, int), 10); *buffer != '\0'; buffer++)
          write(STDOUT_FILENO, buffer, 1);
        break;
        
      case 'f':
        for(buffer = decimalToString(va_arg(args, double), 7); *buffer != '\0'; buffer++)
          write(STDOUT_FILENO, buffer, 1);
        break;
      
      case 'd':
        for(buffer = decimalToString(va_arg(args, double), 16); *buffer != '\0'; buffer++)
          write(STDOUT_FILENO, buffer, 1);
        break;
        
      default:
        break;
    }
  }
  va_end(args);
}

int input(char* format, ...){
  va_list args;
  va_start(args, format);
  
  int* int_temp; double* double_temp; char buffer[1024];
  char* traverse; int nItem = 0, nBytesReaded = 0;
  for(traverse = format; *traverse != '\0'; traverse++){
    while (*traverse != '%' && *traverse != '\0')
      write(STDOUT_FILENO, traverse++, 1);
    
    switch (*++traverse){
      case 'i':
        nBytesReaded = read(STDIN_FILENO, buffer, sizeof(buffer)) - 1;
        if (nBytesReaded < 0)
          continue;
        nItem++;
        buffer[nBytesReaded] = '\0';
      
        int_temp = va_arg(args, int*);
        *int_temp = stringToInt(buffer, nBytesReaded);
        break;
        
      case 'c':
        int_temp = va_arg(args, int*);
        read(STDIN_FILENO, int_temp, 1);
        nItem++;
        break;
        
      case 'f':
        nBytesReaded = read(STDIN_FILENO, buffer, sizeof(buffer)) - 1;
        if (nBytesReaded < 0)
          continue;
        nItem++;
        buffer[nBytesReaded] = '\0';
        
        double_temp = va_arg(args, double*);
        *double_temp = stringToDouble(buffer, nBytesReaded);
        
      default:
        break;
    }
  }
  va_end(args);
  return nItem;
}


#endif

int main() {
  print("char: %c\n", 'c');
  print("string: %s\n", "str");
  print("int: %i ", -123);
  print("zero: %i ", 0);
  print("int: %i\n", 123);
  print("float: %d", 12345678.3);

  double temp = 5.3;
  input("test: %f", &temp);
  printf("end: %f", temp);
}