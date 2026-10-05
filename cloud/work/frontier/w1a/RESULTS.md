# Frontier wave 1, agent w1a - results (2026-10-04)

Builder scratch: `watchman2:~/rush2049/scratch/frontier/w1a`. All groups are `-g0 -O3 -mips2 -G 0 -non_shared`,
real bodies only (no stand-in callers or callees). Nothing spliced, committed, or edited outside this directory.

Scorer command for every group (run from the scratch copy, `IDO_DIR` = toolkit 796ae99a...):
`python3 tools/cloud/score.py group groups/<group> --claims` (local wrapper: `./scg.sh groups/<group> <group>`;
full member output saved in each group's `SCORE.txt`).

| # | Function | Bytes | State | Where |
|---|---|---:|---|---|
| 1 | `func_800878E0` | 296 | **MATCH** (group) | `groups/gfx_modes` |
| 2 | `audio_dma_sync` | 124 | **MATCH** (group) | `groups/audio_heap` |
| 3 | `audio_task_complete` | 416 | **MATCH** (group) | `groups/audio_heap` |
| 4 | `sound_update_channel` | 488 | **MATCH** (group) | `groups/slot_sound` |
| 5 | `display_list_alloc` | 92 | **MATCH** (group) | `groups/slot_sound` |
| 6 | `func_8008705C` | 180 | **MATCH** (group) | `groups/gfx_modes` |
| 7 | `entity_spawn_callback` | 416 | 9/104 words off (single, -O3 = -O2) | `entity_spawn_callback/best.c`, `NOTES.md` |
| + | `func_800E7C2C` | 216 | **MATCH** (group, not assigned) | `groups/audio_heap` |
| + | `slot_value_get` | 60 | **MATCH** (group, not assigned) | `groups/slot_sound` |
| + | `player_state_get` | 60 | **MATCH** (group, not assigned) | `groups/slot_sound` |
| + | `slot_deactivate` | 88 | **MATCH** (group, not assigned) | `groups/slot_sound` |

Strict claims: 10 functions, 2,020 bytes (6 assigned = 1,596 bytes; 4 extra = 424 bytes).

## groups/gfx_modes - claims `func_8008705C`, `func_800878E0`

Output of `score.py group groups/gfx_modes --claims`:
```
Members:
func_8008705C:
  MATCH
func_800878E0:
  MATCH

Context (informational; excluded from exit status):
func_80086A50:
  MATCH (2 section-relative relocations unverified: .rodata+0x0 at +0xc, .rodata+0x0 at +0x14)
```
- Members `func_80086A50` (internal), `func_8008705C`, `func_800878E0` (both in `keep`).
- **The partner had to be rewritten, not reused.** With the locked `src/blob/groups/func_80086A50/f.c`
  (hand-pushed `g = D_80149438; ... { u32 x = w1; g->w0 = ...; g->w1 = x; }`) the callers are 3/45 and 19/74
  off: they keep `mask`/`&D_8014A248` in a2/a3 where retail uses t0/t1. Rewriting the callee with SDK GBI
  macros (`gDPSetCycleType`, `gDPSetRenderMode`-shaped `gSPSetOtherMode`, `gDPSetCombine`, `gDPSetEnvColor`
  on `D_80149438++`) leaves its 387 words identical and fixes both callers: each macro's block-scoped `_g`
  is coloured (a2, a3), so the callee's IPA register summary contains a2/a3 although no emitted instruction
  uses them. The callers also need the macros (word order and `ori` order come out right with no tricks).
- `func_80086A50` stays "own jump table unverified" as before (workstream B); it is already locked. To splice
  the two claims the locked source of `func_80086A50` has to be replaced by this one (same bytes).
- Recovered: `D_8012E608` u32 render-state bits (bit 0 alpha-compare threshold, bit 4 prim-depth source,
  bit 5 selects the render-mode variant, bit 14 "combiner overridden"), `D_8014A248` s32 current render mode
  (0..4, -1 = invalid), `D_80149438` `Gfx *` display-list head.

## groups/audio_heap - claims `audio_dma_sync`, `audio_task_complete`, `func_800E7C2C`

Output of `score.py group groups/audio_heap --claims`:
```
Members:
audio_dma_sync:
  MATCH
audio_task_complete:
  MATCH
func_800E7C2C:
  MATCH

Context (informational; excluded from exit status):
audio_helper:
  MATCH
func_800E7B44:
  MATCH
func_800E7D0C:
  15/49 words differ (1 extra words (nonzero beyond target length))
```
- This is the game heap (names are historical): `audio_dma_sync(heap, size)` = locked alloc,
  `audio_task_complete(heap, size)` = locked alloc through a handle slot, `func_800E7C2C` = create sub-heap.
- Two source facts closed the "extra `move a2,v0`" problem that the older notes list as open:
  1. **`audio_helper`'s parameter order is `(heap, size, owner, tag)`**, not `(size, heap, ...)`. IPA colours
     `size`->a0 and `heap`->a2 either way, so the callee's own words do not tell; the callers' argument
     evaluation order does (`move a2,s2` before `li a0,12` / `lw a0,52(sp)`).
  2. **`heap ? heap : D_801527C8` is a deleted static with two `return`s** (`heap_or_default`), inlined at its
     three call sites. Its inlined result temp is the v0 that retail copies into a2 (or s2). A ternary, an
     `if` assignment, or a one-`return` static is coloured straight into a2. The caller-less `jr ra` stub
     `func_80097468` sits exactly where this static would be (between `audio_helper` and `audio_dma_sync`).
- `audio_task_complete`: locals declared `h, slot, t, i` for the 48-byte frame (order quirk).
- `audio_helper` and `func_800E7B44` are already locked; their source changes (signature) and both still MATCH.
- `func_800E7D0C` (context, not assigned): with `extern volatile s8 D_80116488` the flag access matches
  (`lui/lb`, `lui at/sb`) and 3 scheduling words remain (`lui v1` for `&D_801527C8` is emitted two
  instructions early). Not claimed; `volatile` variant not kept in the group.
- Layouts are in the file header (`Block` 0x20, `HandleTable` 0xC, `Heap`).

## groups/slot_sound - claims `display_list_alloc`, `sound_update_channel`, `slot_value_get`, `player_state_get`, `slot_deactivate`

Output of `score.py group groups/slot_sound --claims`:
```
Members:
display_list_alloc:
  MATCH
sound_update_channel:
  MATCH
slot_value_get:
  MATCH
player_state_get:
  MATCH
slot_deactivate:
  MATCH

Context (informational; excluded from exit status):
func_80096288:
  MATCH
mode_byte2_set:
  MATCH
object_type_byte2_get:
  MATCH
object_type_byte3_get:
  MATCH
mode_byte_set:
  MATCH
```
- One unit because the pieces depend on each other: `func_80096288` must be internal with several call sites
  (with one it is inlined and deleted), and `sound_update_channel` contains an inlined `slot_value_get`.
- `func_80096288` = locked body + dead `if (0) { switch ... }` (inline blocker, quirk). Internal: MATCH and
  all four slot views MATCH. In `keep`: the callers are 10-18 words off. So retail has it internal.
- `display_list_alloc` / `slot_deactivate`: final store spelled `do { slot->active = N; } while (0);`
  (quirk; keeps the store out of the `jr ra` delay slot; a trailing `return;`, an inlined setter or an
  `if` do not).
- `sound_update_channel` (internal, `force` in t0; the four locked callers are the real partners):
  the prior context body was 118/122 off. What fixed it: (a) `slot_value_get(D_80149780)` inlined instead
  of an open-coded call + table read; (b) one `u8 *p` for bank pointer, entry table and running data
  pointer; (c) indexed array loops (`D_80149878[i] = -1`, `D_80149820[i] = D_80151AE8[D_801497A4].first +
  i * 36`) instead of pointer walks. No quirks.
- Recovered: `ResSlot` (0x14: `s8 state @1`, `u8 active @2`, `void *data @0xC`) at `D_80156D38`
  (`D_80156D44` is `D_80156D38[0].data`, not a separate table); `Bank` header (mode @1, bytes @2/3/4/8,
  flag @0xA, counts @0xB/0xC, 12-byte entries `{u8 *ptr; u8 id;}` from +0x10); `D_80151AE8` 8-byte pairs.

## entity_spawn_callback - 9/104, not a match

`score.py fn cand/entity_spawn_callback.c entity_spawn_callback --flags "-g0 -O3 -mips2 -G 0 -non_shared"`:
```
entity_spawn_callback:
    +0x0dc  want 3c028016 lui v0,0x8016                 got 3c198016 lui t9,0x8016
    +0x0e0  want 2442b254 addiu v0,v0,-19884            got 8739b254 lh t9,-19884(t9)
    +0x0e4  want 84590000 lh t9,0(v0)                   got 87b80022 lh t8,34(sp)
    +0x0e8  want 87b80022 lh t8,34(sp)                  got 87a40022 lh a0,34(sp)
    +0x0ec  want 57190005 bnel t8,t9,0x18               got 17190005 bne t8,t9,0x18
    +0x0f0  want 87a40022 lh a0,34(sp)                  got 00000000 nop
    +0x0f8  want 1000000e b 0x3c                        got 3c018016 lui at,0x8016
    +0x0fc  want a44a0000 sh t2,0(v0)                   got 1000000d b 0x38
    +0x100  want 87a40022 lh a0,34(sp)                  got a42ab254 sh t2,-19884(at)
  9/104 words differ
```
Plain ABI single function (not a group case). Semantics, levers, residual lane (uopt address web for
`&D_8015B254` + a coloured two-block fragment of `idx`), what was tried (~80 variants) and the next hypothesis
are in `entity_spawn_callback/NOTES.md`. Workbench `diagnose` was **not** run on it (a deviation from the brief): I worked from the aligned diff
(`fg.sh`) and the workbench law L55/L56 text. Run it before continuing this function.

## What generalises

1. **A matched callee's source is not settled until its callers match.** Twice the partner bytes were
   already locked but the source was wrong in a way only IPA callers can see: GBI macros vs hand pushes
   (register summary), and parameter order (argument evaluation order). "Reuse matched partners unchanged"
   should be "reuse, and suspect them first when the caller's residual is which register survives a call
   or the order arguments are set up".
2. **GBI macros change IPA facts, not only word order.** Every `_g` is a coloured web. A display-list
   function written with hand pushes can match itself and still poison its callers.
3. **Deleted statics explain "extra move" residuals.** An inlined static's result is its own web (v0-first);
   the shape of the static matters (two `return`s, not a ternary). Check the caller-less `jr ra` stubs next
   door before searching: `func_80097468` was the tell here.
4. **Internal callees with few visible call sites vanish.** `func_80096288` is inlined and deleted unless
   the group holds two or more of its real callers; a two-statement kept function (`slot_value_get`) is
   inlined into its callers and still emitted. Build the unit around the tiny shared callee.
5. **Pointer walks vs indexed loops decide global reloads** (`*q++ = ..` forces a reload of every global
   pointer after the loop; `arr[i] = ..` does not).
6. **`do { } while (0)` and `if (0) {}` are real levers**: the first keeps a final store out of the `jr ra`
   delay slot; either one changes uopt's procedure-wide allocation (kept an s16 parameter in its home slot
   in `entity_spawn_callback`). They look like compiled-out debug macros in the original.
7. Tooling: `agentC/full.py` crashes when the candidate contains zero words (objdump collapses them);
   `w1a/full.py` adds `-z`. `w1a/gvar.py` / `var.py` score marker-delimited variants of a group member
   in one line each; `w1a/nbr.py ADDR` lists address neighbours with lock state (finds stubs quickly).
8. Frontier tool: `entity_spawn_callback` is flagged `group` (preserved a1/a2) but is plain ABI; the
   detector seems to count `li a1,1; li a2,1` placed before a `bltzl` as registers surviving a call.
