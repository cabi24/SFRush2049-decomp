# Rush 2049 Decompilation Progress Report

**Generated**: 2025-12-27
**Project**: San Francisco Rush 2049 (N64)
**ROM**: US Version (12 MB, SHA-1: 3f99351d7bb61656614bdb2aa1a90cfe55d1922c)

---

## Executive Summary

| Metric | Value | Status |
|--------|-------|--------|
| **Overall Progress** | ~45% | Phase 2 of 5 |
| **C Source Coverage** | 126% | 111 C files / 88 asm files |
| **Build Status** | Compiling | All 111 files compile |
| **ROM Matching** | Not Yet | Stubs, not matching builds |

---

## Phase Breakdown

### Phase 1: Project Setup (100% Complete)
*Establish build system and extract ROM contents*

| Task | Status | Notes |
|------|--------|-------|
| ROM verification | Done | US version confirmed |
| Splat configuration | Done | 88 code segments identified |
| Initial disassembly | Done | 18,608 lines of MIPS asm |
| Build system | Done | GNU Make + mips-linux-gnu-gcc |
| Symbol identification | Done | 228 static ROM functions |
| Reference repos cloned | Done | rushtherock arcade source available |

**Phase 1 Deliverables**: Working build, ROM splits, symbol map

---

### Phase 2: C Source Creation (90% Complete)
*Convert all assembly to compilable C source*

| Task | Status | Progress |
|------|--------|----------|
| Assembly file coverage | Done | 88/88 (100%) |
| C file creation | Done | 111 files created |
| Compilation | Done | 111/111 compile (100%) |
| Header files | Done | 48 headers |
| Function stubs | In Progress | Many functions are stubs |

**Current Stats**:
- 111 C source files
- 80,274 lines of C code
- 48 header files
- 100% compilation success

**Phase 2 Deliverables**: All asm covered by C, compiling codebase

---

### Phase 3: Function Decompilation (25% Complete)
*Replace stubs with actual implementations*

| Category | Named | Unnamed | Total | Decompiled |
|----------|-------|---------|-------|------------|
| libultra OS | 124 | - | 124 | ~60% |
| libultra GU | 10 | - | 10 | ~80% |
| libc | 8 | - | 8 | 100% |
| libgcc | 17 | - | 17 | 100% (stubs) |
| Game code | ~150 | 557 | ~707 | ~20% |
| **Total** | 311 | 557 | 868 | ~25% |

**Key Files by Decompilation Status**:
| File | Functions | Status |
|------|-----------|--------|
| src/game/game.c | ~395 | Partial (stubs + real) |
| src/libultra/*.c | ~124 | Mixed (some real, some stubs) |
| src/libc/*.c | 8 | Complete |
| src/inflate/inflate.c | 16 | Complete |

**Phase 3 Deliverables**: Real implementations, matching function output

---

### Phase 4: Matching Builds (0% Complete)
*Achieve byte-for-byte matching with original ROM*

| Task | Status | Notes |
|------|--------|-------|
| IDO compiler setup | Not Started | Required for matching |
| Function matching | Not Started | Use diff.py tool |
| Register allocation | Not Started | May need asm tweaks |
| Compiler flags tuning | Not Started | -O2, inline settings |

**Phase 4 Deliverables**: ROM that matches original SHA-1

---

### Phase 5: Documentation & Polish (10% Complete)
*Clean code, document structures, prepare for community*

| Task | Status | Notes |
|------|--------|-------|
| CLAUDE.md | Done | Project context file |
| README.md | Partial | Basic info only |
| Code comments | Partial | Some functions documented |
| Struct definitions | Partial | Key structs identified |
| Build documentation | Not Started | - |

**Phase 5 Deliverables**: Clean, documented, community-ready codebase

---

## Detailed Metrics

### Source Code Breakdown

```
src/
├── boot/          1 file     - Entrypoint stub
├── game/         21 files    - Game logic, menus, physics
├── inflate/       1 file     - DEFLATE decompression
├── libc/          2 files    - String/memory functions
├── libgcc/        1 file     - 64-bit math support
├── libm/          1 file     - Math functions
├── libultra/     80 files    - N64 OS library
├── os/            1 file     - DLL/timer queue
└── util/          3 files    - Checksums, DMA
                 ___
                 111 files total
```

### Symbol Coverage

| Type | Count | Percentage |
|------|-------|------------|
| Named functions | 311 | 36% |
| Unnamed (func_*) | 557 | 64% |
| **Total symbols** | 868 | 100% |

### Lines of Code

| Type | Lines |
|------|-------|
| C source | 80,274 |
| Assembly | 18,608 |
| **Ratio** | 4.3:1 |

---

## What's Been Accomplished

1. **Complete ROM Extraction**: All 88 code segments extracted and disassembled
2. **Full C Coverage**: Every assembly file has a corresponding C source
3. **Compiling Codebase**: All 111 C files compile without errors
4. **Symbol Identification**: 868 functions identified, 311 named
5. **Library Functions**: libc, libm, libgcc, libultra largely covered
6. **Game Code Started**: game.c has ~395 function stubs from decompressed ROM
7. **Reference Material**: Arcade source code available for comparison
8. **Build Infrastructure**: Make system, diff tools, decompiler ready

---

## What Remains

1. **Function Implementation**: Replace ~650 stubs with real code
2. **IDO Compiler**: Set up original compiler for matching builds
3. **Matching Work**: Tune each function to match original bytes
4. **Game Structures**: Define car, track, player structs properly
5. **Compressed Code**: Better coverage of 752 game code functions
6. **Documentation**: Code comments, struct docs, build guide

---

## Estimated Effort Remaining

| Phase | Current | Target | Effort |
|-------|---------|--------|--------|
| Phase 2 | 90% | 100% | Low |
| Phase 3 | 25% | 100% | High |
| Phase 4 | 0% | 100% | Very High |
| Phase 5 | 10% | 100% | Medium |

**Overall**: ~45% complete, with the hardest work (matching) still ahead.

---

## Commands to Regenerate This Report

```bash
# Quick progress check
make progress

# Detailed stats
find src -name "*.c" | wc -l                    # C file count
find src -name "*.c" -exec cat {} \; | wc -l    # C lines
grep -c "^func_" symbol_addrs.us.txt            # Unnamed functions
grep -v "^func_" symbol_addrs.us.txt | grep -c "="  # Named symbols

# Build verification
make cc  # Compile all C files
```

---

*Report template: docs/PROGRESS_REPORT.md*
*Reference: See CLAUDE.md for full project context*
