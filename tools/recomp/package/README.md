# San Francisco Rush 2049 (N64) static recompilation: gameplay-test package @@VERSION@@

This folder builds and runs the Rush 2049 decompilation project's static recompilation of the
N64 game as a native PC program, so that a person can play it and report what works.

Read in this order:

1. `INSTALL.md`: build it (Windows 11 first; Linux second; WSL2 only as a fallback).
2. `TEST_PROCESS.md`: the ordered test procedure (about 60 to 90 minutes including the build).
3. `REPORT_TEMPLATE.md`: fill it in and send it back to the coordinator.
4. `CHANGES.md`: what changed since the previous package.

Pins of this package (also in `VERSION.txt`):

| Component | Source | Commit |
|---|---|---|
| this package | Rush 2049 decomp repo, `tools/recomp` | `@@REPO_COMMIT@@` |
| N64Recomp (the recompiler) | `src/N64Recomp` (snapshot) | `@@N64RECOMP_COMMIT@@` |
| N64ModernRuntime | `src/N64ModernRuntime` (snapshot) | `@@RUNTIME_COMMIT@@` |
| RT64 (renderer) | NOT included: `git clone @@RT64_URL@@` | `@@RT64_COMMIT@@` |

ROM included: @@WITH_ROM@@. You need the US cartridge ROM (`.z64`, SHA-1 `@@ROM_SHA1@@`).
The ROM, everything built from it, and the prebuilt binary in `prebuilt/` (which contains code translated
from the ROM) are private: do not upload or share them.
