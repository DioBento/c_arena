#include <stdio.h>
#include <stdbool.h>

#include "types.h"
#include "arena.h"

#define KiB(n) ((u64)(n) << 10)
#define MiB(n) ((u64)(n) << 20)
#define GiB(n) ((u64)(n) << 30)

i32 main()
{
    mem_arena* perm_arena = arena_create(GiB(1), MiB(1));

    if (perm_arena == NULL) {
        fputs("Failed to create arena.\n", stderr);
        return 1;
    }

    while (1) {
        if (arena_push(perm_arena, MiB(16), false) == NULL) {
            puts("Arena is full.");
            break;
        }

        int ch = getc(stdin);
        if (ch == 'q' || ch == EOF) { break; }
    }

    arena_destroy(perm_arena);

    return 0;
}
