//
//  debug.c
//  clox
//
//  Created by Karol Harasim on 2/4/25.
//

#include "debug.h"
#include <stdio.h>
#include "value.h"

void disassembleChunk(Chunk* chunk, const char* name) {
    printf("==%s==\n", name);
    
    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
    
}

static int simpleInstruction(const char* name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

static int constantInstruction(const char* name, Chunk* chunk, int offset, uint8_t byteCount) {
    // offset is at first byte of instruction
    int constant = chunk->code[offset + 1]; // index of values array is stored in second byte for this instruction
    if (byteCount == 3) {
        constant |= (chunk->code[offset + 2] << 8);
        constant |= (chunk->code[offset + 2] << 16);
    }
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]); // print value of constant
    printf("'\n");
    return offset + 2; // OP_CONSTANT is 2 bytes, so next instruction is +2
}

int disassembleInstruction(Chunk* chunk, int offset) {
    printf("%04d ", offset);
    // if same line in next instruction, then just print
    if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]) {
        printf("   | ");
    } else {
        printf("%4d ", chunk->lines[offset]);
    }
    
    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset, 1);
        case OP_ADD:
          return simpleInstruction("OP_ADD", offset);
        case OP_SUBTRACT:
          return simpleInstruction("OP_SUBTRACT", offset);
        case OP_MULTIPLY:
          return simpleInstruction("OP_MULTIPLY", offset);
        case OP_DIVIDE:
          return simpleInstruction("OP_DIVIDE", offset);
        case OP_CONSTANT_LONG:
            return constantInstruction("OP_CONSTANT_LONF", chunk, offset, 3);
        case OP_NEGATE:
            return simpleInstruction("OP_NEGATE", offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}


