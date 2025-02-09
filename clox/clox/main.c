//
//  main.c
//  clox
//
//  Created by Karol Harasim on 2/2/25.
//

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include <assert.h>
#include <stdio.h>
#include "vm.h"

#include <stdlib.h>
#include <string.h>

static void repl(void) {
    char line[1024];
    for (;;) {
        printf("> ");
        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }
    }
    interpret(line);
}

static char* readFile(const char* path) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }
    fseek(file, 0l, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);
    char*buffer = (char*)malloc(fileSize + 1);
    if (buffer == NULL) {
      fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
      exit(74);
    }
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize) {
      fprintf(stderr, "Could not read file \"%s\".\n", path);
      exit(74);
    }
    buffer[bytesRead] = '\0';
    fclose(file);
    return buffer;
}

static void runFile(const char* path) {
    char* source = readFile(path);
    InterpretResult result = interpret(source);
    free(source);
    
    if (result == INTERPRET_COMPILE_ERROR) exit(65);
    if (result == INTERPRET_RUNTIME_ERROR) exit(70);
    
}


void test_write_constant(Chunk* chunk) {
    int byteOffset = 0;  // Tracks chunk->code current placement
    for (int i = 0; i < 256; i++) {
        writeConstant(chunk, (Value)(i + 1), i);
        
        assert(chunk->constants.values[i] == (Value)(i + 1));
        assert(chunk->code[byteOffset] == OP_CONSTANT); // Check OP_CONSTANT instruction
        assert(chunk->code[byteOffset + 1] == (i & 0xFF)); // Verify operand (1 byte)
        
        printf("PASS: OP_CONSTANT written correctly for index %d\n", i);
        byteOffset += 2; // OP_CONSTANT takes 2 bytes (opcode + operand)
    }
    
    for (int i = 256; i < 300; i++) {
        double test_value = (i * 100.0);
        writeConstant(chunk, test_value, i);
        
        assert(chunk->constants.values[i] == test_value);
        assert(chunk->code[byteOffset] == OP_CONSTANT_LONG); // OP_CONSTANT_LONG instruction
        assert(chunk->code[byteOffset + 1] == (i & 0xFF));  // LSB
        assert(chunk->code[byteOffset + 2] == ((i >> 8) & 0xFF)); // Middle byte
        assert(chunk->code[byteOffset + 3] == ((i >> 16) & 0xFF)); // MSB

        // Debugging output
        printf("PASS: OP_CONSTANT_LONG written correctly for index %d\n", i);
        printf("  LSB: %02X, MSB: %02X, 3rd byte: %02X\n",
               chunk->code[byteOffset + 1],
               chunk->code[byteOffset + 2],
               chunk->code[byteOffset + 3]);

        byteOffset += 4; // OP_CONSTANT_LONG takes 4 bytes
    }
}

int main(int argc, const char * argv[]) {
    initVM();
    if (argc == 1) {
        repl();
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        fprintf(stderr, "Usage: clox [path]\n");
        exit(64);
    }
    freeVM();
//    Chunk chunk;
//    initChunk(&chunk);
//    int constant = addConstant(&chunk, 1.2);
//    writeChunk(&chunk, OP_CONSTANT, 123);
//    writeChunk(&chunk, constant, 123);
//    
//    constant = addConstant(&chunk, 3.4);
//    writeChunk(&chunk, OP_CONSTANT, 123);
//    writeChunk(&chunk, constant, 123);
//
//    writeChunk(&chunk, OP_ADD, 123);
//
//    constant = addConstant(&chunk, 5.6);
//    writeChunk(&chunk, OP_CONSTANT, 123);
//    writeChunk(&chunk, constant, 123);
//
//    writeChunk(&chunk, OP_DIVIDE, 123);
//    writeChunk(&chunk, OP_NEGATE, 123);
//    
//    writeChunk(&chunk, OP_RETURN, 123);
//    disassembleChunk(&chunk, "test chunk");
//    interpret(&chunk);
//    freeChunk(&chunk);
//    
////    test_write_constant(&chunk);
//    freeVM();
//    freeChunk(&chunk);
    return 0;
}
