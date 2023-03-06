# C vector
A very simple header-only implementation of a dynamic array (vector) in C with all the required features for a dynamic array.

# Features
- Simple to use.
- Portable as it's header-only and written in C89.
- Small in size.
- Very simple and basic source code, no overcomplications.

# Usage
To use it you must define `VECTOR_IMPLEMENTATION` once before including `vector.h`.
```c
#define VECTOR_IMPLEMENTATION
#include <vector.h>
```

Explanations on what each function does are in [vector.h](vector.h). There's also a provided example [here](example.c), utilizing all of the features of the library.

# Credits
Original implementation is by [this article](https://aticleworld.com/implement-vector-in-c/)</br>
Majority of the source code credit goes to that article. I only added a few new features and improvements to the code, and turned it into a small and simple portable header file.
