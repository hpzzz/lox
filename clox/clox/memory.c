//
//  memory.c
//  clox
//
//  Created by Karol Harasim on 2/3/25.
//

#include <stdlib.h>

#include "memory.h"
#include "vm.h"
#include "object.h"

void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
    if (newSize == 0) {
        free(pointer);
        return NULL;
    }
    
    void* result = realloc(pointer, newSize);
    if (result == NULL) exit(1);
    return result;
}

void freeObject(Obj* object) {
    switch (object->type) {
        case OBJ_STRING: {
            ObjString* string = (ObjString*)object;
            FREE_ARRAY(char, string->chars, string->length+1);
            FREE(ObjString, object);
            break;
        }
    }
}

void freeObjects(void) {
    Obj* object = vm.objects;
    while (object != NULL) {
        Obj* next = object->next;
        freeObject(object);
        object = next;
    }
}
