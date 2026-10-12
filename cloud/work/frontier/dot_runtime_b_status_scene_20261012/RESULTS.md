# Image B status-scene helper: 532 newly strict-matched candidate bytes

Source-first deliverable: `group.c` and `group.json`, group identity
`runtime_b_hud_scenes_20261006`. Sole member and claim `func_80391650` covers
all 133 words, `[0x80391650,0x80391864)`. Four owned literal bytes at
`0x80394F08` also match. Genuine `func_80391864` and `func_80391B00` supply
complete NONMATCH context; only the actual outer caller `func_80391B00` is kept.
No previously exact or accepted member is re-counted or moved between groups.
No accepted-image or ROM-verified byte gain is claimed. Keep draft; the
independent checker owns installation, cartridge validation and merging.

## Native-led source correction

The current-master lean packet (`cloud/work/lean/runtime_b_hud_scenes_20261006`)
reproduces exactly **19/133 differing words**, a complete 532-byte body and an
88-byte frame. Workbench diagnosis and the stock compiler's UGEN listing locate
three causes:

- Declare the existing four-byte point first, then kind and index. This places
  point at sp+84 and the existing potentially carried kind at sp+80. No storage,
  local or capacity is added; frame size remains 88.
- Evaluate the existing status pointer after selecting the player's flag kind.
  The player-array induction address then precedes the status-array address.
- Use `D_80395CD0[i].uv` at the existing UV use sites. The compiler creates that
  full indexed induction expression before the camera and status-position
  expressions. The first-use order, prologue address setup and loop increments
  now all reproduce native exactly. The matrix address and every call are unchanged.

The stock UGEN listing independently shows the UV address initialization moving
before the camera/position address initializations. This is an expression and
induction-order correction, not a source-line directive or scheduling patch.
The original names and declaration order are not established.

The inherited source declared resource-count byte `D_80140BDC` volatile. This
qualifier is also present in accepted `string_copy_format.c`, but
`include/game_globals.h` explicitly calls it an open declaration quirk. This
packet removes the qualifier and uses an explicit native 32-bit pointer round-trip
for each existing count read: `*(u8 *)(u32)&D_80140BDC`. That reconstructs native
address materialization without imposing volatile accesses. Plain scalar, array
and struct declarations do not recover the sequence. Original spelling is unknown;
the round-trip is explicitly a source-shaping device within the N64 32-bit address
model, not evidence that the address originally appeared this way.

No fake prototype, changed helper, stand-in, forced register, assembly, dummy
local, extra read or buffer-capacity change was introduced. Context retains its
real bodies and all their uncertainties. Removing the old volatile qualifier
changes the broad parent candidate from 564 to 566 differing words; no parent
match is claimed. The sibling remains 130/167 with one nonzero excess word.

## Canonical score and full independent proof

From the repository root:

```
python3 cloud/work/frontier/dot_runtime_b_status_scene_20261012/verify.py --score
```

The wrapper authenticates current-tree image/loader evidence in memory and calls
unchanged `score.py group`, without `--claims` or `--allow-unverified`:

```
Members:
func_80391650:
  MATCH
    own .rodata verified at 0x80394F08..0x80394F0C
```

The two context results remain explicitly informational NONMATCHes. Every exact
defined body is listed as both member and claim. This is a genuine whole-program
private-ABI group, not a standalone cloud/matches submission.

`verify.py` proves:

- Authenticated native member and complete context target rows/extents, selected
  symbol bindings and all eight external helper bodies. Inputs include current
  source, recipe, verifier, layout check, focused test and baseline source/recipe.
  No shared-tool, lock, Makefile or assembly-text digest pins; no historical git show.
- Recognized stock IDO 5.3 with identities of all eight compilation passes. No static
  ROM translation unit is rebuilt; per-TU static-compiler selection is inapplicable.
- Complete ELF STT_FUNC extents for all three emitted bodies: 532, 676 and 2,336
  bytes, contiguous without gaps/overlap. Member ends exactly at the next function.
  Entire text section is 3,552 bytes with eight final zero alignment bytes.
- Independent GNU linkage resolves all 185 text and eight context-table
  relocations in the complete object. All **38 member text relocations**, every
  native member word, complete four-byte owned literal and exact placement match.
  No unresolved names, leftover relocations or unexpected allocated sections.
  MIPS2 identity is checked; the documented EF_MIPS_NOREORDER bit may differ between
  the old volatile baseline and current object. GNU uses explicit `--hash-style=sysv`.
- The remaining 76 rodata bytes and the two context bodies are not match/data
  installation claims. Linking their full object proves the member's bindings;
  it does not claim original contiguous group placement or context identity.
- Native caller census has exactly `80391B00+0x5c`. The real compiled parent also
  calls at +0x5c, passing zero in stack slot zero in the delay slot. The target's
  unused formal and private clobber/save convention are therefore preserved;
  no invented callback argument was introduced. Return one is ignored by the parent.
- Missing owned data and a corrupted literal are rejected. C89 compile-time checks
  under real 32-bit IDO validate pointer/word sizes, all selected record strides,
  point size and accessed status/scene/player offsets.

Deep proof (`verify.py --deep`) additionally compiles and GNU-links the current
lean baseline, independently reproducing all 19 full-word differences. Causal
rollbacks give declaration order 3, status-assignment order 6, indexed UV 10,
and plain non-volatile byte 80 differences. The zero-residual final form uses the
unmodified stock acceptance compiler throughout; diagnostic objects are not used
as acceptance evidence.

## Checks and limits

- Normal and optimized-Python deep replay: pass.
- Canonical whole-group scorer: sole member MATCH with all owned data verified.
- Single focused test `tests/cloud/test_dot_runtime_b_status_scene.py`: pass.
- Missing IDO: clean shared-conftest skip, after checking input bindings.
- Explicit packet test covers this frontier group; ordinary changed-single
  discovery is not its gate.

The helper maps flag bits to a per-player HUD scene kind, replaces stale scene
handles, sets scale/mode fields, rotates/scales its UV transform and projects its
point. Backed arrays, player count 0..4, valid handles and live helper resources
are assumed. On the initial iteration flags must be zero or contain bit 1/2;
otherwise native reads an uninitialized kind. On later iterations another
nonzero flag may retain the preceding kind. No speculative initialization was added.

The genuine sibling kind selector assumes kinds 0..8; other values can use an
uninitialized resource. The parent retains its unexplained initial dead color
stores and broad reconstruction differences. The inherited parent has a 32-byte
name buffer whose general bound is not proved here. These contexts are neither
installation claims nor a gameplay/memory-safety certification.

No host execution of external helpers, complete caller/gameplay execution,
arbitrary invalid-pointer safety, asynchronous mutation, hardware behavior,
original source identity, image installation/compression or full-ROM hash is
claimed. No native bytes, raw assembly, objects, credentials or private data
are included in the publication.
