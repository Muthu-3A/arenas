#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define PushArray(arena, type, count) (type *)ArenaPush((arena), sizeof(type)*(count))
#define PushArrayZero(arena, type, count) (type *)ArenaPushZero((arena), sizeof(type)*(count))
#define PushStruct(arena, type) PushArray((arena), (type), 1)
#define PushStructZero(arena, type) PushArrayZero((arena), (type), 1)

#define ArenaDefaultCapacity (1024 * 1024) //1 megabyte


typedef struct{
    unsigned char *base;
    size_t capacity;
    size_t offset;
} Arena;

Arena *ArenaAlloc(void);
void ArenaRelease(Arena *arena);

void *ArenaPush(Arena *arena, size_t size);
void *ArenaPushZero(Arena *arena, size_t size);

void ArenaPop(Arena *arena, size_t size);
size_t ArenaGetPos(Arena *arena);

void ArenaGetPosBack(Arena *arena, size_t pos);
void ArenaClear(Arena *arena);

Arena *ArenaAlloc(void) {
    Arena *arena = malloc(sizeof *arena);
    if(!arena) {
        return NULL; //allocation failed
    }

    arena->base = malloc(ArenaDefaultCapacity);
    if(!arena->base) {
        free(arena);
        return NULL; //allocation failed
    }

    arena->capacity = ArenaDefaultCapacity
    arena->offset = 0;

    return arena
}

void ArenaRelease(Arena *arena) {
    free(arena->base);
}

void *ArenaPush(Arena *arena, size_t size) {
    if(!arena || size == 0) {
        return NULL; //invalid function call
    }

    if(arena->capacity - arena->offset < size) {
        return NULL; //cannot allocate required memory
    }

    void *ptr = arena->base + arena->offset;
    arena->offset += size;
    return ptr
}

void *ArenaPushZero(Arena *arena, size_t size) {
    if(!arena || size == 0) {
        return NULL;
    }

    if(arena->capacity - arena->offset < size) {
        return NULL;
    }

    void *ptr = arena->base + arena->offset;
    arena->offset += size;

    memset(ptr, 0, size); //zero the allocated memory
    return ptr
}

void ArenaPop(Arena *arena, size_t size) {
    if(!arena || size == 0) {
        return NULL;
    }

    if(size > arena->offset) {
        //could set offset to 0 but want concious ArenaClear to be used for that
        return NULL;  // so in this implementation we catch arena accounting mistakes
    }
    
    arena->offset -= size;  
}
size_t ArenaGetPos(Arena *arena) {
    if(!arena) return;
    return arena->offset;
}

void ArenaGetPosBack(Arena *arena, size_t pos) {
    if(!arena || pos > arena->offset) {
        return; //invalid
    }

    arena->offset = pos;
}

void ArenaClear(Arena *arena) {
    if(!arena) return;
    arena->offset = 0;
}

int main() {
    return 0;
}