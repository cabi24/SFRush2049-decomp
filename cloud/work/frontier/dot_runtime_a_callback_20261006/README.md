# Image-A menu text callback: complete matching candidate

## Current receipt schema (2026-10-06)

Current receipts omit whole-file manifest, scorer/own-data tool, lock and redundant
production-context digests. The recorded BASE and `git show BASE:path` source reads
remain, as do native target authentication, own packet/verifier/C-harness bindings,
compiler executable identities, complete compiled extents, relocations, owned data
and behavioral evidence. Historical compatibility normalizers accept only their
explicitly listed legacy fields; unknown proof fields and changed invariants still
fail comparison. Descriptions of the earlier receipt schema below are historical.
This schema correction adds no matching or accepted bytes; fresh replay and the
aggregate test matrix are separate required checks.

Target **A:func_803A6A28**, `[0x803A6A28, 0x803A6B50)`, **296 bytes / 74 words**.
Base: `cd22879d40b3de443cfde047b86e75e159b6cec6`.
Status: strict standalone **MATCH**, awaiting independent review and maintainer integration.
This runtime-image result adds no accepted cartridge or game-blob coverage.

## Source and contract evidence

The full ordinary-O32 callback has a 40-byte frame and homes its unused first
argument. It saves/restores only `ra`; there are no private live-in registers,
unsaved saved-register uses, switch tables, local data pools, or owned storage.
The protected image-A extent records `data_ref` and `prologue` evidence. The
callback-address table itself was not inspected. Image B ends below this address.

The existing matched A:803AE51C callback supplies the same float draw-state,
font/alignment/color, text-rendering, state-restoration, and return-one structure
(`cloud/matches/ovl_a/func_803AE51C.c`). This is a witnessed N64 rendering sibling,
not an asserted original translation unit or arcade donor. Available arcade
LIB/font source did not identify an equivalent N64 callback.

The body sets draw state, draws `D_8017A4E4`'s pointer field at +936, optionally
renders formatted text when the word at `D_80156978` equals `0x3C000`, optionally
draws the +920 text field when signed byte `D_80156994` is nonzero, then restores
state and returns one. Keep the second condition after the formatted call:
external helpers may change the later-observed globals and text-object pointer.
`D_803B87E8` remains an opaque format pointer; neither its contents nor the
meaning/signedness of its two word-sized format operands is claimed recovered.

Helper evidence:

- `src/blob/render_helper.c`: one float in `f12`.
- `src/blob/func_800B669C.c`: two word-sized state fields.
- `src/blob/dispatch_handler.c`: integer color selector.
- `cloud/work/tiny_A138/camera_auto_follow.c` and native helper: six signed
  halfword arguments followed by the text pointer; seventh argument at stack +24.
- `src/blob/music_tempo_adjust.c` and native helper: two signed halfword
  coordinates, format pointer, then ordinary O32 varargs.
- `src/blob/groups/slot_sound/group.c`: signed halfword mode argument.
- Independent native helper review: `object_create` returns a pointer ignored
  here, `object_byte9_set` takes/returns a signed byte, and `state_utility`
  narrows both coordinates to signed halfwords. The candidate uses these types.

The 32-bit `MenuText` view has pointer members exactly at +920/+936. The original
struct definition and complete storage bounds are not asserted recovered.

## Verification and bounded history

`verify.py` freshly compiles IDO 5.3 with `-g0 -O3 -mips2 -G 0 -non_shared
-Wab,-r4300_mul`. It checks the protected manifest and exact image-qualified
extent, pins target bytes and 15 relocation anchor addresses, checks the full
ELF symbol extent, then independently links through GNU `ld`.

- Strict production score: **0/74 words**, no unresolved/unverified relocations,
  masks, errors, or excess nonzero words.
- ELF function: **296 bytes**, followed by **8 zero alignment bytes**, separately
  checked. All **29 relocations** resolved. No owned data or extra function body.
- Independent ELF parser and GNU-linked complete bytes equal the native target.
- `gcc -m32` checks pointer width and the two native field offsets.
- **12,544** hosted C89 UBSan/bounds call-trace checks agree with an independent
  Python oracle: all signed-byte states, seven word-mode boundary values, and
  seven helper-triggered mutation positions. The real renderer is stubbed at
  its witnessed call boundary; these are bounded contract tests.
- A compiled inverted-mode-guard control is rejected at **1/74 differing words**.

The first source compile was 2/74 solely because two globals were transcribed
with high word `8014` instead of native `8015`. Correcting those addresses gave
strict equality. No register/shape sweep, artificial keeper, dummy parameter,
inline stub, target/scorer edit, or byte patch was used. Later evidence-based
prototype corrections preserved the same complete matching body.

The initial local receipt used the older recovery scorer. The saved receipt was
freshly rerun with current-master scorer/own-data tooling and unchanged protected
A inputs; no local data relocations are present in this candidate.

## Reproduce

From a full checkout with IDO configured and MIPS GNU binutils on PATH:

```sh
PYTHONDONTWRITEBYTECODE=1 python3 cloud/work/frontier/dot_runtime_a_callback_20261006/verify.py
python3 tools/cloud/score.py fn cloud/matches/ovl_a/func_803A6A28.c func_803A6A28 --targets asm/us/ovl_a
```

In a source-only sparse checkout, pass `--repo /path/to/current-checkout` to reuse
its unchanged protected targets and tooling. Set `IDO_DIR` to the existing IDO
5.3 directory. Generated binaries remain under ignored `build/runtime_a_callback`.
The receipt contains hashes and counts, never ROM bytes or raw instruction dumps.

No broad repository suite, image/recompression/full-ROM gate, hardware run,
original-source recovery, or cartridge coverage is claimed. Publication is a
draft for the independent checker; merging and production integration remain
theirs.

## Integration-portable replay (2026-10-06)

`portable_receipt()` compares both saved and fresh evidence after excluding only
explicit historical whole-tree/tool/source-context digests. Packet source and
verifier bindings, selected native bodies and addresses, ELF extents, relocations,
owned data, behavior, and compiler executable identities remain authoritative.
Accepted production context is read from the recorded base commit rather than
the live integrated tree. Tests are deliberately not hashed into receipts.

The unchanged candidate body was freshly strict-matched through the canonical
scorer with the bare source recipe `-g0 -O3 -mips2 -G 0 -non_shared`; the scorer
still injects its mandatory `-Wab,-r4300_mul` backend flag. Full packet proof was
replayed under O3. No same-unit callers, inline helpers, or deleted-static stubs
are required. Historical O2 results remain available in Git history.

The raw-object digest and checkout HEAD are local provenance; resolved body,
complete extent, GNU equality and allocation/relocation checks bind the proof.
