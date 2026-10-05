# Frontier agent A: results (2026-10-04)

Scoring was done on `watchman2` in `~/rush2049/scratch/frontier/agentA` (isolated copy), with

```
IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido \
  python3 tools/cloud/score.py fn cand/NAME.c NAME
```

Only the scorer's own `MATCH` is counted. Nothing was committed, spliced or promoted.

| # | function | size | result | flags |
|---|---|---:|---|---|
| 1 | `display_list_traverse` | 468 | **MATCH** | `-g0 -O2 -mips2 -G 0 -non_shared` |
| 2 | `entity_cull_check` | 256 | **MATCH** (one shaping quirk) | same |
| 3 | `entity_lod_select` | 716 | 36/179 words, at `-O3` only | `-g0 -O3 ...` |
| 4 | `func_80096C28` | 128 | **MATCH** | `-g0 -O2 ...` |
| 5 | `func_800947F0` | 152 | **MATCH** (one shaping quirk) | same |
| 6 | `func_800B66B0` | 152 | 5/38 words alone; `MATCH` only as IPA-internal `-O3` group member (stand-in callers) | see below |

## Strict matches

Each line is the exact scorer command (prefix above) and its output.

- `display_list_traverse`: `python3 tools/cloud/score.py fn cand/display_list_traverse.c display_list_traverse` -> `display_list_traverse:` / `  MATCH`.
  Source `cloud/matches/display_list_traverse.c`. Natural source, first compile. The prior near-miss
  seed treated the cursor as `s32 *` with byte offsets (m2c), which is why it was 59 words off.
- `entity_cull_check`: `python3 tools/cloud/score.py fn cand/entity_cull_check.c entity_cull_check` -> `entity_cull_check:` / `  MATCH`.
  Source `cloud/matches/entity_cull_check.c`. **Quirk:** the block-local copy `off = base;` in front of
  each rebase expression. It emits no code; it founds the web for `base` before the `gfx->w1` load so
  `base` is coloured `a3` (`move a3,a2`) instead of last (`s0`). An empty `if (base) {}` in the same
  place matches too. Not proven to be the original source.
- `func_80096C28`: `python3 tools/cloud/score.py fn cand/func_80096C28.c func_80096C28` -> `func_80096C28:` / `  MATCH`.
  Source `cloud/matches/func_80096C28.c`. No quirk; the second store is written through
  `table->header->entries` (the header pointer just stored), which supplies the temp-ring pop.
- `func_800947F0`: `python3 tools/cloud/score.py fn cand/func_800947F0.c func_800947F0` -> `func_800947F0:` / `  MATCH`.
  Source `cloud/matches/func_800947F0.c`. **Quirk:** `D_8002EB94` is read as `*(f32 *) (s32) &D_8002EB94`.
  The `(s32)` address laundering makes IDO materialise the address (`lui/addiu`, `lwc1 0(t6)`) and load
  it before `D_80118E30`. A plain `volatile f32` read gets the address form but the wrong load order
  (3 words); a plain non-volatile read gets the order but `lui at`/`%lo` (6 words). `D_80118E30` and
  `D_8017A630` are `volatile f32` (reloaded after every store).

## Non-matches

### `func_800B66B0` (5/38 alone; body proven in a stand-in group)

- Best source: `cloud/work/frontier/agentA/func_800B66B0/best.c`.
  `python3 tools/cloud/score.py fn cand/func_800B66B0.c func_800B66B0` -> `5/38 words differ`
  (`lbu t0`/`addu t1`/`bnez t0`/`lbu t2`/`beqz t2` where the ROM has `t6`/`t7`/`t6`/`t8`/`t8`).
- Residual class: **allocation, temp-ring width.** The ROM's ring is four wide (`t9` wraps to `t6`);
  a single-file compile gives the ten-wide ring. Same wall as `cloud/work/workbench_pilot_W1.md`.
  The frontier tool's "not an IPA member" is wrong for this function: it reads only ABI registers,
  but it was compiled as an IPA-internal `-O3` member.
- Evidence: `func_800B66B0/group_standin/` (same body plus two stand-in callers, callee not in `keep`):
  `python3 tools/cloud/score.py group cand/func_800B66B0/group_standin` -> `Members:` / `func_800B66B0:` / `  MATCH`.
  This is diagnostic only (stand-in callers; not spliceable, not a claim).
- What mattered in the body: `u32 len` (an unsigned index stops IDO strength-reducing `str[len - 1]`,
  which is what leaves the two separate `addu tN,v1,a0`), and `count++; if (...) {` on **one physical
  line** (line-number scheduling; on separate lines `addiu v0` and `addu t7` swap, 2 words).
- Tried: ~120 single-file variants (declaration orders, pointer/index forms, loop forms, element
  types), `-O3` single file (still 5).
- Next hypothesis: build it in a real `-O3` group with its only caller `menu_input_process`
  (non-`keep`); the body should need no further change.

### `entity_lod_select` (36/179 at `-O3`; 108/179 at `-O2`)

- Best source: `cloud/work/frontier/agentA/entity_lod_select/best.c` (generator used for sweeps: `tpl.py`).
  `python3 tools/cloud/score.py fn cand/entity_lod_select.c entity_lod_select --flags "-g0 -O3 -mips2 -G 0 -non_shared"`
  -> `36/179 words differ (2 section-relative relocations unverified: .rodata+0x0 at +0x15c, .rodata+0x0 at +0x164)`.
  At the default `-O2` flags the same source gives `108/179 words differ (2 section-relative relocations unverified: ...)`.
- The function is an `-O3` unit, not `-O2` (frontier tool assumption wrong): only `-O3` keeps `base`/`seg`
  in `t3`/`t2` with home-slot spills round the call, as the ROM does. Compiling it inside the real
  `audio_frame_sync` group gives the same words as single-file `-O3`. Even at zero differing words the
  scorer would report the two jump-table relocations (`0x80123A70`) as unverified, so this can only
  become a lead until the table is owned.
- State: instruction-for-instruction identical (mnemonic diff 0, frame 232, same saved registers,
  same hoisted constants, same jump-table shape). The 36 words are two register residuals:
  1. **Allocation, loop head:** ROM `and v1,…` / `andi a0,t6,0xc0`; candidate `and v0,…` / `andi v1,…`
     (5 words). Something keeps `v0` unavailable in the first block of the loop.
  2. **Temp-ring phase, one slot** from the first rebase onward (`t7,t8,t6` vs `t6,t7,t8`; 31 words).
     The ROM frees the `op` temp (`t6`) after the `op2` temps and before `op2`'s last test, i.e. one
     hidden pop inside the `op == …` chain.
- What moved it (in order): one `switch` over `1, 4, 0xDB..0xE1` with the cases in ROM block order
  (`0xE0, 1, 0xE1, 4, 0xDE, 0xDF`); `was = first != 0; first = 0; if (was)`; an **index** `depth` with
  `while (depth >= 0 && depth < 10)` (IDO strength-reduces it to the pointer and leaves the ROM's
  `sll t6,zero,2`); `-O3`; skip tests as `goto next` (fixes `221`/`222` register order); `type`/`idx`
  locals read from `cmd[0]` directly; `s32 w1` with `(w1 + (u32) base)` (fixes `addu` operand order and
  stops `0x0F000000` being hoisted, because constants are keyed by type); and an **empty
  `if (cmd[0] & 0x00FF0000) {}`** in a case body, which is what makes IDO hoist `0x00FF0000` into `s3`.
- Honest notes: the empty `if`, the unused `pad0/pad1/pad[20]` frame locals and the `(u32)` cast are
  shaping devices; the empty `if` is very likely a real emptied test in the original (its position is
  not pinned: `case 1`, `0xE1`, `0xDF` and `0xDB-0xDD` all give the same words).
- Tried without movement on the two residuals (~900 scripted variants): dead reads of every local and
  parameter at six positions, shared named temporaries for the three `&` results, `op`/`op2` types and
  redundant masks, casts and `!= 0` forms on each test of the chain, `seg < 0` spellings, E0 statement
  orders, dead-read placement/expression grids.
- Next hypothesis: both residuals come from one more emptied statement near the top of the loop body
  that still reaches `ugen` (a phantom pop, lever 15/16 class), most likely an emptied test on `op`
  between `op2 = …` and the `op == 1 || …` chain. `(op2 == 4) != 0` does shift the ring (wrong slot),
  so the ring is reachable from that chain.

## What generalises

- **Display-list opcode idiom** (shared by `entity_cull_check` and `entity_lod_select`): `u8 op = (w0 & 0xFF000000) >> 24;`
  with `u8` locals and the tests written on `(op & 0xC0)` twice. `u8` is what gives constant-first
  compare operands (`beq t2,a2`); an `s32` local gives variable-first. Do not name the masked word.
- **Segment rebase:** `(w1 & 0x0F000000) | ((w1 + base) & 0x00FFFFFF)`; `Gfx`-like `{ u32 w0; u32 w1; }`.
- **Record layouts:** `func_80096C28(Table *t, Header *h)`: `Table { Header *header; s32 count; Entry *entries; }`,
  `Header { u32 entries /* offset, top bit = already relocated */; s32 count; }`, `Entry` is 12 bytes with a
  rebased word at `+4`. `func_800B66B0(u8 *str, s16 max)`: length of a string that is single-byte, or
  two-byte when it starts with `0xFF`; `max < 0` means unlimited.
- **Typed globals:** `D_80118E30`, `D_8017A630` are `volatile f32` here (the shared headers say `s32`/`f32`);
  `D_8002EB94` is `f32` (already `volatile f32` elsewhere).
- **IDO behaviour confirmed on our flags:**
  - An unsigned index (`u32 len`) is not strength-reduced; a signed one is.
  - A `while` loop with a constant-true first test leaves `sll tN,zero,2` for an indexed array; `do/while` folds it.
  - Hoisted constants are keyed by value **and type**: `x & C` with `u32 x` and with `s32 x` are two candidates,
    and a single-use candidate is not hoisted. An empty `if (expr & C) {}` adds a use and no code.
  - Compare operand order is not a spelling: constant-first means the other side is an expression or a
    promoted narrow local.
  - A parameter whose arrival register is taken is coloured last unless its web is founded earlier
    (`off = base;` or an empty `if (base) {}` at the right place).
  - `as1` breaks a load-use pair by pulling an independent `lui at` between them, so the listing order
    is not `ugen`'s evaluation order.
  - `-O3` single file equals the group build for a `keep` function whose callee is opaque.
- **Frontier tool gaps:** it classified two of six as "compile alone at `-O2`" wrongly. A four-wide temp
  ring (`t9 -> t6` with `t0`-`t5` unused by temps) marks an IPA-internal member; caller-saved registers
  spilled to parameter home slots round a call, with constants rematerialised after it, mark an `-O3` unit.

Helpers left in this directory: `sc.sh` (strict score), `dd.sh` + `d.py` (full aligned listing with a
mnemonic-only diff count), `batch.sh`/`grepb.sh` (4-way parallel variant scoring), `gbatch.sh`/`gd.py` (group mode).
