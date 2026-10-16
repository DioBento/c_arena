# Memory Arena Allocator in C

A small memory arena allocator written in C, using virtual memory APIs on Windows and Linux.

The project explores low-level memory management, virtual memory reservation and commitment, aligned allocations, and cross-platform system programming.

## Features

- Reserve a contiguous virtual address range for an arena.
- Commit memory as allocations require it.
- Allocate memory with configurable zero-initialization.
- Support aligned allocations, bulk reset, and stack-like position restoration.
- Use platform-specific virtual memory APIs on Windows and Linux.
- Include a small test suite.

## How It Works

The allocator maintains a position within a reserved block of virtual address space. Each allocation advances this position, with alignment applied before the allocation.

The arena distinguishes between two sizes:

- **Reserved memory:** The total address range available to the arena.
- **Committed memory:** The portion made accessible for reading and writing.

When an allocation exceeds the currently committed region, the allocator attempts to commit additional memory. If the allocation exceeds the reservation or the operating system operation fails, the allocation returns `NULL`.

The allocator also supports resetting the arena or restoring a previously saved position, allowing groups of allocations to be discarded together without individually freeing them.

## API

| Function | Description |
|---|---|
| `arena_create()` | Reserves address space and commits the initial region. |
| `arena_destroy()` | Releases the arena's reserved address space. |
| `arena_push()` | Allocates memory from the arena. |
| `arena_pop()` | Moves the allocation position backward. |
| `arena_pop_to()` | Restores a previous allocation position. |
| `arena_clear()` | Resets the arena for reuse. |

Convenience macros are provided for allocating individual objects and arrays, with or without zero-initialization.

## Build

### Requirements

- GCC
- GNU Make
- Windows or Linux

### Compile

```sh
make
```

This builds the demo executable.

### Run

```sh
./demo
```

The demo performs repeated allocations of 16 MiB. Press `q` to exit.

### Run tests

```sh
make test
```

### Clean generated files

```sh
make clean
```

## Project Structure

```text
.
├── arena.c
├── arena.h
├── types.h
├── demo.c
├── tests/
│   └── arena_tests.c
├── Makefile
└── README.md
```

## Design Decisions

- **Bulk allocation:** Individual allocations do not need separate deallocation. The arena can be reset or discarded as a whole.
- **Virtual memory:** Reserving address space separately from committing it allows the arena to grow its accessible region as needed.
- **Explicit allocation failure:** Allocation functions return `NULL` when an allocation cannot be fulfilled.
- **Platform abstraction:** Virtual memory operations are isolated behind a small set of platform-specific functions.

## Limitations

- Allocations are aligned to pointer-size boundaries; arbitrary per-allocation alignment is not currently supported.
- Individual allocations cannot be freed independently.
- The arena is not thread-safe.
- The implementation is a learning project and has not been extensively benchmarked or validated for production use.

## Future Improvements

- Expand tests for alignment, capacity limits, allocation failures, and position restoration.
- Add automated builds and tests for Windows and Linux.
- Benchmark against relevant allocation strategies.
- Explore support for custom alignment and temporary arena scopes.

## Motivation

This project was built to deepen my understanding of C, virtual memory, memory allocation strategies, and the interfaces between application code and operating systems.
