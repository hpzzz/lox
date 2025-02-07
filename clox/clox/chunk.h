//
//  chunk.h
//  clox
//
//  Created by Karol Harasim on 2/3/25.
//

#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_CONSTANT_LONG,
    OP_NEGATE,
    OP_RETURN,
} OpCode;

typedef struct {
    // number of elements in array we have allocated and how many of those are in use
    int count;
    int capacity;
    uint8_t* code;
    int* lines;
    ValueArray constants;
} Chunk;

void initChunk(Chunk *chunk);
void freeChunk(Chunk *chunk);
void writeChunk(Chunk *chunk, uint8_t byte, int line);
int addConstant(Chunk *chunk, Value value);
void writeConstant(Chunk* chunk, Value value, int line);

#endif /* clox_chunk_h */
