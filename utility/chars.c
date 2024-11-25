#include "chars.h"

bool isLowerCase(const char character){
  return (character >= 97 && character <= 122); // a b c d e f g h i l m n o p q r s t u v x y z
}

bool isUpperCase(const char character){
  return (character >= 65 && character <= 90); // A B C D E F G H I L M N O P Q R S T U V X Y Z
}

bool isDigit(const char character){
  return (character >= 48 && character <= 57); // 0 1 2 3 4 5 6 7 8 9
}

bool isPunctuation(const char character){
  return (character >= 33 && character <= 46) || // ! " # $ % & ' ( ) * + , - . /
         (character >= 58 && character <= 63) || // : ; < = > ? @   
         (character >= 91 && character <= 96) || // [ \ ] ^ _ `
         (character >= 123 && character <= 126); // { | } ~
}

bool isSpace(const char character){
  return character == 32 || (character == 9 && character == 13);
}

bool isLetter(const char character){
  return isLowerCase(character) || isUpperCase(character);
}

bool isLetterOrDigit(const char character){
  return isLetter(character) || isDigit(character);
}

bool isNewLine(const char character){
  return character == '\n';
}

bool isPrintable(const char character){
  return character >= 32 && character <= 127;
}