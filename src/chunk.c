#include <stdio.h>
#include <stdlib.h>
#include "chunk.h"
#include "clox_memory.h"

void initChunk(Chunk *chunk)
{
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;

  initLineNumberArray(&chunk->lines);
  initValueArray(&chunk->constants);
}

void writeChunk(Chunk *chunk, uint8_t byte, int line)
{
  if (chunk->capacity < chunk->count + 1)
  {
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
  }

  writeLineNumberArray(&chunk->lines, line);

  chunk->code[chunk->count] = byte;
  chunk->count++;
}

void freeChunk(Chunk *chunk)
{
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);

  freeValueArray(&chunk->constants);
  freeLineNumberArray(&chunk->lines);

  /** deallocate the memory and re-init the chunk
   * to leave it in a well-defined empty state.
   * So clever! I mean why would you be freeing it
   * explicitly if you don't intend to re-use it?
   * Like, if the context that this chunk is being
   * use in ends... it should all get cleaned up
   * (obviously in C this isn't automatic but if we
   * were using an arena allocator or something then
   * it would... maybe? Anyway, I found this interesting)
   */
  initChunk(chunk);
}

int addConstant(Chunk *chunk, Value value)
{
  writeValueArray(&chunk->constants, value);
  return chunk->constants.count - 1;
}