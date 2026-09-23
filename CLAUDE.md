# CLAUDE.md

This file gives Claude Code project-specific context and rules. Keep it updated as the project evolves.

## Project Overview

- What it is: petrocli, implemented incrementally, starting from an empty repository.
- Stack: C++17, CMake. Prefer the standard library; do not introduce third-party dependencies unless absolutely necessary (and confirm before doing so). Approved exception: GoogleTest, added via CMake FetchContent, for testing.
- Entry point: src/main.cpp

## Project Structure

```
petrocli/
  CMakeLists.txt
  src/                     # application source
    CMakeLists.txt
    main.cpp               # entry point
    cli.cpp                # CLI argument parser
    csv.cpp                # CSV handling
    commands.cpp           # application commands
    validation.cpp         # sample validation
  include/petrocli/        # public headers, mirrors src/
    cli.h
    csv.h
    commands.h
    model.h                # data model
    validation.h
  data/                    # sample/reference data
    data.csv
  tools/                   # auxiliary scripts
    reference.py
  tests/                   # GoogleTest suite
    CMakeLists.txt
    cli_test.cpp
    commands_test.cpp
    csv_test.cpp
    validation_test.cpp
    resources/             # fixture files for tests
      petro_samples_linux.csv
      petro_samples_windows.csv
  README.md
  AI_LOG.md                # maintained manually via /export
```

- Keep the structure simple and close to this specification exactly. Do not add extra directories, files, namespaces, or abstractions beyond what's listed.
- Do not add build directories, .gitignore scaffolding, CI config, or tooling beyond what's listed unless separately requested.

## Project-Specific Instructions (petrocli)

- Keep the architecture simple and appropriate for a mid-level C++ project. Do not overengineer.
- Implement only what is requested in each step; do not add functionality, abstractions, or scaffolding beyond the current request.
- Prioritize: clear code, const correctness, small testable functions, separation of responsibilities, deterministic output, useful errors on stderr, and non-zero exit codes on failure.
- Build incrementally: each step should leave the repository in a working, buildable state.

## Writing Style

- Avoid overusing the parenthetical em dash (—) in prose, comments, and documentation, in any language. Prefer a comma, colon, or rephrasing the sentence instead, reserving the em dash for dialogue or cases where it's genuinely the most natural punctuation.

Examples:

- Avoid: "The result — which no one expected — changed the whole project."
  Prefer: "The result, which no one expected, changed the whole project."
- Avoid: "The function failed — likely due to a null pointer — and the program crashed."
  Prefer: "The function failed, likely due to a null pointer, and the program crashed."
- Avoid: "There's only one option left — rewrite the parser."
  Prefer: "There's only one option left: rewrite the parser."
- Avoid: "The build passed — but the tests still failed."
  Prefer: "The build passed, but the tests still failed."

## Comments

- Default to no comment. Add one only when the code can't say it: intent,
  a workaround, a non-obvious tradeoff.
- Keep comments to one line. Use multiple lines only when one truly can't
  carry it.
- Write fragments, not prose. `// retry: API drops first request after idle`,
  not `// We need to retry here because the API sometimes...`.
- Public functions and types get a one-line `/** */` summary. Add `@param`,
  `@return`, or error notes only when they aren't obvious from names and types.
- No change-history comments ("fixed", "updated", "now uses X"). Use the
  commit message.
- Don't touch comments unrelated to the current change.
- When changing code, update or remove comments that no longer match it.
- No commented-out code.
- TODOs: `// TODO: <what and why>`, one line.
- English, ASCII-only.

## Code Style

- Naming (Google C++ Style Guide): `PascalCase` for types/classes and functions (e.g. `ParseCsvLine`); `snake_case` for local variables and parameters; member variables get a trailing underscore (`count_`); constants use `kPascalCase` (e.g. `kMaxRetries`); macros use `UPPER_SNAKE_CASE`.
- Function size: keep functions small and single-purpose; a function doing more than one thing should be split.
- Comments explain *why*, never *what*. Do not restate what the code already says; only comment on non-obvious intent, constraints, or tradeoffs.
- Const correctness: mark variables, parameters, and member functions `const` wherever possible.
- Every header uses `#pragma once` as its include guard.
- Include order: the file's own header first, then C++ standard library headers, then third-party headers, then other project headers, each group alphabetized and separated by a blank line.
- Error handling: fail loud. Report errors on `stderr` with a clear message and return a non-zero exit code on failure; never fail silently.
- No dead code: no commented-out code, unused functions, or unreachable branches. Delete what isn't used.

## C++ / Build Conventions

- Cross-platform: the project must build and behave identically on Windows (MSVC) and Linux (GCC). Avoid compiler-specific extensions and platform-specific APIs.
- Build system: CMake + Ninja. Out-of-source builds only; never write build artifacts into the source tree.
- Testing: GoogleTest, added via CMake FetchContent, is the test framework. Write tests as functionality is added.
- Portability: use `std::filesystem` for all path handling, never manual string concatenation of paths.
- Line endings: code must handle both LF and CRLF; normalize line endings via `.gitattributes` so the repository stays consistent across platforms.
- Output: program output is ASCII-only.
- `tools/reference.py` must stay portable across platforms: use `pathlib` for all path handling, not raw string paths.

## Commit Format

Follow Linux kernel commit conventions:

- Subject line: `subsystem: summary` in imperative mood (e.g. "fix", not "fixed" or "fixes").
- Body: explain *why* the change was made, not what the diff already shows.
- Wrap both subject and body at 72 characters.
- End with a `Signed-off-by:` trailer using `git config user.name`/`user.email` (e.g. via `git commit -s`), never a hardcoded name or email.
- No AI attribution trailers or co-author lines unless explicitly requested.

Examples:

```
csv: fix off-by-one when parsing trailing empty field

The parser dropped the last field when a line ended with a
delimiter, because the loop exited before appending the final
empty token. This caused row counts to silently mismatch the
header on CSVs with trailing commas.

Signed-off-by: Full Name <email@example.com>
```

```
cli: add --output flag to control destination format

Users need to redirect results to a file for downstream
processing instead of only printing to stdout. This keeps the
default behavior unchanged while enabling scripted use.

Signed-off-by: Full Name <email@example.com>
```

## Pull Requests

- Open a PR for every change; never commit directly to `main`.
- Keep PRs small and scoped to one step or feature, matching the incremental build philosophy.
- A PR must leave the repository in a working, buildable state on both Windows and Linux.
- The PR description explains *why* the change was made, not just what changed.

## Things Not To Do

- Never push to `main`.
- Never touch CI configuration, secrets, or `.env` files without asking first.
- Never rewrite shared history (no force-push, no rebasing branches others may have pulled).

## Useful Commands

Configure (Ninja generator):
```
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
```

Build:
```
cmake --build build
```

Run:
```
./build/src/petrocli        # Linux
build\src\petrocli.exe      # Windows
```

Test:
```
ctest --test-dir build --output-on-failure
```

Format:
```
clang-format -i src/*.cpp src/*.h
```
