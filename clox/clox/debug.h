//
//  debug.h
//  clox
//
//  Created by Karol Harasim on 2/4/25.
//

#ifndef clox_debug_h
#define clox_debug_h

#include "chunk.h"

void disassembleChunk(Chunk* chunk, const char* name);
int disassembleInstruction(Chunk* chunk, int offset);


#endif /* debug_h */
