# Image B: lives-text callback at 80393518

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

Status: complete source, strict 100/100-word MATCH. No accepted-byte, image,
recompression, cartridge, or full-ROM claim. Base: master
`cd22879d40b3de443cfde047b86e75e159b6cec6`. Independent review is required before
publication/integration. Source is `cloud/matches/ovl_b/func_80393518.c`.

## Identity and ownership

The whole native interval is image B `[0x80393518,0x803936A8)`, 400 bytes.
Native SHA-256 is
`3d42d71c005a624710193d99402c216ce58b37a1c2a2fae0847a5ab832c0ed6a`;
image SHA-256 is
`b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd`.
`asm/us/ovl_b/extents.json` marks the start by data reference and prologue.
All protected source targets/symbols/manifest remain unchanged. No neighboring
body or helper is claimed. The source was reconstructed from authenticated
native behavior, not claimed to be an arcade-source recovery.

Local branches, master paths, address/function history, existing runtime-image
packets and open-PR searches were checked on 2026-10-06. No matching source or
active ownership for this callback was found. The historical R16 survey tested
raw generated seeds for small bodies; this does not establish an authored
baseline or source identity for this callback. B:D3A4/D798 have existing service
research and were not treated as fresh targets. Submitted A8CC/CA24/9133C and
active 914B4 were excluded.

The registered callback word is at B:80393F24, descriptor 8 (36-byte stride,
base B:80393F08) in the ten-record B:80393DE8 table. B:803925D0 passes that table
and count 10 to `sound_control` (native NewMultiBlit, 800B37E8) in mode 6. The
texture-name sentinel is -1: existing NewMultiBlit source stores the callback
in Info and passes the Blit as Image. This proves registration identity, not
active frame-dispatch reachability, universal callback lifetime, or residency.

## Genuine ABI and behavior

The callback has one genuine opaque incoming a0. Native code homes it at
entry-sp+0, or frame-sp+0x60, even though this body does not consume it. All
s0–s8 saves/restores and the return address are conventional. The frame is
0x60 bytes and return value is 1. No extra formal, pressure local, keeper,
volatile annotation, inline assembly, or fabricated caller was used.

It sets render-helper value 0.0, selects font 10, and sets alignment pair (1,1).
A signed-short player loop uses the live signed-short count at 80151AD0.
For each valid player, signed kind +384 equal to 8 skips output; negative
signed lives +385 also skip. Player stride is 952 bytes at 80152818. Position
is the pair of signed halfwords at B:803941D0 indexed by [count-1][player].
The authenticated four-by-four pair table covers player counts 1..4.
The inner count test is present in native control flow and is retained.

The lives value is formatted with the authenticated `%d` at B:80394AB0 into a
real 12-byte local array. `fcvt_wrapper` at 800B4360 is the existing sprintf-like
helper. The signed-byte nonnegative domain 0..127 needs at most four bytes;
12 also holds any signed-32 decimal value including sign and NUL. Array length
is a source-level inference supported by native layout, not an original-source
claim. Palette 0 draws the shadow at signed-halfword (x+1,y+1), then palette 1
draws at (x,y), via `state_utility` at 800B71D4. Coordinates are captured before
formatting and reused for the pair. Count is reread after drawing before the
next player; external helper effects are not assumed absent. At exit alignment
becomes (0,3), render-helper value becomes -1.0, and the callback returns 1.

`render_helper` stores its float and clears global bit 0x10 for both values used
here. `func_800B669C` stores the alignment pair. `dispatch_handler` modes 0/1
select palette entries; these are not generic event operations. Existing helper
implementations remain external and unmodified.

## Build and bounded hypothesis

Current IDO 5.3 recipe: `-g0 -O3 -mips2 -G 0 -non_shared`, with the
canonical scorer adding `-Wab,-r4300_mul`. The original build below used O2;
fresh O3 evidence and the immutable historical receipt are identified below.
The source contains one complete function and no optimizer-visible helper body.
The initial complete source used a 16-byte decimal buffer: 96/100 native words
already agreed, with only four frame/home/buffer offsets different. Workbench
diagnosis found identical 40-byte saved-register areas and an eight-byte
non-save-frame discrepancy. No schedule/register/logic changes were needed.

One directed control changed the real decimal buffer from 16 to 12 bytes. It
predicted the frame 0x68→0x60 and buffer offset 0x50→0x4C. All four differences
then vanished. The 16-byte control remains reproducible in `verify.py` and
must fail strict equality with exactly four differing words. No further
permutations were attempted. The current candidate is frozen for review.

## Reproduce

From a checkout with the pinned compiler and MIPS binutils:

    python3 tools/cloud/score.py fn cloud/matches/ovl_b/func_80393518.c func_80393518 --targets asm/us/ovl_b --flags "-g0 -O3 -mips2 -G 0 -non_shared"
    python3 cloud/work/runtime_b_lives_text_20261006/verify.py --check

For a source-only local packet, `verify.py --reference-root /path/to/repository`
reads the unchanged target files and pinned asset through that Git repository.
The packet root still supplies the unchanged current score/owndata tools.

The verifier binds complete source, recipe, target, tools and object hashes;
checks every relocation against the full 400-byte native body; rejects any
owned data; and links the whole object with GNU ld at its exact VMA. Symbol
assignments precede section placement, which avoids GNU-version-dependent
backwards-jump relocation behavior. Native bytes are never published.

Host C89 tests run ASan, UBSan and bounds instrumentation over 1,796 fixtures:
all signed-byte lives values, negative/zero/1..4 counts, kind exclusion, exact
helper ordering/text/positions, genuine callback return, unchanged player data,
signed-halfword coordinate wrap at 32767/-32768, and helper mutation that shrinks or grows count and modifies subsequent
player/table state. Four compiled semantic mutants must fail. These host checks
are bounded; they do not execute renderer internals or prove runtime residency.
Independent native execution, when available, is recorded separately rather
than claimed by this verifier. Full-ROM gates were not run.

## Private-caller scout, explicitly not admitted as standalone work

Read-only native evidence found these actual dependencies:

- A95C `[8038A95C,8038AA14)`, 184 bytes: consumes signed-short index in s4 and
  clobbers unsaved s0–s3. Real parent AA8C `[8038AA8C,8038C910)`, 7,812 bytes,
  supplies s4 at call sites 8038AB5C and 8038ADEC.
- AA14 `[8038AA14,8038AA8C)`, 120 bytes: index in a0 is genuinely homed, but
  loop locals clobber unsaved s0–s3. Same AA8C parent calls at 8038AB40/8038ADC4.
- D200 `[8038D200,8038D328)`, 296 bytes: consumes record pointer s1 and mode a0;
  E114 supplies s1 from its actual record and mode 0/1/2.
- D328 `[8038D328,8038D3A4)`, 124 bytes: consumes actual s1 resource index,
  s2 parent handle, s3 flags and s4 transform-mode discriminator. E114 prepares
  these values, including indices 1,2,3,5,7,8,9, parent -1 or existing object,
  and real flags 0/0x2080/0x800000.
- E088 `[8038E088,8038E114)`, 140 bytes: consumes the record in s1 and clobbers
  s0. E114 call sites 8038E4E0/8038E6B0 set s1 from its live record pointer.

D200/D328/E088 share real parent E114 `[8038E114,8038F560)`, 5,196 bytes; its
caller FCE0 also participates in the image's genuine closure. These mappings
do not supply complete parent source or matching admission. No tiny dummy
caller, false ordinary ABI, or extra unused arguments can replace that work.
Next useful action is independent review of the ordinary-ABI callback above;
private groups remain bounded dependency findings, not another permutation run.

## Integration-portable replay (2026-10-06)

Scorer, whole-manifest and accepted-context digests are historical provenance,
not live-tree requirements. The verifier normalizes only enumerated provenance
fields on both receipt sides. Packet source and verifier bindings, compiler
identity/actual flags, selected native bodies and addresses, complete emitted
extents, relocations, owned data and behavioral checks remain binding.

The submission now uses the bare header
`/* flags: -g0 -O3 -mips2 -G 0 -non_shared */`. Fresh O3 strict replay
uses the unchanged canonical scorer, which adds `-Wab,-r4300_mul`; the receipt
records those actual compiler flags. The complete standalone function needs
no callers, inlined helpers or deleted-static stubs in its translation unit.
Historical O2 source/receipt evidence remains available at integration commit
`6b2e9e506fe3d2267a710e41c85af5364ccd00c7`; it is not relabeled as O3.

O3 retains path-dependent nonallocated ECOFF debug metadata. Raw object SHA-256
is historical provenance only after `portable_object_elf` binds the full ELF
header, section identity/attributes, symbols, all relocations, executable/data
bytes and storage sizes. Only nonallocated `.mdebug` bytes and physical file
offsets are excluded. Each replay recompiles identical source in another path
and rejects mutations to text, relocation/symbol tables, register metadata and
ELF ABI flags. Native/body/extent/data/behavior checks are unchanged.

### Host-tool portability

The receipt records host GCC and GNU linker binary hashes as provenance, not
frozen proof inputs. Pinned IDO hashes, packet/source hashes, native words, ELF
extents and relocations, GNU-linked text bytes, and all behavior/mutation checks
remain binding. A host tool change must pass those same checks.
