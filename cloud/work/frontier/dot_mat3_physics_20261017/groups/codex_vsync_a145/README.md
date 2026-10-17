# Settings/race reset: complete codex_vsync_a145 extension

`func_800DE45C.c` supplies the 1,020-byte reset routine at 0x800DE45C.
All seven members of this group strictly match with stock IDO 5.3. The six
previous members are preserved, not new matching credit. The new candidate
credit is 1,020 bytes; accepted image/ROM credit is zero. Whole-unit, image,
and ROM gates remain the independent coordinator's responsibility.

Install this packet as an extension of the existing group named
`codex_vsync_a145`, retaining that name and all six old members. `group.c` and
`func_800D6530.c` are byte-identical copies of the existing group files at
master 58425072b49e95f0fbe5c4c239988fb4885f7c5c. The new source must come last
in the source list; earlier placement leaves a deleted-helper stub after an
old member. The complete members and claims lists contain all seven exact
bodies. The private-ABI `speed_set` retains its genuine callers and is not
kept external. No canonical source, lock, assembler target, or override is
changed by this packet.

## Evidence and source hypotheses

This is a native N64 reconstruction following the MAT3-informed approach to
source boundaries, variable identity, and first references. No authenticated
MAT3 identity or donor body has been established for this particular routine.
The historical w27d draft was 220/255 words in its old whole-program context;
a fresh genuine-callee closure was 237/255 before these changes.

- A distinct scoped `j` initializes the four player records. Reusing the
  function's `i` prevents the native fully unrolled form. The final byte-clear
  loop reuses `i`, recovering the 41-option loop's native comparison and the
  delayed 56-byte frame teardown.
- `input_rec0` is a four-element, 76-byte player-record array. Its definition
  makes object bounds visible to as1 and recovers the native address
  coalescing. Existing source views already use this canonical symbol as a
  76-byte-stride array. The second definition is the existing four-byte
  per-player state array `D_80146180`.
- Defining those objects in this translation unit is an inferred native
  declaration-ownership hypothesis. Their compiled `.bss` object offsets
  are 0 and 304, but their existing image addresses are 0x8014A118 and
  0x80146180. **Object-file adjacency is not a proposed image layout.**
  Both are `SHT_NOBITS`, with sizes 304 and 4. No existing non-extern
  definition of either symbol was found under `src` at the base revision.
  The named global relocation path binds each to its established image
  address and returns only function bytes. It does not add duplicate
  production storage. See `blob_group.relocate` and the named-defined-global
  regression test, and `blob_splice.link_function`'s explicit absolute
  rebinding of defined data. A future conventional whole-program linker
  must not allocate this `.bss` block as a contiguous replacement.
- `reset_player_counts` contains the real final two stores setting the
  player/window counts to one. Its original boundary is an explicit helper
  hypothesis, not recovered donor evidence. It fully inlines; the object
  has exactly the seven expected emitted function symbols. Moving the
  stores directly into the root changes two words. The adjacent native
  eight-byte empty-return stub at 0x800DE454 is consistent with deleted
  helpers, but does not identify this helper; that stub is not claimed.
- The final loop body is on its own physical line. Joining it to the loop
  header changes four words. This statement-line scheduling dependency is
  intentional and disclosed.

There are no fake prototypes, artificial callers, inline assembly, added
volatile accesses, unused local frame fillers, or padding objects. Unknown
bytes inside the two observed record layouts represent actual fields not
accessed here. Host assertions check the field offsets and extents.

## Replay and controls

With stock `IDO_DIR` and MIPS binutils on `PATH`:

```
python3 tools/cloud/score.py group cloud/work/frontier/dot_mat3_physics_20261017/groups/codex_vsync_a145
python3 tools/cloud/score.py group cloud/work/frontier/dot_mat3_physics_20261017/groups/codex_vsync_a145 --claims
python3 cloud/work/frontier/dot_mat3_physics_20261017/groups/codex_vsync_a145/replay.py --check
python3 -m pytest -q tests/cloud/test_mat3_race_reset.py
```

Both fresh positive compiles match every member. Own literals are verified
for `continue_prompt` (0x80124280..0x80124284), `func_800D6530`
(0x801241AC..0x801241B0), and the root (0x80124308..0x80124310).

The six controls fail the root while preserving the six old members:
wrong option count, 1/255 differing; external player array, 240/255 plus two
extra words; external tail array, 17/255 plus three extra words; inlined
count stores, 2/255; shared initial counter, 251/255; joined final loop
statement, 4/255. The receipt records actual difference counts and extra
words separately. `semantics.c` runs 256 bounded host cases under UBSan,
covering every signed option-byte value, all stores, unchanged player-record
bytes, and the ordered external calls.

The protocol-v2 receipt hashes source/recipe/replay/semantics inputs, relevant
target words and extents, and referenced addresses. It does not pin compiler
tools, locks, or assembler text. The object layout report distinguishes
compiler storage metadata from native addresses. A separate cloud worker
independently replayed the complete group, checked the unchanged sources and
known-data rebinding path, and reproduced the helper/extern controls before
publication. The independent whole-unit/image/ROM gates have not run.
