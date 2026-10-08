# EF288 gear-label callback: complete C reconstruction, NONMATCH

## Result and scope

`func_800EF288`, main game image, **0x800EF288–0x800EF5B0**, **808 native
bytes / 202 words**. This packet provides a first complete C reconstruction
and one canonical O3 diagnostic with genuine existing semantic helper context.
It is **NONMATCH**: the callback is **788 compiled bytes**, with **179 complete
word differences** against the 808-byte native extent. No matching submission,
accepted bytes, protected changes, integration or ROM coverage is claimed.

Base: `6b2e9e506fe3d2267a710e41c85af5364ccd00c7`. The base-commit alias,
lock, claim and complete C/H-definition audit is replayed with `git show` and
`git grep`; it does not assert a later live tree remains unlocked. The only
same-address alias at that base is `func_800EF288`. The old
`ollama_analysis/overnight2_decompiled.txt` entry wraps an assembly dump, with
undecoded instructions, in inline asm; it is not a usable C reconstruction.
Earlier session D explicitly marked EF288 unattempted behind its font-selector
context blocker. No existing near-match source was retuned.

## Recovered behavior

- Set the rendering float parameter to 0. Select font 10, 11, or 12 for signed
  player count 1, 2, or any other value, with the actual acquire/select/release
  sequence. Ignore the old selector result; do not fabricate native dead-result
  copies in C.
- Iterate positive player count through 2,056-byte model records. Snapshot the
  signed gear byte at +0x730 before rendering callbacks. Draw only while the
  global enable byte is nonzero, model +0x0A is zero, and the selected 952-byte
  car record's signed byte +0xEF differs from 1. Car index is signed halfword
  model +0x7C6.
- Use the position table's 32-byte player-count row and 8-byte player column.
  X is a full 32-bit word, narrowed to signed 16 bits after minus/plus 4;
  Y is a signed halfword at +6. Draw localized label 204 right-aligned with
  colors 0 and 1 at shadow/main coordinates. Reload the label pointer after
  each width callback. Unsigned arithmetic preserves low-bit subtraction wrap.
- Reload count and coordinates after the label pair. Format gear -1 as R,
  zero as N, and every other signed byte as its low byte plus ASCII '0'.
  Draw the one-character terminated string with colors 0 and 14.
- Reload loop count after callbacks, restore the rendering parameter to -1,
  and return 1. Preserve snapshots across callbacks where native does.

Names are descriptive; neither the original source spelling nor original
record declarations are claimed. Unknown byte arrays are observed record
storage, not artificial stack padding. The two-byte local holds exactly one
character and its terminator. Native frame size is 184 bytes; this build's
frame is 104. No capacity was invented to reproduce that frame.

The callback has an ordinary saved-register entry and homes incoming a0.
Its unused formal `u32 callback_argument` means one uninterpreted 32-bit ABI
slot. Original signedness and pointer meaning are **not** established. No
registered direct JAL caller exists at the base; a callback-valued data entry
is present at 0x801150AC. Address-taking is compatible with the ordinary entry,
but does not establish the complete original callback type.

## Honest helper context and ABI limit

`context.c` is the exact clean C104 semantic context already archived at
`cloud/work/dot_selector_d9058_20261006/context.c`, except that the unchanged
canonical O3 header is moved to line 1. It includes the selector, bank refresh,
byte-9 setter, actual object-create and world-trigger wrappers, and minimal
empty native hook. It does **not** import the accepted slot_sound context's
non-original dead-switch/empty-read inline blocker.

This is a complete semantic context hypothesis, not proof of original TU,
visibility, inlining or private register composition. Its keep list preserves
those existing genuine roots plus this address-taken callback. The native
selector input is **s2**; the compiled context input is **s1**. The two caller
executions use explicitly different hook conventions. Passing the compiled
caller into the native helper unchanged is **not** asserted correct.

All seven emitted functions' complete ELF STT_FUNC extents are measured,
including extra and missing words. The complete object text is 1,936 bytes;
12 bytes outside function extents are independently checked zero alignment.
All 139 text relocation records are checked. Owned data/literal/storage bytes
are zero, and any newly introduced owned data fails closed.

GNU ld links the unmodified group at a real contiguous text placement of
0x80000000. Its entire emitted function contents agree with the project
resolver using those same addresses. A separate original-entry map produces
the native comparison. This is not a native-spliced group or ROM-link proof.
No instruction, relocation or ELF symbol metadata is edited for that witness.

Canonical source flags are exactly:

    -g0 -O3 -mips2 -G 0 -non_shared

The unchanged group build automatically adds as1 `-r4300_mul`, recorded as
actual provenance. No raw-cc substitute, backend changes, flag searches,
permutations, fake parameters, pressure locals, dead reads, ungrounded volatile,
asm, or frame padding were used. Compilation is frozen after this baseline.
Further matching needs authentic private helper/TU evidence.

## Verification and portable replay

The 2,257 owner cases compare host C, every protected native instruction, and
the complete relocated compiled callback under their explicit helper hooks.
All 202 native words are visited. Cases cover no-player counts, all font
branches, each gate, signed gear edges, narrowed coordinate/width arithmetic,
and callbacks changing count, label entry, positions, or gear. The redundant
native inner bound's impossible outcome is not claimed as reachable.

The independently authored review is archived under `review/`. It reports 7,281
additional unchanged-host-C/native cases and 10,000 ASan/UBSan cases, including
outer label-table replacement and distinct callback mutation points. It reused
this interpreter after auditing it; it is not represented as a second native
engine. Its negative controls cover gear snapshot, post-width label reload,
coordinate-row reload, unsigned gear, cached count, selector ABI, unknown
opcode, and truncated extent.

The host's GearAssets pointer offsets naturally differ from the 32-bit target;
its fixture uses host pointers. The native interpreter separately enforces
MIPS +4 pointer-field access and target widths. Tests use valid four-player
model/car/table storage and assert actual record extents. They do not prove
behavior for invalid pointers, out-of-bounds car indices, or arbitrary corrupted
player counts. External queue, selector, bank loader, font and renderer bodies
are contract hooks; their execution and asynchronous concurrency are not proved.

From a complete repository checkout with the base commit in history:

    python3 cloud/work/frontier/dot_gear_label_ef288_20261006/verify.py
    python3 -m pytest tests/cloud/test_gear_label_ef288.py -q

Set TMPDIR to a writable workspace directory where system /tmp is constrained.
IDO_DIR may select the pinned installed IDO. Compiler tests skip cleanly if
IDO cc or the MIPS GNU linker is missing. RUSH_GIT_REPO is optional and only
needed for a source-only local workspace whose Git history lives elsewhere.
`RUSH_REFERENCE_ROOT` takes precedence when set. `RUSH_TOOL_ROOT` selects a
separate unchanged canonical-tool/target root; both default to this repository.

The receipt binds only packet sources/verifier and native target words. The
test file is not hashed. Shared manifests, scorer, symbols, locks and production
sources are not pinned to their live hashes. Context is read at the recorded
base. Fresh compiler evidence compares complete words, extents, relocations,
own-data and behavior. Raw native dumps, ROM bytes and object files are not
packet deliverables. Focused checks are not the full integration suite.

Focused validation on the final packet: **7 passed** with IDO/GNU available;
**3 passed, 4 skipped** with IDO absent. The negative ELF tests reject shortened
function extents, unsupported relocations, and newly introduced owned data.
Return-value and private-selector-ABI behavioral mutations also diverge as
expected. These are source/contract diagnostics, not gameplay validation.
