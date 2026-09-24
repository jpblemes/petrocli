# petrocli

A command-line application, built incrementally with C++17 and CMake.

## Build

### Prerequisites

- A C++17 compiler:
  - Linux: GCC (9 or newer).
  - Windows: MSVC, via Visual Studio 2019/2022 with the "Desktop development
    with C++" workload, or the standalone Visual Studio Build Tools.
- [CMake](https://cmake.org/) 3.20 or newer.
- [NSIS](https://nsis.sourceforge.io/) — Windows only, and only needed to
  build the `.exe` installer via `cpack -G NSIS`. Not required to build, run,
  or test petrocli itself.

GoogleTest is fetched automatically via CMake FetchContent; no manual setup
is needed for it.

Both platforms use CMake's default generator: on Linux that's Unix
Makefiles (single-config, so the build type is chosen with
`-DCMAKE_BUILD_TYPE` at configure time); on Windows it's the installed
Visual Studio version (multi-config, so the build type is chosen with
`--config` at build and test time instead).

### Linux

```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

### Windows

Run from a "Developer Command Prompt for VS" (or after running
`vcvarsall.bat`) so the MSVC compiler is on `PATH`:

```
cmake -S . -B build
cmake --build build --config Debug
```

## Test

```
ctest --test-dir build --output-on-failure           # Linux
ctest --test-dir build --output-on-failure -C Debug   # Windows
```

## Commands

The examples below run against the sample dataset at `data/data.csv`
(20 rows) and show the exact output petrocli produces for it. After
building, the binary is at `./build/src/petrocli` on Linux or
`build\src\Debug\petrocli.exe` on Windows; the examples below just say
`petrocli` for readability. Run `petrocli --help` or
`petrocli <command> --help` for full usage.

### check

Validates a CSV file and reports how many samples it contains.

```
petrocli check --input data/data.csv
```

```
valid: data/data.csv (20 samples)
```

### stats

Prints count, min, max, and mean for each numeric column.

```
petrocli stats --input data/data.csv
```

```
Column             Count      Min      Max     Mean
depth_m               20  1200.00  2150.00  1675.00
api_gravity           20    19.50    37.50    29.06
sulfur_pct            20     0.25     3.10     1.36
density_g_cm3         20     0.83     0.94     0.88
```

### filter

Prints the samples matching a location and depth range.

```
petrocli filter --input data/data.csv --location ALFA --depth-min 1000 --depth-max 1500
```

```
sample_id      depth_m   api_gravity   sulfur_pct   density_g_cm3  location
S001              1200        29.800        1.100           0.878      ALFA
S002              1250        31.200        0.850           0.865      ALFA
S003              1300        27.500        1.600           0.892      ALFA
```

### rank

Prints the top N samples ranked by `api_gravity` (highest first) or
`sulfur_pct` (lowest first), with `sample_id` ascending as the tie
breaker.

```
petrocli rank --input data/data.csv --by api --top 5
```

```
sample_id     api_gravity
S018               37.500
S010               36.800
S011               35.900
S004               34.000
S005               33.100
```

```
petrocli rank --input data/data.csv --by sulfur --top 5
```

```
sample_id      sulfur_pct
S018                0.250
S010                0.300
S011                0.350
S004                0.450
S005                0.550
```

See `CLAUDE.md` for project conventions.
