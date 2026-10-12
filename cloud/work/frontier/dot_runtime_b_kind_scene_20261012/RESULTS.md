# Image-B kind HUD scene: 668 newly strict-matched candidate bytes

## Source-first deliverable and accounting

`group.c` / `group.json` retain the accepted `runtime_b_hud_scenes_20261006`
group identity. Both complete exact bodies are members and claims:

- New `func_80391864`: all 167 words, **668 bytes**, native interval
  `[0x80391864,0x80391B00)`. Its complete **40 owned bytes** are eight float
  literal bytes plus the 32-byte switch table at `[0x80394F0C,0x80394F34)`.
- Inherited accepted `func_80391650`: **532 bytes** and four owned literal
  bytes, excluded from new credit. Its source body is unchanged and input-bound.
- Genuine kept root `func_80391B00` stays complete NONMATCH context, with its
  source body unchanged. It is not selected for installation.

Base master: `84d8a4827e7fed06365846e013351b580593d921`. No dependency on an
unmerged PR or historical checkout. **Zero newly accepted-image or ROM bytes.**
Keep draft; independent checker owns integration and merging.

## Native-led reconstruction

Current-master baseline is **130/167 words differ**, a 676-byte emitted body
versus native 668 bytes, and one nonzero excess word. Both baseline and final
have the native **88-byte frame**. This function maps player kinds 0..7 to
resources 216..223, releases/replaces stale scene handles, applies color and
scale, rotates the scene transform and projects the per-player HUD position.
Kind 8 suppresses creation. No whole-function arcade donor was established.

Workbench diagnosis first separated the real extra loads, induction-order
changes and misplaced existing homes from relocation-display artifacts. Native
code retains two different values: a signed player kind and a promoted status
kind. The former survives the stale-kind comparison but must be refreshed if
the release helper ran. The latter survives the creation guard/switch and is
refreshed after the create/color/model calls before the scale-choice branch.

The source now expresses these as separate, consumed `s32 player_kind` and
`s32 kind` carriers. Both reload at the observed helper mutation boundaries.
Indexed `D_80395C00[i]` accesses replace the redundant status pointer; the
existing point is declared first. These changes recover the native induction
setup, remove the unsigned-short switch reload and post-scale handle reload,
and place the existing point/resource/color homes at sp+84/+72/+68. None of
these variables is unused. The extra real carrier fits the existing frame.

The intermediate 11-word residual was traced with a stock-identical diagnostic
uopt. Player/status webs tied at priority 20, so the earlier player expression
won v0; the separate switch copy remained live against the original field web.
A two-web force-only oracle reduced that to four differences; coalescing the
switch carrier was correctly refused because it still interfered. This ruled
out a simple color-only fix. Explicit lifetime/reload reconstruction removed
the copy naturally; a genuine player-kind carrier recovered the final native
first-use ordering. The final acceptance build uses only unmodified stock IDO.
The optional stock UGEN/UMERGE trace produces a byte-identical complete object.

No arbitrary local/table padding, array resizing, fabricated caller/formal,
helper invention, new volatile, artificial read, inline assembly, compiler
force or shared-tool modification is retained. Original source spelling and
local declaration names are unknown. Inherited source-shaping and context
uncertainties are unchanged from the accepted sibling's packet.

## Complete verification

```
python3 cloud/work/frontier/dot_runtime_b_kind_scene_20261012/verify.py --score
```

Canonical unchanged scorer, without `--claims` or `--allow-unverified`:

```
Members:
func_80391650:
  MATCH
    own .rodata verified at 0x80394F08..0x80394F0C
func_80391864:
  MATCH
    own .rodata verified at 0x80394F0C..0x80394F34
```

Parent remains **566/595 words differ** with the inherited 14 unverified
section-relative sites and one owned-data mismatch. Its full emitted extent is
2,336 versus 2,380 native bytes; no suffix is silently dropped or borrowed.
It receives no matching or installation credit.

The packet's input-bound proof checks:

- All three complete ELF STT_FUNC extents; contiguous 532/668/2,336-byte bodies,
  full 3,536-byte text section, no hidden function or suffix, and exact native
  next boundary for the new member. Unknown allocation and unresolved names
  fail closed. The parent remains a complete, explicitly short NONMATCH body.
- Independent GNU linkage from all **185 original text REL records** and all
  **eight switch-table relocations**. The two exact members have 38 and 44 text
  relocations. Every native word and all **44 owned bytes**, including every
  switch entry and native table address, match. The other 36 rodata bytes belong
  to unclaimed context and are not represented as owned-data matches.
- Native caller census is exactly 91B00+0x54 for new 91864 and +0x5c for inherited
  91650. Both native and compiled complete parents pass zero in stack argument
  slot 0 and save s0–s7/s8 plus f20/f22/f24/f26/f28/f30 before the calls. The
  private caller-owned clobber convention is preserved; neither helper grows a
  fabricated ordinary ABI. Both return one, which the parent ignores.
- The actual current `packet_spec → group_bodies → mismatch` path produces
  exactly these two native-equal member slices and excludes the parent.
  Selective substitution into authenticated image B leaves every byte equal.
  This is disposable, read-only production-path replay, not an installation or
  full production image build.
- Missing owned bytes, a corrupted literal, and a corrupted switch-table entry
  each fail closed. Native IDO C89 compile-time checks cover relevant record
  sizes/offsets and 32-bit pointer/word sizes.
- Complete GNU causal controls preserve 91650: baseline 130 differences/676
  bytes; removing the player carrier 9; old point order 6; pointer UV induction
  34. The last three have complete 668-byte extents. No forced control is used
  for acceptance.
- **26,730 actual-source logical-host cases**, repeated with ASan/UBSan and
  strict aliasing, compare current and baseline bodies. Counts 0..4, all kind
  combinations, present/missing handles, repeated players, every callback stage,
  helper mutations to player/status/handle fields, loop-count change, complete
  scene/player/camera state and full helper event order/arguments are checked.
  Only LeakSanitizer is disabled. Helpers are synchronous doubles, not retail
  execution. The fixture clears snapshot padding before byte comparison.

Receipts bind source/recipe/own verifier/layout/harness/trace/test, selected
native words/extents, exact relevant symbol/helper/caller inputs, current-tree
baseline and all eight recognized stock IDO 5.3 passes. No shared scorer,
pipeline, whole-lock, Makefile or raw assembly-text equality pins. No static
ROM translation unit is rebuilt; static per-TU compiler selection is inapplicable.

## Replay, cheap check and limits

```
python3 cloud/work/frontier/dot_runtime_b_kind_scene_20261012/verify.py --deep
python3 -O cloud/work/frontier/dot_runtime_b_kind_scene_20261012/verify.py --deep
python3 cloud/work/frontier/dot_runtime_b_kind_scene_20261012/trace.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_b_kind_scene.py -q -o addopts=''
```

Normal/-O full proof, stock trace and sanitized host checks pass. The single
collected test passes in 0.81 seconds; deep controls/host traces are opt-in.
Missing IDO skips through shared conftest after checking input bindings.
This frontier group has its own explicit CI test; changed-single discovery is
not its coverage mechanism.

Valid backed arrays, count 0..4, player/status kinds 0..8, valid signed scene
handles and stable synchronous helper contracts are assumed. Native invalid
kinds can retain an uninitialized resource; no speculative default was added.
The inherited 91650 flag-kind precondition and parent name-buffer/dead-color
uncertainties remain. Logical host addresses/layout are not native N64 memory.
Exceptional floats, arbitrary invalid pointers/aliases, asynchronous mutation,
original source identity, hardware/gameplay, full image composition/recompression
and ROM SHA-1 are unproved. No ROM bytes, raw assembly/trace dumps, compiled
objects, credentials, protected edits or unrelated private data are published.
