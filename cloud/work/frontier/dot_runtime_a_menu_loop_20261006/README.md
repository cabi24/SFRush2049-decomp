# Image-A nested menu callback: four stack-home words remain

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

**Research only, NONMATCH.** `A:func_803A4340`,
`[0x803A4340,0x803A44EC)`, **428 bytes / 107 words**, remains **4/107**.
The complete candidate has the correct 112-byte frame and differs only in four
accesses to one compiler-cached flag-pointer home: native `sp+76`, candidate
`sp+80`. No matching body, accepted-byte or ROM-coverage gain is claimed.

Base: `cd22879d40b3de443cfde047b86e75e159b6cec6`. The protected image-A extent
identifies a data-reference start and prologue. The dispatch table was not
inspected; callback identity is inferred from that evidence and the ordinary
O32 menu-callback pattern. This address is outside image B's text. The callback
homes an unused word argument, preserves all saved registers it uses, changes
drawing state, traverses the dynamic player count, conditionally draws four
text rows, restores the rendering value, and returns one.

## Source and genuine contracts

The accepted A:803AE51C menu callback is a structural sibling, not an original
translation-unit witness. The arcade checkout at
`845329d7b36f5a384c5625ed9a0aef584ab46139` was searched in `game/select.c`,
`game/xselect.c`, `game/attract.c`, and other game texture-lookup call sites.
No direct callback donor was found. `MB/mb_struct.h` supplies the genuine
`TexDef` ancestry: a 16-byte name followed by unsigned width and height, placing
height at byte 18. The N64 accepted lookup establishes a 36-byte record stride;
this packet does not identify its remaining fields. This is an N64-specific
reconstruction, not a claim to recovered original declarations.

Pinned arcade file SHA-256s:

- `MB/mb_struct.h`: `67abcfa7a765a058925d29e80e3907d67562020fb1069511670e0020b8524f38`
- `game/select.c`: `f8fc98e7efc0fd6ac76731cf16847f3d9e34ca5964e8509d10451fafd88a5452`
- `game/xselect.c`: `ca8cc3476bf0870cdb4502b4621f460a992b499e5be70a7e9d55a6205941596c`
- `game/attract.c`: `31c124091ee8762cb0450ca3ded1890f0a6b9222d3e68343ebd8d015677eb3bd`

The current accepted `cloud/matches/func_800B24EC.c` has SHA-256
`405f7b1f5c31e4e4c19b82d745a778e85c2046903cea6d439d5d0564e925712b`.
It was freshly compiled at ordinary O2 and remains strict MATCH. It witnesses
five ordinary O32 arguments (name, signed-halfword output pointer, signed-byte
low/high bounds, error mode), a record-pointer return that can be null, and
the existing volatile unsigned-byte view of the table count. The callback
reads that count once per lookup. The output index local is a real required
helper argument even though this callback does not subsequently read it.
The candidate requires a successful lookup before dereferencing the record.

Other entry/body bindings are recorded in `verification.json`:

- `render_helper`: one floating-point value;
- `object_create`: word selector, ignored pointer return;
- `dispatch_handler`: word selector;
- `state_utility`: two signed-halfword coordinates and a text pointer;
- `object_bytes_sum_global`: a signed-halfword result in the attainable range
  `[-128,637]` from two unsigned bytes and one signed byte.

**The font-height helper is not globally pure.** Independent inspection caught
an initial producer assumption: its full body calls `sound_update_channel`
twice, and that callee writes renderer/font caches. Both native bodies are
pinned. The scoped source proof requires those writes to be disjoint from
texture records, menu/text/flag/count/mode storage and their pointer chains.
Native code reads the text arguments after this helper; C argument evaluation
can read them before it. The disjoint-storage precondition makes those orders
equivalent within this packet. The harness does not execute or characterize
actual cache writes. It permits other external helper boundaries to change
menu globals and text pointers, and checks their next observation points.

`MenuData` has a metadata pointer at +12 and text-array pointer at +16;
metadata has its unsigned-halfword starting index at +26. These are partial
native views, not a recovered full object type. A 32-bit compilation checks
all field offsets, pointer width and the 36-byte texture extent.

## Bounded source history

The complete initial expression gave 39/107 differences. Splitting out the
consumed vertical coordinate gave 50/107. Giving the texture lookup its own
meaningful record-pointer local produced the full 112-byte frame and 13/107.
The outer skip guard's ordinary `continue` spelling closes the register
allocation residual, reaching the four stack-home operands.

Ten complete earlier source controls are retained under `controls/`; their
full ELF/GNU-relocated bodies are reproducibly described in `controls.json`.
The two final recipe checks make twelve ledger entries:

- Initial expression: 428 bytes, 39 differing positions.
- Named y: 428 bytes, 50 differences.
- Named texture and y: 428 bytes, 13 differences.
- Continue guard: 428 bytes, 4 differences.
- Named texture-height value: 428 bytes, 21 differences.
- Reordered existing locals: 428 bytes, 4 differences, but frame/index-home
  operands differ instead of the final pointer-home residual.
- Swapped pointer/y declarations: 428 bytes, 4 differences.
- Separate y subtraction: 432 bytes, **56** full-body differing positions;
  the scorer's 55 target-span differences plus one extra nonzero word.
- Block-scoped texture pointer: 428 bytes, 4 differences.
- Explicit flag-pointer iteration: 428 bytes, 10 differences.
- Final O2: 428 bytes, 4 differences. Final O1: 396 bytes, 101 positions.

Workbench diagnosis preceded the bounded structure/stack controls. The final
source is just the continue-guard version with redundant braces removed;
no further source search is active. All variables carry real program values.
No filler local, stack padding, fabricated caller/helper/formal, inline
assembly, pressure expression, new volatile shaping, protected change or
compiler-recipe change was introduced. The existing table-count qualifier is
inherited from the accepted helper contract. A new authentic local-lifetime
or original source-context explanation is required before reopening this
specific compiler stack-home residual.

## Verification

IDO 5.3 flags: `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.

- Complete ELF function: **428 bytes**, plus **four separately checked zero
  alignment bytes**; no extra function, owned literal/data/storage or mask.
- All **21 relocations** and **12 unique address bindings** checked, including
  entry, protected image identity, complete extent and native body hash.
  GNU linking of the complete unmodified object agrees with the independent
  ELF reader and project relocation output. The four exact differences are
  at `+0x70`, `+0x7C`, `+0x150`, `+0x164`.
- **85,476 unchanged-source C89 UBSan/bounds traces** agree with a separate
  Python oracle: all 256 skip-byte values, negative/zero/one/multiple counts,
  mode cases, unsigned texture-height boundaries, attainable font-height
  boundaries, signed-byte table-count conversions, and 17 hook positions.
  The test checks complete helper arguments and menu-state snapshots.
- Five compiled wrong-source mutants are rejected by host behavior and native
  scoring: inverted skip, inverted mode, wrong spacing, signed texture height,
  wrong text index. These are negative controls, not candidate variants.
- Six focused tests pass with `REQUIRE_TOOLCHAIN=1`, including exact frozen
  receipt/control replay from an unrelated working directory and fail-closed
  Python optimization checks. No broad repository suite was run.

Full u16 height can make the draw coordinate exceed signed-halfword range.
The proofs explicitly model native low-16-bit sign extension and the observed
IDO/GCC implementation-defined signed narrowing; this is not portable ISO C
for every representable source value. The final unsigned divide has two
compiler-generated negative-correction words that cannot execute after the
unsigned load. A legitimate native-coverage ceiling is **105/107**, not 107.
All 107 words remain part of complete-body comparison.

The independent review and its additional native differential tests are
recorded separately. Production acceptance, integration and merging belong
to the independent checker. No CI watcher is requested.

## Reproduce

From a full repository with configured IDO and MIPS GNU binutils:

```sh
PYTHONDONTWRITEBYTECODE=1 python3 cloud/work/frontier/dot_runtime_a_menu_loop_20261006/verify.py --check
PYTHONDONTWRITEBYTECODE=1 python3 cloud/work/frontier/dot_runtime_a_menu_loop_20261006/verify_controls.py --check
REQUIRE_TOOLCHAIN=1 PYTHONDONTWRITEBYTECODE=1 python3 -m pytest -q -p no:cacheprovider tests/cloud/test_runtime_a_menu_loop_packet.py
```

For source-only checkouts, both verifiers accept `--repo /protected/checkout`
and `--tools-repo /current/tool/checkout`; tests accept
`RUSH_PROTECTED_REPO=/protected/checkout`. The producer tools are unchanged
current-master copies. Private object/native material is generated only under
ignored `build/` paths. Receipts contain hashes, counts and results, never
raw native bytes or instruction dumps. Path-sensitive full-object hashes and
local compiler/linker version strings are separate local provenance.

The producer's valid host fixtures have four nonoverlapping accessible flag
entries, safe text arrays and a successful lookup. They do not prove all game
caller bounds. Hardware, unrestricted pointers, malformed data, asynchronous
writers, concurrency, gameplay, runtime-image construction, compression and
full-ROM identity remain unverified. This packet earns zero new coverage.

## Integration-portable replay (2026-10-06)

`portable_receipt()` compares both saved and fresh evidence after excluding only
explicit historical whole-tree/tool/source-context digests. Packet source and
verifier bindings, selected native bodies and addresses, ELF extents, relocations,
owned data, behavior, and compiler executable identities remain authoritative.
Accepted production context is read from the recorded base commit rather than
the live integrated tree. Tests are deliberately not hashed into receipts.

## Rebase follow-up: IDO discovery and historical receipt refresh

The master follow-up merged at `83f4ae311dfd662565530dbd748941fdad6a47ba`
adds explicit/default IDO discovery checks for cc, cfe, uopt, ugen and as1.
Those checks and their three synthetic discovery cases are retained here together
with the MIPS GNU linker guard; two additional cases verify missing-linker behavior.
The earlier branch refresh changed only historical whole-manifest fingerprints;
all selected helper bodies, extents, addresses and actual target/proof fields were
unchanged. Those redundant fingerprints are omitted by the current schema above.
Full current-tree replay and integration testing remain separate gates.
