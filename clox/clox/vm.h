//
//  vm.h
//  clox
//
//  Created by Karol Harasim on 2/6/25.
//

#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "table.h"

#define STACK_MAX 256

typedef struct {
    Chunk* chunk;
    uint8_t* ip;
    Value stack[STACK_MAX];
    Value* stackTop; //stackTop points to where the next value to be pushed will go.
    Table strings;
    Table globals;
    Obj* objects;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

extern VM vm;

void initVM(void);
void freeVM(void);
InterpretResult interpret(const char* source);
void push(Value value);
Value pop(void);

#endif /* vm_h */
