# Stock IDO 5.3 aggregate-copy lowering: verified version difference

Date: 2026-10-04 UTC. Status: **COMPILER-LOWERING PROVED; SOURCE STILL COMPLETE-NONMATCH**.

## Result

The free-register-availability hypothesis from the 7.1 source is falsified for the
retained 5.3 eight-byte aggregate copies. The approved stock 5.3 `ugen` selects
`$at` for the first word at these small offsets without consulting its free list.
Changing temporary-register pressure alone cannot change that selection.

The unchanged 15720 and 158D8 sources still score 6/110 and 8/77, respectively,
with no excess words, unresolved symbols, unverified references, or errors. No C
candidate was added, no target claimed, and no matching or cartridge credit is due.

## Provenance and direct binary correspondence

The repository setup pins decompals/ido-static-recomp v1.2. The existing host
compiler identifies itself as IDO 5.3, v1.2, built 2024-11-12 by GCC 9.4.0.
Its `ugen` SHA-256 is:

`1734564701d39db213d6fe9c8f2cff16b82b28939fb1418348de479b41b7ac72`

The complete installed compiler-file manifest still has the approved fingerprint:

`8ca550d30c1fef7c14b1470a1ef93cc6d415a507c6e18c04a7bfeaf8016a7b89`

The same upstream v1.2 tag resolves to commit
`9c242adc890beef098020149d9554f48208f699d` and contains the original 5.3 MIPS
`ugen`. It was fetched for read-only inspection and was never executed. Its
SHA-256 is:

`4d6ae5a5ea8a6ee77bfcc4c95195800823d4b1217e757cc516e219d05bbf5345`

Its Git blob SHA-1 independently verifies as
`beb81f4bd5ea81fcf2be6aaa0bcb3f88f4a9f992`.

Although stripped of ordinary symbols, this original binary retains dynamic
symbols. These identify `eval_mov` at 0x0042A078, ending at 0x0042B424;
`get_free_reg` at 0x0043E124; `free_reg` at 0x0043E7F8; and
`free_reg_is_available` at 0x0043DA80. Resolving the original GOT call targets
must account for their 12-byte PIC-prologue skip. There are zero calls to
`free_reg_is_available` anywhere in this 5.3 `eval_mov`.

More importantly, the original selection block at 0x0042AAD8–0x0042ABD4 and the
actual approved host binary's corresponding block at image offsets
0x0005C64F–0x0005C748 independently implement the same selection condition.
The host block reads the two copy offsets and length, without a free-list read
or availability call. This is direct evidence about the accepted host binary,
not an assumption that a different compiler version behaves similarly.

## Exact selection rule on the general copy path

Let S and D be the source and destination offsets peeled from the expression
trees and N the copy length. This 5.3 path requests two ordinary carriers only if:

    D >= 32756 OR S >= 32756
    OR ((D + N >= 32768 OR S + N >= 32768) AND N <= 32)

If true, it calls `get_free_reg(nil, 1)` for the first carrier, then for the
second carrier, and releases the second and then the first before emitting the
copy. If false, it emits the no-at directive, explicitly chooses register 1 for
the first carrier, obtains the second with `get_free_reg(nil, 1)`, and releases
the second before emitting the copy. These are statically established events,
not a claimed runtime allocation trace.

The 1-, 2-, and 4-byte small-object special case does not cover an eight-byte
record. Alignment subsequently selects aligned versus unaligned transfers; it
does not add an availability condition to the eight-byte fallback.

By contrast, the published 7.1 implementation first obtains the second carrier
and then also considers `free_reg_is_available()` when selecting the first.
The earlier source lead therefore was correctly version-limited and cannot be
promoted into a 5.3 allocator-pressure explanation.

## Byte-inert stock diagnostics on the retained sources

Both sources were recompiled unchanged through the recorded stock driver
commands. Replaying those explicit stages produced full objects identical to
the driver's objects. Adding only `ugen -e TREEFILE -l LISTFILE` then produced
full objects identical to the corresponding baselines, including `.text`,
`.rel.text`, symbol and string tables. The unassembled diagnostic stream itself
contains extra listing-related directives and is not byte-identical; the final
object is the byte-inertness test.

Full object SHA-256, equal for baseline, explicit stages, and diagnostic build:

- 15720: `651cbb225e06c872f72f784fde387063b5c4793f7079334dceaa087aa55398c6`
- 158D8: `a7ed727f7cb5cbdf8896da859bed07adadde216e3856d3fcd983a3f0be10eaf5`

The retained Ucode stream was decoded with the already-vendored workbench
parser. Both copy instructions are opcode 88 (`Umov`), aggregate dtype M,
length 8 bytes, source alignment 32 bits (`I1`), and destination alignment 32
bits (`Lexlev`). The post-optimization tree has two address-typed `Ulod` nodes
from the register memory class, at register offsets 8 and 12, corresponding to
destination `$v0` and source `$v1`. They are not offset-bearing `Uadd` trees.
Consequently `eval_mov` leaves both peeled offsets at their initialized zero.
The selection condition above is false for D=0, S=0, N=8.

Copy provenance is line 33, with loop provenance line 32, in both unchanged
sources. The 15720 copy tree is node 134, destination node 130 and source node
133; the 158D8 copy tree is node 132, destination node 128 and source node 131.
The resulting preassembler carrier pairs remain `$at/$t3` and `$at/$t9`.

The stock input also exposes register-reservation descriptors: `(I1=1,
length=9, offset=2)` for 15720 and `(I1=1, length=6, offset=2)` for 158D8, with
`(I1=3, length=4, offset=44)` in both. These are recorded as input metadata;
they are not presented as a live/free-register snapshot.

Diagnostic `ugen -d` was tested only for observation validation. It changed the
generated object on 15720 and is excluded from code-generation evidence. Do not
use its register messages as though they describe a byte-inert accepted build.

## Live-state limit

A hardware-breakpoint-only observer was prepared for this pinned host binary;
it did not patch compiler files, executable bytes, emulated memory, or general
registers. Its child was rejected at `PTRACE_TRACEME` with `EPERM`, before
`ugen` executed. No live/free snapshot was obtained, no breakpoint was installed,
and that route was neither retried nor elevated. The unavailable snapshot is not
needed to determine the proven fallback condition, which has no availability
test.

## Valid next gate

The earlier reopen gate should no longer ask for a free-list change that makes
this stock 5.3 `Umov` choose two ordinary carriers. That proposed lever does not
exist on this path. A genuine eight-byte table record cannot acquire a large
peeled displacement through fabricated padding, shifted bases, or an invented
object layout.

A different authentic source construction could still lower to independent
scalar load/store operations instead of this `Umov`; that is a possibility,
not an established reconstruction. The recorded 14 insertion and 16 removal
controls already tried word/member copies, copy cursors, typed and aligned
records, and aggregate variants without success. They must not simply be rerun.
Parameter-copy lowering also does not justify changing the proven global-table
destination or the actual ABI. There is no new source construction supported by
this audit.

The next justified input is either:

1. Original or independently established record/copy source showing a genuinely
   different lowering operation while preserving the native ABI, eight-byte
   geometry, packed fields, traversal direction, and valid aliasing domain; or
2. Compiler provenance for this translation unit, with an explicitly authorized
   and pinned comparative compiler investigation. The observed 7.1 difference
   alone does not establish which compiler built the native function or that
   another version will match. Any such comparison would remain research and
   would not change the accepted compiler or acceptance gates.

Until one of those inputs exists, retain COMPLETE-NONMATCH. First-copy residuals
remain 15720+E0 and 158D8+E8; the removal source's two earlier branch-orientation
differences remain separate. No new C variants are justified by register
availability or alignment alone.

## Reproduction and publication boundary

The companion `receipt.json` binds source hashes, compiler hashes, protected
target-manifest/scorer hashes, metadata, section hashes, and unchanged scores.
To reproduce, capture the approved driver's `-v` commands on each exact retained
source, preserve its front-end Ucode and symbol inputs, replay the same commands,
and add only `-e TREEFILE -l LISTFILE` to `ugen`. Compare final objects in full
before trusting diagnostic content. Decode UGEN's positional input with the
existing workbench Ucode parser, not its binasm-shaped `-temp` output.

All repository checkouts, accepted compilers, targets, scorer, protection, layout,
and source files remain unchanged. Raw compiler/native listings, Ucode, binaries,
objects, and the unsuccessful observer are private local material and are not
part of this deliverable. This packet contains only this analysis and hash/metadata
receipt; it contains no ROM data or raw assembly dumps.

Primary sources:

- [Pinned original IDO 5.3 ugen](https://github.com/decompals/ido-static-recomp/blob/9c242adc890beef098020149d9554f48208f699d/ido/5.3/usr/lib/ugen)
- [Pinned static recompiler](https://github.com/decompals/ido-static-recomp/tree/9c242adc890beef098020149d9554f48208f699d)
- [Version-limited 7.1 eval_mov source](https://github.com/decompals/ido-matching-decomp/blob/4e9bd753d197e9f593475aab1e3c46701015bdfa/src/ugen/eval.p#L1818-L2046)
- [Register-manager availability source](https://github.com/decompals/ido-matching-decomp/blob/4e9bd753d197e9f593475aab1e3c46701015bdfa/src/ugen/reg_mgr.p#L590-L596)
