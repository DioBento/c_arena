#include "../arena.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"

#define TEST(name) static void name(void)
#define RUN_TEST(name) do {          \
    printf("Running " #name "... "); \
    name();                          \
    puts(GREEN "OK" RESET);          \
} while (0)

TEST(test_create_destroy)
{
    mem_arena* arena = arena_create(64 * 1024, 4096);

    assert(arena != NULL);
    assert(arena->pos == ARENA_BASE_POS);
    assert(arena->commit_pos >= ARENA_BASE_POS);
    assert(arena->commit_pos <= arena->reserve_size);

    arena_destroy(arena);
}

TEST(test_push)
{
    mem_arena* arena = arena_create(64 * 1024, 4096);
    assert(arena != NULL);

    u32* value = PUSH_STRUCT(arena, u32);
    assert(value != NULL);

    *value = 12345;
    assert(*value == 12345);

    arena_destroy(arena);
}

TEST(test_zero_initialized_memory)
{
    mem_arena* arena = arena_create(64 * 1024, 4096);
    assert(arena != NULL);

    u8* memory = PUSH_ARRAY(arena, u8, 128);
    assert(memory != NULL);

    for (u64 i = 0; i < 128; ++i) {
        assert(memory[i] == 0);
    }

    arena_destroy(arena);
}

TEST(test_clear_and_reuse)
{
    mem_arena* arena = arena_create(64 * 1024, 4096);
    assert(arena != NULL);

    void* first = arena_push(arena, 128, false);
    assert(first != NULL);

    arena_clear(arena);
    assert(arena->pos == ARENA_BASE_POS);

    void* second = arena_push(arena, 128, false);
    assert(second != NULL);

    // Reuse is allowed; the address may be the same
    assert(arena->pos > ARENA_BASE_POS);

    arena_destroy(arena);
}

TEST(test_capacity_limit)
{
    mem_arena* arena = arena_create(4096, 4096);
    assert(arena != NULL);

    // Consume the remaining reservation.
    u64 remaining = arena->reserve_size - arena->pos;
    void* memory = arena_push(arena, remaining, false);
    assert(memory != NULL);

    // Another nonzero allocation must fail.
    assert(arena_push(arena, 1, false) == NULL);

    arena_destroy(arena);
}

TEST(test_pop_to)
{
    mem_arena* arena = arena_create(64 * 1024, 4096);
    assert(arena != NULL);

    (void)arena_push(arena, 100, false);
    u64 saved_pos = arena->pos;

    (void)arena_push(arena, 200, false);
    assert(arena->pos > saved_pos);

    arena_pop_to(arena, saved_pos);
    assert(arena->pos == saved_pos);

    arena_destroy(arena);
}

int main(void)
{
    RUN_TEST(test_create_destroy);
    RUN_TEST(test_push);
    RUN_TEST(test_zero_initialized_memory);
    RUN_TEST(test_clear_and_reuse);
    RUN_TEST(test_capacity_limit);
    RUN_TEST(test_pop_to);

    puts(GREEN "All tests passed." RESET);
    return 0;
}
