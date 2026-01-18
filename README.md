# Sunneo/libs - C Data Structures and Algorithms Library Collection

A comprehensive collection of data structures and algorithms implemented in C, developed and maintained by Sunneo IceCold.

**License**: BSD 3-Clause License  
**Language**: C  
**Build System**: Make / GCC

📚 **完整中文文件 (Complete Chinese Documentation)**:
- [架構文件.md](./架構文件.md) - Architecture Documentation (Traditional Chinese)
- [使用手冊.md](./使用手冊.md) - User Manual with Examples (Traditional Chinese)

---

## Overview

This repository contains production-ready implementations of various data structures and utilities:

### Data Structures
- **Linear**: List (Doubly Linked), Vector (Dynamic Array), Stack, Queue
- **Trees**: Binary Search Tree, Red-Black Tree, B+ Tree
- **Hash-based**: HashMap, Hashtable, Hash Functions
- **Heap**: Priority Queue implementation

### Algorithms & Utilities
- **Combinatorics**: Permutation Generator, Powerset Generator
- **String Processing**: Dynamic String, String Tokenizer
- **Network**: Socket wrappers (TCP/UDP/Unix Domain)
- **Graphics**: BMP Image Processing
- **Concurrency**: Coroutine/Fiber (ucontext and jmpbuf implementations)
- **File I/O**: File utilities and Config parser
- **Generic Templates**: Macro-based generic data structures

---

## Quick Start

```bash
# Clone the repository
git clone https://github.com/sunneo/libs.git
cd libs

# Build a specific module (e.g., List)
cd List
make

# Run tests
make test

# Use in your project
gcc -I./include your_program.c -L./bin -lList -o your_program
```

---

## Core Modules

### 1. List (Doubly Linked List)
```c
#include "list.h"

List* list = list_create(sizeof(int), compare_func);
list_push_back(list, &value);
list_delete(list);
```

### 2. Vector (Dynamic Array)
```c
#include "vector.h"

Vector* vec = vector_create(sizeof(int));
vector_push_back(vec, &value);
int* item = (int*)vector_at(vec, index);
vector_delete(vec);
```

### 3. HashMap
```c
#include "hashmap.h"

struct HashMap* map = hashmap_new(hash_func, equal_func);
hashmap_put(map, key, value);
void* val = hashmap_get(map, key);
hashmap_delete(map);
```

### 4. RBTree (Red-Black Tree)
```c
#include "rbtree.h"

RBTree* tree = rbtree_create(elesize, keysize, compare_func);
rbtree_insert(tree, key, value);
void* result = rbtree_search(tree, key);
rbtree_delete(tree);
```

### 5. String
```c
#include "cstring.h"

String* str = string_create("Hello");
string_append_cstr(str, " World");
string_delete(str);
```

### 6. BMP (Bitmap Image Processing)
```c
#include "bmp.h"

BMP* bmp = bmp_create(800, 600, 0xFFFFFF);
bmp_line(bmp, 50, 50, 200, 200, 0x0000FF);
bmp_write(bmp, "output.bmp");
bmp_delete(bmp);
```

### 7. Coroutine (Fiber)
```c
#include "coroutine.h"

Fiber* fiber = fiber_create(function, param);
fiber_start(fiber);
fiber_resume(fiber);
fiber_delete(fiber);
```

### 8. Socket (Network Programming)
Provides simplified wrappers for TCP, UDP, and Unix domain sockets.

### 9. Permutation & Powerset
Generate all permutations and subsets of a set.

---

## Documentation

### Chinese Documentation (繁體中文文件)

**[架構文件.md](./架構文件.md)** - Comprehensive architecture documentation including:
- Detailed module descriptions
- API reference for all modules
- Design patterns and best practices
- Performance characteristics

**[使用手冊.md](./使用手冊.md)** - Complete user manual with:
- Installation and setup instructions
- Step-by-step tutorials for each module
- Code examples and use cases
- FAQ and troubleshooting

### Quick Reference

| Data Structure | Insert | Delete | Search | Space |
|---------------|--------|--------|--------|-------|
| List          | O(1)   | O(n)   | O(n)   | O(n)  |
| Vector        | O(1)*  | O(n)   | O(1)   | O(n)  |
| HashMap       | O(1)*  | O(1)*  | O(1)*  | O(n)  |
| RBTree        | O(log n)| O(log n)| O(log n)| O(n) |
| B+ Tree       | O(log n)| O(log n)| O(log n)| O(n) |

*Average case

---

## Building

### Requirements
- GCC 4.x or later
- Make
- Optional: pthread (for threaded versions)

### Build Individual Module

```bash
cd <module_name>
make          # Build library
make test     # Build and run tests
make clean    # Clean build artifacts
```

### Integration

**Static Linking**:
```bash
gcc -I/path/to/libs/Module/include \
    your_code.c \
    -L/path/to/libs/Module/bin \
    -lModule \
    -o your_program
```

---

## Examples

### Example 1: List with Custom Structures

```c
#include "list.h"

typedef struct {
    int id;
    char name[32];
} Student;

int student_compare(const void* a, const void* b) {
    return ((Student*)a)->id == ((Student*)b)->id;
}

int main() {
    List* students = list_create(sizeof(Student), student_compare);
    Student s1 = {1, "Alice"};
    list_push_back(students, &s1);
    list_delete(students);
    return 0;
}
```

### Example 2: HashMap

```c
#include "hashmap.h"

int string_hash(const void* key) {
    const char* str = (const char*)key;
    unsigned int hash = 0;
    while (*str) hash = hash * 31 + (*str++);
    return hash;
}

int string_equal(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b) == 0;
}

int main() {
    struct HashMap* map = hashmap_new(string_hash, string_equal);
    hashmap_put(map, "name", "Alice");
    char* name = (char*)hashmap_get(map, "name");
    hashmap_delete(map);
    return 0;
}
```

More examples in [使用手冊.md](./使用手冊.md).

---

## Thread Safety

⚠️ **Important**: Most modules are **not thread-safe** by default.

For multi-threaded environments, add your own synchronization:

```c
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_lock(&lock);
list_push_back(list, data);
pthread_mutex_unlock(&lock);
```

---

## Memory Management

- All `xxx_create()` must be paired with `xxx_delete()`
- When storing pointers (elesize = 0), manually free the memory
- Use Valgrind to detect leaks: `valgrind --leak-check=full ./program`

---

## License

Copyright 2024, Sunneo IceCold

Licensed under the **BSD 3-Clause License**. See [LICENSE](./LICENSE).

---

## Contact

- **Repository**: [https://github.com/sunneo/libs](https://github.com/sunneo/libs)
- **Issues**: GitHub Issues
- **Pull Requests**: Welcome!

---

**Version**: 1.0  
**Last Updated**: 2024-01-18  
**Maintainer**: Sunneo IceCold
