# petrocli

A command-line application, built incrementally with C++17 and CMake.

## Build

```
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

## Test

```
ctest --test-dir build --output-on-failure
```

See `CLAUDE.md` for project conventions.
