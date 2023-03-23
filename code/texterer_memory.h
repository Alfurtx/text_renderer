#ifndef TEXTERER_MEMORY_H
#define TEXTERER_MEMORY_H

#include "texterer.h"

typedef struct Arena {
    usize used;
    usize total;
    u8* base;
} Arena;

typedef struct ArenaLocal {
    Arena* arena;
    usize pos;
} ArenaLocal;

Arena      ArenaCreate(usize size);
void       ArenaRelease(Arena* arena);
void*      ArenaPush(Arena* arena, usize size);
ArenaLocal ArenaGetScratch(Arena* arena);
void       ArenaReleaseScratch(ArenaLocal* local);

#ifdef ARENA_IMPLEMENTATION

Arena
ArenaCreate(usize size)
{
    return (Arena) {
        .used = 0,
        .total = size,
        .base = malloc(size), // NOTE(fonsi): consider using VirtualAlloc instead of malloc
    };
}

void
ArenaRelease(Arena* arena)
{
    assert(arena, "Arena is NULL");
    free(arena->base);
}

void*
ArenaPush(Arena* arena, usize size)
{
    if(arena->used + size > arena->total) {
        assert(0, "[TODO] Implement growable arenas");
    }
    void* result = arena->base + arena->used;
    arena->used += size;
    return result;
}

ArenaLocal
ArenaGetScratch(Arena* arena)
{
    return (ArenaLocal) {
        .arena = arena,
        .pos = arena->used,
    };
}

void
ArenaReleaseScratch(ArenaLocal* local)
{
    local->arena->used = local->pos;
}

#endif // ARENA_IMPLEMENTATION

#endif // TEXTERER_MEMORY_H
