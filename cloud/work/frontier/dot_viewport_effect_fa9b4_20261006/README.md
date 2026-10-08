# Complete FA9B4 viewport/effect caller reconstruction

## Result

`render_viewport_init`, main game address `0x800FA9B4`, is now complete semantic C rather than the historical empty stub. The single canonical O3 baseline is **NONMATCH research**: 912 candidate function/text bytes versus 924 native bytes, 227/231 positional words different. Every relocation resolves, with no masks, unverified references, errors, or nonzero excess words. No match/coverage/ownership claim is made.

Base context is `6b2e9e506fe3d2267a710e41c85af5364ccd00c7`. The stale stub's README says 948 bytes; the authenticated complete target is 924 bytes. Interior historical label `loading_screen_handler` at FA9E4 is not a new function. The misleading native name `save_write_data` at AF06C is retained only as a link symbol; the routine is an effect service, not serialization.

Exactly one native compiler invocation was performed after source, ABI and pre-build semantic review. It used the bare source header `-g0 -O3 -mips2 -G 0 -non_shared` through the unchanged canonical `score.compile_single` route, including its mandatory `-Wab,-r4300_mul`. There was no optimization/layout sweep. The 128-byte candidate frame versus the native 192-byte frame, an additional saved floating pair for the consumed 1.0 constant, register allocation and instruction ordering remain real differences. No padding locals, dead reads, pressure work, fake callees/arguments, assembly, ungrounded volatile or production changes were introduced. The unknown byte ranges in record declarations describe native field offsets; they are not automatic stack objects.

## Source contract

The no-input, no-consumed-result caller preserves this ordered behavior:

1. Preflight, then mode-specific roots: mode 4 C2BE0; mode 6 B:FCE0 then B:0F60; otherwise mode 5 D169C or F93A0, followed by F8EC8. CADA4 and AEB54 always follow.
2. Iterate parallel player/model arrays with the live signed-byte count reloaded after every iteration. Capture each model's signed-short slot before any event helper.
3. Negative event: conditional wrapping unsigned-half statistic increment, optional auxiliary position snapshot and state save, effect creation with `&slot, 1, 1.0f, 0`, sound cleanup, conditional sound dispatch, optional mode-4 C3578, then event clear.
4. Positive event: clear flag bit 0x20; optionally restore auxiliary state and invoke F8E90; clear the event. Zero event does neither.
5. Invoke B0180 with the captured slot. Invoke BEAA0 for status 0/6 only when mode is not 2 or the captured slot is zero.
6. Invoke BD2C8 and CF69C; F0100 follows only if the final signed-byte flag is zero.

Layouts are deliberately partial local views, not claims about complete original type names:

- PlayerState: stride 952; signed bytes +ED event, +35C auxiliary index, +35D/+35E state bytes.
- ModelState: stride 2056; float position at +22C, signed short slot +7C6, signed byte mode +7CC.
- ScoreRecord: stride 76; unsigned-half wrapping counter +40.
- EffectRecord: stride 152; three float source fields +24..2C and destination fields +84..8C.
- Status: eight-byte entries with an unsigned byte at +7.
- Nonnull object table element: pointer +0, pointer +28, pointer +0, signed byte +5. Only the table element itself is null-checked, as in native code.

The sound cleanup helper may change the model's slot/mode. Sound dispatch reloads those model fields, while subsequent index-taking helpers use the earlier local slot. No callee is declared pure and the mode/count globals stay mutable at external boundaries. The source's event logic does not add a lower-bound guard absent from native code; the replay uses valid array indices 0..3.

## External boundaries and image identity

The independent viewport runtime audit authenticates FA9B4, main AF06C and the actual B-image FCE0/0F60 bodies against the historical decompressed data asset, rejects both A-image address collisions, and traces the mode-6 loader path. Its base is the same commit. Image B must already be resident via that lifecycle; the caller does not load or revalidate it.

FCE0 and 0F60 are ordinary `void(void)` roots. Their private child conventions do not become caller arguments. AF06C has four ordinary parameters: a tagged pointer, mode word, float scale, sound word. The FA9B4 short local is only read/copied; its address does not escape and is not written through. All three can remain opaque external boundaries without matching their implementations.

The additional seventeen direct main-game services were read independently at the base. Their explicit nonvolatile register writes have matching stack saves/restores. Their consumed signatures are:

- EC0DC, C2BE0, D169C, F93A0, F8EC8, CADA4, AEB54, BD2C8, CF69C and F0100: no logical inputs consumed by this caller.
- D5524: model record pointer. Actual slot +7C6 and mode +7CC reads corroborate this type.
- 92360: three words and a low byte; returned handle is unused here.
- AED64: consumed position float pointer, third float, fifth/sixth float stack inputs, three following words, final low byte. The native second/fourth carriers are really supplied but unused by that body. Their `const void *`/float declarations are semantic views, not claims to recover an unobserved original formal type. The second address object's contents are never read by this caller or callee, so it remains an opaque byte declaration. The consumed result is a word handle elsewhere, ignored here.
- C3578: full-word index; F8E90, B0180 and BEAA0: signed-short index, proved by their entry narrowing/homing.

No complete transitive simulation of these helper bodies is claimed. B0180 and BEAA0 can invoke real callbacks; the caller replay conservatively models memory mutation and arbitrary caller-saved clobbering, rather than assuming they preserve global fields.

## Verification

- Before the native build: 10,000 native-word versus literal-host-C fixtures passed.
- After the one native build: the same 10,000 fixtures passed three-way native/compiled/host comparison, totaling 20,000 machine executions. Native versus compiled memory regions compare byte-for-byte, including untouched fields.
- Literal candidate C passes 10,000 host cases under ASan/UBSan.
- All 20 external service identities are exercised. 229/231 native instruction words and 54 branch outcomes are covered. Two duplicate setup instructions at offsets 0x78 and 0x224 have no reachable predecessor in this body; they are not silently omitted from strict comparison.
- Cases include all mode families, nonpositive/positive signed counts, signed event and auxiliary-selector extremes, enabled/disabled sound, nullable pointer chains, flag masks, wrapping counters, status values, and normal-or-zero binary32 payloads including signed zero.
- One selected callback per fixture can alter mode, live loop count, model slot/mode, sound flag, status, final flag or event. Calls poison caller-saved integer and floating registers; stack and all conventional nonvolatile registers are checked after return.
- Unsupported opcodes and unmapped reads fail closed. Byte/half/word accesses enforce their natural alignment; the producer negative control explicitly checks an unaligned word read. Floating doubleword saves/restores are modeled as two word accesses, so the interpreter does not independently enforce eight-byte alignment. The independent review separately proves eight-byte stack adjustments and doubleword offsets in both bodies, and rejects offset-plus-four store mutants.

The oracle never invokes the target compiler. `--native-object` reuses an existing baseline object. The host includes `candidate.c` literally. Bounded boundary stubs check ordered call arguments, not whole-game effect execution or frame-to-frame callback lifetime. Invalid indices, arbitrary pointer corruption, concurrent changes, every lifecycle transition, original-source identity, full-ROM identity and matching are outside the result.

## Reproduction

Use a repository containing BASE in its history and current canonical tools/targets. The tool root and history root may be distinct. Supply absolute paths from an unrelated working directory; set TMPDIR to a writable workspace directory.

Host build:

    cc -std=c89 -pedantic -Wall -Wextra -Werror -O2 -shared -fPIC host.c -o /workspace/tmp/host.so

Pre-build semantic replay:

    python3 verify_semantics.py --reference-root REPOSITORY --tool-root REPOSITORY --host-library /workspace/tmp/host.so --output /workspace/tmp/semantics.json --cases 10000

One canonical build:

    python3 baseline.py --tool-root REPOSITORY --object /workspace/tmp/candidate.o --output /workspace/tmp/baseline.json

Reuse the object for three-way replay by appending `--native-object /workspace/tmp/candidate.o`. Set IDO_DIR to the pinned IDO installation and PATH/LD_LIBRARY_PATH for MIPS binutils. The build script skips cleanly unless both pinned IDO and MIPS GNU linker are available.

The packet binds its own C/harness/verifier and native target words. It does not pin mutable production/tool/lock hashes or assert live acceptance state. Publication, protected edits, integration aggregates, lock/ROM acceptance and CI watching were intentionally not performed.

Additional local evidence:

- `runtime_contract_audit.py` and `independent-identity-replay.json`: copied source of the independent read-only runtime audit, replayed locally against BASE; twelve target identities and two wrong-image controls pass.
- `audit_helpers.py` and `additional-contracts.json`: identity and explicit nonvolatile-write/save-pair checks for the seventeen remaining services, with no compiler invocation.
- Focused `tests/cloud/test_viewport_effect_fa9b4.py`: 3 passed while reusing the existing one-shot object; 2 passed/1 skipped with IDO missing; 2 passed/1 skipped with MIPS GNU linker missing. Set `RUSH_REFERENCE_ROOT`, `RUSH_TOOL_ROOT` and optionally `RUSH_VIEWPORT_OBJECT` to explicit absolute paths. The object override avoids additional target builds. Without it, the one compiler-dependent test builds only after the toolchain guard passes. No test file hash is embedded in any receipt.

The suite's subprocess replay changes to an unrelated temporary working directory. Generated binaries, native byte buffers and assembly listings remain outside the packet and are not deliverables.

## Independent source review

The separate source-review lane, archived under `review/`, checked the same frozen source and existing O3 object without a target compile. Its 2,548 three-way fixtures include 2,145 repeated-boundary schedules and 22,729 applied mutations. It compared ordered arguments and all modeled nonstack bytes at every call and return, and poisoned outgoing argument homes after calls (0..12 bytes, extended through 36 for the ten-argument positional service). All passed, including the full suite under UBSan. Sixteen deliberate source mutations were rejected by altered observations or explicit layout checks. The reviewer independently reproduced the canonical 227/231 NONMATCH comparison and statically proved eight-byte alignment of every native/candidate stack adjustment and FP doubleword access; misaligned doubleword-store mutants were rejected.

This review has independently selected fixtures and boundary checks, but reuses the producer instruction interpreter. It is not an independent CPU implementation or actual callee execution. Source SHA-256 remains `e843f3500c6a952dd067949a7d5f894febfb8d635d77baa480778360dc95f4ee`; the existing object remains `03cd24df3ce24448c2f8dc2e91e2f0553565b8dff60113b0da27c72ec361e9b5`. The sole review correction was narrower documentation of the producer interpreter's alignment scope, with no source/object changes.
