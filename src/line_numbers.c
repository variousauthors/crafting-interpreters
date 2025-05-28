#include "line_numbers.h"
#include <stdio.h>

#include "clox_memory.h"
#include "value.h"

void dump(LineNumberArray *array, char *context)
{
  fprintf(stderr, "%s: ", context);
  for (int i = 0; i < array->count; i++)
  {
    fprintf(stderr, "%d, ", array->lineNumbers[i]);
  }
  fprintf(stderr, "\n");
}

void initLineNumberArray(LineNumberArray *array)
{
  array->lineNumbers = NULL;
  array->capacity = 0;
  array->count = 0;
}

/** we store a pair [line number, count] at each position */
void writeLineNumberArray(LineNumberArray *array, LineNumber value)
{
  int current = LAST_ELEMENT_INDEX(array);

  // if this is the same number again
  if (array->count > 0 && value == array->lineNumbers[current])
  {
    // increment the count
    array->lineNumbers[current + 1]++;

    return;
  }

  // otherwise add another pair
  if (array->capacity < array->count + 2)
  {
    int oldCapacity = array->capacity;
    array->capacity = GROW_CAPACITY(oldCapacity);
    array->lineNumbers = GROW_ARRAY(LineNumber, array->lineNumbers,
                                    oldCapacity, array->capacity);
  }

  array->lineNumbers[array->count] = value;
  array->lineNumbers[array->count + 1] = 1;
  array->count += 2;
}

void freeLineNumberArray(LineNumberArray *array)
{
  FREE_ARRAY(Value, array->lineNumbers, array->capacity);
  initLineNumberArray(array);
}

void printLineNumber(LineNumber value)
{
  printf("%d", value);
}

/** returns the nth line number
 * -1 indicates failure or out of bounds
 */
int getLine(LineNumberArray *array, int index)
{
  if (array->count == 0) {
    return -1;
  }

  // walk the RLC
  int position = 0;
  int i = 0;
  int length = array->lineNumbers[i + 1];
  int result = array->lineNumbers[i];

  while (position < index)
  {
    position++;

    if (position > length - 1) {
      i += 2;
      length += array->lineNumbers[i + 1];
      result = array->lineNumbers[i];
    }
  }

  return result;
}