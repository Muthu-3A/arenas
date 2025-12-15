#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    if(!arena) { //sequential dependencies
        return NULL; //allocation failed
    }

    arena->base = malloc(ArenaDefaultCapacity);
    if(!arena->base) {
        free(arena);
        return NULL; //allocation failed
    }

    arena->capacity = ArenaDefaultCapacity;
    arena->offset = 0;

    return arena;
}

void ArenaRelease(Arena *arena) {
    free(arena->base);
}

void *ArenaPush(Arena *arena, size_t size) {
    if(arena && size != 0 && arena->capacity - arena->offset >= size) {
        void *ptr = arena->base + arena->offset;
        arena->offset += size;
        return ptr;
    }
}

void *ArenaPushZero(Arena *arena, size_t size) {
    if(arena && size != 0 && arena->capacity - arena->offset >= size) {
        void *ptr = arena->base + arena->offset;
        arena->offset += size;
        memset(ptr, 0, size); //zero the allocated memory
        return ptr;
    }
}

void ArenaPop(Arena *arena, size_t size) {
    if (arena && size && arena->offset >= size) {
        arena->offset -= size;
    }
}
size_t ArenaGetPos(Arena *arena) {
    if(arena) {
        return arena->offset;
    }
    return SIZE_MAX;
}

void ArenaGetPosBack(Arena *arena, size_t pos) {
    if(arena && pos == !SIZE_MAX && pos <= arena->offset) {
        arena->offset = pos;
    }
}

void ArenaClear(Arena *arena) {
    if(arena) {
        arena->offset = 0;
    }
}

int main() {
    Arena *listy = ArenaAlloc();
    if(!listy) return 1;

    int *ptr = ArenaPush(listy, sizeof(int[10]));
    if(!ptr) return 1;

    *ptr = 3;
    *(ptr + 1) = 4;
    
    printf("%d\n", *ptr);
    printf("%d\n", *(ptr+1));

    ArenaClear(listy);

    return 0;
}