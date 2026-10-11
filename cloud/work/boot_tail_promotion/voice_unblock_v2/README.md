# Voice-unblock: compact current-compiler promotion candidate

Deliverable: [one genuine standalone source](../../../matches/voice_unblock_production/func_8001F954.c)
and [one-claim group](../../../matches/voice_unblock_production/group.json).
The source and group are byte-for-byte extractions from
[PR #163, head 1bd09c5](https://github.com/cabi24/SFRush2049-decomp/tree/1bd09c5eb3bde8f803c46d0d657e9264d4223a38/cloud/matches/voice_unblock_production).
The old aggregate remains research history; this packet does not supersede its
other sources, behavioral work or portability repairs.

At master `9a3925dfa` on 2026-10-11,
`func_8001F954` is still a 124-byte `GLOBAL_ASM` slot in `src/rom/lib_1f5b0.c`.
Its old standalone source is already locked, so this is **zero newly discovered
matching bytes, 124 distinct promotion-candidate bytes, zero accepted/ROM bytes**.
Open PRs #305–#311 do not claim this static target. The only competing adaptation
is the unchanged original in #163; do not count it twice or merge both as new work.

## Current evidence

`lib_1f5b0` now pins **IDO 7.1**. This replay resolves the actual ROM compiler with
`tools.conveyor.compilers.tu_cc`, identifies the pinned release, and uses that
compiler for the O3 group and all production-O2 and whole-TU builds. It does not
reuse #163's IDO 5.3 proof or compile the ROM TU through `score.ido("cc")`.

- Sole claim: strict 31/31 native words at `[0x8001F954, 0x8001F9D0)`.
- O3 canonical group, O2 production-recipe source and O2 `rom_tu.h` context all
  match, including fully resolved relocations, exact ELF extent and no owned data.
- Separate GNU link of the standalone group verifies all 124 bytes, function
  address/extent, four zero alignment bytes and exact ELF32 big-endian MIPS2 ABI
  metadata. `--hash-style=sysv` keeps the ABI identity explicit across GNU versions.
- Native compiler assertions verify 4-byte pointers, 16-byte `SequenceNode`,
  416-byte `VoiceState` and every used/shared named field offset.
- The live production TU and its scratch insertion compile through the actual
  asm-processor route. All 18 functions / 4,604 summed function bytes match; all
  14 pre-existing C bodies remain textually unchanged. Existing passthroughs earn
  no C credit. The replay also tolerates a future identical promoted C body.
- 2,112 actual-source C89 host cases check sentinel handling, call order, arguments,
  side-effecting helper hooks and complete-array effects under ASan/UBSan/bounds.
  Three freshly checked adverse controls reject a wrong identifier assignment,
  introduced owned data and a wrong ELF function extent.

The body differs from the established master standalone source only by the two
production record field names (`identifier60`, `activeBD`). Its actual record
declarations equal the live TU; a named `state` prototype keeps the real external
helper visible before this slot under the normal whole-file declaration dedup.
No fake callers, retained stubs, private-register tricks or unrelated changes.
The original [detailed contract and historical proof](https://github.com/cabi24/SFRush2049-decomp/tree/1bd09c5eb3bde8f803c46d0d657e9264d4223a38/cloud/work/boot_tail_promotion/voice_unblock_contract)
remain available, including the old refusal and native-interpreter investigations.
They are provenance, never replay dependencies.

## Cheap replay and input binding

From the repository root with the pinned compilers/MIPS binutils installed:

```sh
python3 cloud/work/boot_tail_promotion/voice_unblock_v2/verify.py --check
REQUIRE_TOOLCHAIN=1 python3 -m pytest -q tests/cloud/test_voice_unblock_v2.py
IDO_DIR=tools/cloud/ido71 python3 tools/cloud/score.py group \
  cloud/matches/voice_unblock_production --claims --targets asm/us/boot_tail
```

The one collected test first validates inputs/source contracts, then runs the
complete replay in a fresh process with a 25-second ceiling. Shared conftest
handles absent IDO; absent GNU/host compilers skip normally and fail under
`REQUIRE_TOOLCHAIN=1`. `--check --inputs-only` needs no compiler.
Independent peer replay passed in 1.15 seconds; 31 local packet/compiler tests
passed and all 493 static locks were intact. The local complete replay is under one second; the one-test run is comfortably
under the protocol's 30-second IDO budget. No full legacy replay is collected.

The receipt binds only the extracted source, group, established source used for
the two-field-name equivalence, host proof source, and this verifier. It also
records only selected protected `extents.json` / `symbols.json` `SHA256SUMS` rows and the claimed target's
word hash. It does not pin tools, locks, Makefile, whole target manifests, or
mutable splat assembly text. Normal scorer target integrity remains mandatory.
Fresh live-TU results are checked strictly but are not compared to archived
surrounding-TU metadata; unrelated future promotions need no receipt rebind.
No `git show`, unmerged branch or historical tree is needed to replay.

## Landing path and limits

The coordinator must obtain fresh 7.1/O2 score-zero evidence for this adapted
source, replace/migrate the old 5.3 standalone lock without duplicate target
ownership, generate a compiler-labelled context row, and use the unchanged
static promotion transaction plus full-ROM SHA-1 gate. The candidate's O3 header
is a submission recipe, not the O2 production lock. No source lock, production TU,
protected reference, tool, workflow or gate changes are included here.

Host hooks do not execute real helper bodies; the valid domain is slots 0–31 and
`0xFFFFFFFF`. Host pointer size is not evidence of N64 layout. LeakSanitizer alone
is disabled in this container; address/bounds/undefined checks remain enabled.
No original whole-runtime behavior, arbitrary-index safety, image/compression,
full ROM, hardware/gameplay or promotion completion is claimed. Source admission
and merging remain with the independent checker.
