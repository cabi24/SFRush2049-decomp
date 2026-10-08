# Genuine-context Controller Pak reset: A3724 only

**Claim: the complete 88-byte `func_800A3724` body, strict under canonical O3. Nothing else is newly claimed.**

Base context: `6b2e9e506fe3d2267a710e41c85af5364ccd00c7`. This local packet is prepared for independent review. It has not been published, integrated, locked, spliced, or image/ROM gated. The parent owns later aggregate gates and publication authorization.

## Source and boundary

`cloud/matches/pak_reset_a3724_group/group.c` is the byte-for-byte frozen mechanical expansion of the independently reviewed complete Pak source. It uses the required bare first line `/* flags: -g0 -O3 -mips2 -G 0 -non_shared */`. Its historical producer-stage comments describe the precompilation source gate; this receipt records the subsequent one-baseline result.

The reset source has one logical `u8 port` formal. It selects a 772-byte Pak, caches signed byte +5, clears the entire object with `memset`, and restores the cached byte. There are no fake leading formals, empty pressure branches, stack padding, register annotations, noinline controls, synthetic callers, or manufactured escapes.

The unit includes both complete real prepend callers (A2D4C and the 3,200-byte native A377C reconstruction), complete cleanup/reset helpers, the real externally reachable `car_lod_select` initializer, status conversion/update, queue operations and the genuine heap-release source chain. A377C retains both genuine inputs, including the live motor-error suppression parameter.

The eight kept roots have genuine callers outside this unit: `func_800A1E94`, `func_80095F8C`, `func_80095EF4`, `audio_reverb_update`, `func_800A43FC`, `car_lod_select`, `track_process_main`, and `track_render_process`. The natural prepend, memory poison, cleanup and reset helpers have all their observed direct callers inside this chosen unit. Four-image census evidence supplies this bounded visibility hypothesis; computed pointers, untracked data, original linkage, original source order and original whole-program boundaries remain unknown.

The tested source order is common/types, heap release, cleanup/reset, status updater, car_lod_select, A2D4C, A377C. No order or retention sweep was performed. Externally reached heap routines remain external, so their larger native private-register optimization context is not recovered here.

## Complete evidence

The sole baseline used the unchanged canonical `score.compile_group` cc/uld/usplit/umerge/uopt/ugen/as1 route, including mandatory `as1 -r4300_mul`. The matching source and compile-affecting group fields are unchanged. This packet changes only the explicit scoring claim list to A3724.

- A3724 is a normal global ELF `STT_FUNC`, `.text+0x3A4`, size 88. Independent ECOFF end/auxiliary/PDR evidence agrees. No private-locator workaround is needed.
- All 22 words, all three relocations and the complete 88-byte body equal native. No mask, unresolved relocation, own data, prefix crop or deleted-stub credit is used.
- The first instruction consumes the low byte of a3. The genuine compiled A377C calls at function offsets +0x190 and +0x200 and car_lod_select call at +0xB8 supply a3. The three native call sites A391C, A3990 and A45C0 are independently checked.
- 2,048 focused executions cover all four ports, every signed-byte bit pattern at Pak+5 and two a3 high-bit patterns. Exactly one modeled memset clears only the chosen 772-byte Pak; +5 survives; adjacent Pak/guard bytes and sp/ra remain intact. Caller-clobbered registers are poisoned across memset. The private body is allowed its native s0/s1 clobbers.
- Precompile source review passed 210 original and 954 extended native/host executions, 104 cleanup/reset executions, 68 layout assertions and nine fail-closed drills. The same existing compiled object subsequently passed all 1,164 A377C native/compiled fixtures with unchanged service hooks and actual compiled empty-list cleanup/reset.
- The receipt covers every ordinary ELF function's full extent, every relocation, the entire owned 48-byte jump table, compiled body digests, canonical scores, native/compiled callers and reset behavior. The independent full-object audit also identified three hidden queue helpers as separate eight-byte return stubs; they receive no claim.

All behavioral evidence is bounded. SDK/UI/list/codec/heap internals are modeled service contracts. The compiled A377C fixtures do not cover nonempty cleanup. The focused reset interpreter rejects unsupported instructions/unmapped memory; it is not a system emulator or original-source equivalence proof.

## Nonmatches and accepted-context regressions

| Body | Compiled / native bytes | Different / native words | Status |
|---|---:|---:|---|
| func_800A3724 | 88 / 88 | 0 / 22 | Only new claim |
| func_800A1E94 | 132 / 132 | 0 / 33 | Existing accepted context, own table proven |
| func_80095F8C | 76 / 76 | 0 / 19 | Existing accepted context |
| no_catchup | 8 / 56 | 14 / 14 | Deleted return stub; no credit |
| func_80095EC0 | 52 / 52 | 8 / 13 | Accepted-context regression |
| func_80095EF4 | 176 / 152 | 36 / 38 | Accepted-context regression |
| audio_reverb_update | 260 / 244 | 60 / 61 | Accepted-context regression |
| func_800A43FC | 360 / 212 | 51 / 53 | Accepted-context regression |
| func_800A3640 | 204 / 204 | 31 / 51 | Nonmatch |
| car_lod_select | 324 / 324 | 19 / 81 | Nonmatch |
| track_process_main | 1776 / 1752 | 348 / 438 | Unchanged full prior NONMATCH |
| track_render_process | 3052 / 3200 | 786 / 800 | Complete-function NONMATCH |

The five base-accepted bodies that regress are no_catchup, 95EC0, 95EF4, audio_reverb_update and A43FC. Do not replace their accepted production sources with this unit. Only A3724 appears in `members` and `claims`; all eleven other named bodies are context. No hidden or named stub is claimed. No whole-group, linked-image or ROM admission is asserted.

The canonical scorer reports two extra nonzero words after A3640 because it scans to the next ordinary ELF function symbol. ECOFF identifies the intervening sixteen bytes as two separate queue return stubs. A3640's actual 204-byte procedure has no internal excess. The unchanged canonical score is preserved.

no_catchup still inlines into both real callers, leaving an eight-byte return stub. The two-caller retained 56-byte boundary hypothesis fails. There is no justification for another declaration, flag or retention variant without new source evidence.

## Portable replay

Set `RUSH_RECOVERY_ROOT` to a checkout containing the recorded base history. Set `IDO_DIR` to the pinned IDO tools if they are outside that checkout; ensure `mips-linux-gnu-ld` is on PATH. Use the runner's own `TMPDIR` when needed.

    python3 cloud/work/pak_reset_a3724_o3/verify.py
    python3 -m pytest tests/cloud/test_pak_reset_a3724_group.py -q

The normal verifier compiles this exact source/recipe once into a fresh temporary workspace and compares only the claimed evidence: complete compiled bodies, extents, relocations, own-data bytes and behavior. It retrieves mutable production/scorer/context inputs with `git show` at the base commit. It does not pin mutable file hashes, assert live lock state, hash its test file, or depend on the current working directory. Toolchain absence skips before any compiler invocation.

For a read-only inspection of the already recorded original object:

    python3 cloud/work/pak_reset_a3724_o3/verify.py --existing-object /private/path/baseline.o

The existing-object route verifies its recorded hash and runs all object/behavior checks without compiling. The object itself is deliberately absent from this packet. After the packet was frozen, the parent authorized one identical fresh canonical verification replay. All seven packet tests passed, including that compiler-dependent replay and both missing-toolchain guards. Every receipt field reproduced exactly. There has been one original source experiment and one verification compilation, with no variants. Full repository aggregate suites, with and without IDO, remain later parent gates.

Only authored source, tests, verifier and metadata belong in this packet. No ROM bytes, target assembly, raw disassembly, binary object, credentials, copied production source or protected compiler tooling is included.

Digest clarification: A1E94's `relocation_applied_body_sha256` is computed before owned-section placement and retains two masked section-relative fields. Its separate `own_data` proof verifies the complete 48-byte jump table; this intermediate digest is not described as fully relocated.
