#ifndef clox_line_number_h
#define clox_line_number_h

#include "common.h"

typedef int LineNumber;

typedef struct
{
  int capacity;
  int count;
  LineNumber *lineNumbers;
} LineNumberArray;

#define LAST_ELEMENT_INDEX(pointer) (((LineNumberArray *)pointer)->count - 2)
#define ELEMENT_COUNT(pointer) (((LineNumberArray *)pointer)->count / 2)

void initLineNumberArray(LineNumberArray *array);
void writeLineNumberArray(LineNumberArray *array, LineNumber value);
void freeLineNumberArray(LineNumberArray *array);
void printLineNumber(LineNumber value);
int getLine(LineNumberArray *array, int offset);

#endif