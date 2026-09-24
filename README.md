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

## Package

Requires NSIS on Windows (see Prerequisites above); on Linux this
produces a `.deb` instead and needs no extra tooling. Run from the
`build` directory configured above:

```
cd build
cpack
```

or, equivalently, without leaving the top-level directory:

```
cmake --build build --target package                  # Linux
cmake --build build --config Debug --target package    # Windows
```

Either way, CMake picks the right generator for the current platform
automatically (DEB on Linux, NSIS on Windows), and the package is
written into `build/`. On Linux this is `petrocli-0.1.0-Linux.deb`.

### Install

- Linux: `sudo dpkg -i build/petrocli-0.1.0-Linux.deb` installs the
  `petrocli` binary to `/usr/bin`.
- Windows: run the generated `.exe` and follow the installer; it
  installs to `%ProgramFiles%\petrocli` by default.

### Uninstall

- Linux: `sudo apt remove petrocli` (or `sudo dpkg -r petrocli`).
- Windows: use Settings → Apps → petrocli → Uninstall, or run
  `Uninstall.exe` directly from the install directory
  (`%ProgramFiles%\petrocli\Uninstall.exe`) — NSIS generates this
  uninstaller automatically.

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

## Design Decisions

- **Separation of concerns**:
  - `cli.h`/`cli.cpp` only parse arguments and dispatch to a command.
  - `csv.h`/`csv.cpp` only read and parse CSV rows.
  - `model.h`'s `Sample` is a plain data struct.
  - Each command's actual behavior lives in its own class in
    `commands.h`/`commands.cpp`, built on small, CLI-independent
    functions (`validation.h`, `stats.h`, `filter.h`, `rank.h`) that
    are each testable on their own.
- **Command pattern**: every subcommand is a class implementing the
  `Command` interface (`command.h`), constructed by a small
  name-to-factory registry in `cli.cpp`. Adding a subcommand means
  writing one class and registering it, not growing a switch statement.
- **No external runtime libraries**: only the C++ standard library is
  used at runtime. GoogleTest is a build/test-time dependency only
  (fetched via CMake FetchContent) and is never linked into the shipped
  `petrocli` binary.
- **C++17** is the project's language standard throughout (`std::optional`,
  `std::filesystem`, structured bindings, and similar).

See `CLAUDE.md` for project conventions.
