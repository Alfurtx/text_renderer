#ifndef TEXTERER_H
#define TEXTERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define internal static

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef unsigned int uint;
typedef size_t usize;

// NOTE(fonsi): custom assert implementation
#define NDEBUG
#include <assert.h>
#undef assert
#define assert(E, M) (void)((E) || (_assert(#E, __FILE__, __LINE__, M),0))
inline void
_assert(const char* expr, const char* filename, const int line, char* message)
{
    if(!message) {
        fprintf(stderr, "[ERROR][%s:%d] %s\n", filename, line, expr);
    } else {
        fprintf(stderr, "[ERROR][%s:%d] %s\n", filename, line, message);
    }
    exit(EXIT_FAILURE);
}

#endif
