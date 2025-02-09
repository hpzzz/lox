//
//  compiler.h
//  clox
//
//  Created by Karol Harasim on 2/8/25.
//

#ifndef clox_compiler_h
#define clox_compiler_h

#include "vm.h"

bool compile(const char* source, Chunk* chunk);

#endif /* clox_compiler_h */
