#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define PushArray(arena, type, count) (type *)ArenaPush((arena), sizeof(type)*(count))
#define PushArrayZero(arena, type, count) (type *)ArenaPushZero((arena), sizeof(type)*(count))
#define PushStruct(arena, type) PushArray((arena), (type), 1)
#define PushStructZero(arena, type) PushArrayZero((arena), (type), 1)


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
    //todo
}

void ArenaRelease(Arena *arena) {
    free(arena->base);
}

void ArenaClear(Arena *arena) {
    arena->offset = 0;
}

int main() {
    return 0;
}