# Independent AF06C contracts and source audit

Base: `6b2e9e506fe3d2267a710e41c85af5364ccd00c7`.

This audit authenticates the complete 1,200-byte root at `800AF06C`, its 1,128-byte private child at `80090308`, and nine allocator/service/callback bodies from historical native words and the historical compressed asset. `audit_contracts.py` emits metadata only; production context is always read with `git show BASE:path`. No mutable production file or tool hash is pinned. Local private inspection files are not deliverables.

## Supported source contracts

- `void save_write_data(void *input, s32 mode, f32 scale, s32 sound)` is an ordinary preserving root. O32 puts float `scale` in a2 because two nonfloating arguments precede it. No fifth argument is consumed.
- The global shortcut is tested before `mode`: signed gate byte `80156994 == 0` and signed selector byte `8014978C` in `[0,5]` reads a signed halfword even for mode 0. Otherwise nonzero mode uses a signed index and mode 0 uses three binary32 words. Do not reinterpret every mode-0 call as an unconditional vector.
- Native private90308 takes its index in outgoing stack word 0. Its 272-byte frame reads the signed low half at incoming SP+2, or adjusted SP+274 on big-endian target. It freely clobbers saved registers; AF06C contains this behind its ordinary frame. Keeping the complete real helper in the candidate unit is honest. An ordinary external declaration for native90308 would be false.
- The source input pointer never escapes: values are copied to player/effect/node storage. The scene allocator retains pointers to global matrix/effect storage, not the short/vector input. The frame-to-frame lifecycle and ring-slot reuse are outside this proof.
- Node callbacks are `void (*)(EffectNode *, s16)`: all three targets consume a node pointer and explicitly narrow a1 to signed half. The root/helper only register fixed callbacks at node +20; neither invokes a function pointer.
- `8008D6B0` copies exactly nine float words in order, with source then destination pointers. It does not copy the three position words following the matrix.
- `80090088` accepts signed-half scene index plus two full-width flags. `8008E26C` accepts full-width resource, matrix pointer, signed-half parent, full-width flags; returns a sign-extended signed-half scene index. Native callers require a valid returned scene index, with no failure guard.
- `800AED64` is a ten-argument positional sound boundary. AF06C supplies position pointer, unused vector pointer, `400.0f`, unused `0.0f`, `1.0f`, `0.0f`, sound ID, signed node scene, 0, unsigned byte 128. Only the first position pointer is dereferenced; the unused argument homes are never consumed. Its result is ignored here.
- `8008EA10` narrows its first index to signed half and consumes three full-width words. The private child calls `(index, 5, 0, 0)`.

## Field and timing audit

Candidate structures agree with native offsets: node 24, transform 48, player 952, extra effect 60, debris 84, effect group 400, scene 68 bytes on O32. Node +4/+6/+8 are signed halves; node +16 is binary32 and +20 is callback. Scene scale is +12 and color +60. Group has four debris records followed by a 60-byte extra effect and final float timer at +396.

- Root extra-effect color `8011B554` and input index are read before allocation. They are retained across later callbacks. Position is copied after matrix-copy returns.
- General allocation happens independently after the optional extra allocation, even when the extra allocation fails. General allocation failure returns without sound.
- General ring pointer selection uses the live signed-half ring index after allocation. Matrix pointer remains captured after matrix-copy/scene calls. Ring increment reads the live index again after scene creation, narrows to signed half, and wraps at 50.
- Pool head is read after the scene callback, just before linking. Sound-enable byte is read after scene creation. General input is reread after matrix-copy, independently of the earlier optional extra-effect index.
- Root scale is preserved as binary32, written directly to scene +12, and compared inclusively against 1.0 and 0.5. Sound IDs are 45, 69, and 47 respectively. A zero sound argument or zero enable byte suppresses the boundary.
- Private90308 snapshots scale/range/center constants before its four-iteration loop. Each RNG update wraps unsigned 32-bit and extracts 15 bits. Float multiply/divide/add/subtract round separately to binary32; lifetime truncates toward zero before adding 25 and narrowing to byte.
- Private mid/tail eligibility uses signed player count `< 4` and `(gate != 0 || selector >= 6)`. The root's selector nonnegative check must not be added here. The tail test runs even after allocation failure, permitting an event-only path when boundary mutation changes eligibility.
- Private dead color-seed read at `8011B550` is overwritten byte-for-byte with `F4CD1480` before consumption. The candidate deliberately omits this dead ordinary read; the native fixture still maps it.

## Executable evidence

The candidate is compiled unchanged for host execution. Only the adapter translates host pointer width and endianness; it synchronizes external state at every boundary. The native interpreter executes both complete native bodies with branch-delay behavior and rounds each float operation. Every ordinary external boundary poisons caller-save GPR/FPR state, LO, and condition state; outer nonvolatile registers and stack restoration are asserted.

`challenge.py` independently selects cases, authenticates native bodies through this audit's loader, and compares complete mapped storage plus ordered external boundaries. It runs host `-O0` and `-O2`, both with floating contraction disabled, and six deliberately wrong-source controls. See `independent-challenge.json` for source/harness hashes, counts and results. The test file does not hash itself into its receipt.

Coverage is bounded: valid initialized aligned storage, in-range referenced records and scenes, nonaliasing input/global storage, finite normal-or-zero binary32 with default rounding. This is not an FCSR/exception/subnormal arithmetic proof, whole-game behavior proof, strict target match, or proof of historical translation-unit composition. Real scene/audio internals are effect-model boundaries; callback mutations are adversarial semantic challenges, not claims that those helpers normally mutate every tested global.

No target compilation, target tuning, protected edit, external write, publication, or CI monitoring is performed by this audit.

Reproduce from any working directory with an explicit packet and repository path:

`TMPDIR=WORKSPACE_TMP python3 PACKET/independent/challenge.py --packet-dir PACKET --reference-root REPOSITORY --output RESULT.json`

The independent challenge completed 420 fixtures across both host optimization levels, with 571 native instruction addresses and 49 branch outcomes; all six deliberately wrong sources were rejected. These are counts of bounded executed cases, not a claim of full path coverage. HI is not read by the selected native bodies; LO and floating condition state are poisoned at external boundaries.


## Existing-object continuation

The independently selected 210 cases were also replayed against the one existing linked O3 output, without invoking any compiler. All passed, reaching 507 emitted instruction addresses and 51 branch outcomes. `../independent-linked-review.json` binds the unchanged source, existing object and ELF, native targets and interpreter sources.

An independent ELF parser reapplied all 118 MIPS relocations and checked all 39 external symbol addresses against the immutable base context. The resulting complete 2,112-byte text equals the linked ELF. The sole emitted function is 2,096 bytes at text+8, with an 8-byte return prefix and 8-byte zero alignment tail. The private90308 body is naturally inlined. There are zero emitted owned-data bytes/references and no unresolved linked symbols or relocations. The object's 24-byte `.reginfo` is compiler metadata discarded by the diagnostic link.

Direct full-function comparison confirms 298 differing native words out of 300, followed by 224 extra words, 220 of them nonzero. This is explicitly nonmatching. The recorded canonical compiler options and one-build count are checked for receipt/script consistency; a no-compilation audit cannot independently reconstruct prior process history or regenerate the source-to-object mapping.

Reproduce from any directory:

`TMPDIR=WORKSPACE_TMP python3 PACKET/independent/verify_existing_object.py --packet-dir PACKET --reference-root REPOSITORY --object CANDIDATE_OBJECT --elf CANDIDATE_ELF --output RESULT.json`

Original translation-unit admission, strict match, production integration and gameplay/callback lifetime correctness remain outside this result.
