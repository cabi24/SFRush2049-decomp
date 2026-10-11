# w20g RESULTS: entity_spawn_init (0x8008EA10, 5544 B, 1386 words, -O3)

| function | bytes | state | flags | scorer output (Pi, `python3 -m tools.conveyor.pipeline.blob_unit --tag w20g score entity_spawn_init --with cloud/work/frontier/w20g/entity_spawn_init/best.c --neighbours`) |
|---|---:|---|---|---|
| entity_spawn_init | 5544 | NONMATCH, **7 words** (was 61); frame 728 = retail, size 1386 = retail; the only differences are two spill-temp homes | -g0 -O3 -mips2 -G 0 -non_shared | `FAIL entity_spawn_init: 7 of 1386 words differ` / `umerge inlined into it: func_8008B2E4` / `locked bodies that differ in this unit: 0` |

There is nothing to integrate: no match, no group, no cloud/matches file, no unit_overrides and no data-symbol changes.
`entity_spawn_init/best.c` is w17f's `base_alias.c` plus one lever (below). Stop: the function top is solved. The
residual is two spill homes, and I traced them to a structural question that variants cannot settle (section 2).
Time used: about 2 h.

## 1. The lever (61 to 7 words; no shaping device)
Six of the eight `switch (type)` cases (0, 1, 2, 3, 6, 7) toggle a state bit. They are now written
```c
case 0:
    if (D_801392D8[slot] & 1) { D_801392D8[slot] &= ~1; break; }
    D_801392D8[slot] |= 1; v->flags |= 1;
    break;
```
They used to be `if (bit) clear; else { set; flags |= 1; } break;`. The machine code is the same, because the
then-arm's `b` to the join block was already chained to the switch end. But each `else` form leaves an empty uopt
join block that holds only the `break` jump (blocks 90, 94, 98, 102, 135 and 139 in `tools/bmap.py` output). The
model address web and the constant 5 are both live in those blocks.

uopt's priority function (w17f): save = tot / nocs, with nocs = 2 + ((n - 2) >> 2).

| web | n before (base_alias) | save before | n after (6 fewer blocks) | save after |
|---|---:|---:|---:|---:|
| model address | 186 | 32/48 = 0.6667 | 180 | 32/46 = 0.6957 |
| constant 5 | 175 | 30/45 = 0.6667 | 169 | 30/43 = 0.6977 |

Before the change the two webs tied, and the model web won the tie on the lower web number. Now the constant 5 is
decided first, and the model's top piece is coloured t1, exactly as w17f's `PRIO=10:0.999` oracle predicted.
- Measured with `tools/sv.sh` (SAVELOG) on `v/k6.c`.
- Unit result for base_alias with this change: 653 to 7 words.
- Removing 2, 3, 4, 6, 7 or 8 shared blocks flips the order; removing 1 or 5 does not.

Side effect: the change also frees inline budget. k6 plus four compiled-out `if (fast) {}` still inlines all 19
func_8008B2E4 sites (eight does not). Before, base_alias allowed about one.

The surface alias (`*(s16 *)&surface == 8`) is still needed. Without it, `v/n1.c` is 493 words off. It stays
disclosed in the header as a shaping device.

## 2. The remaining 7 words: spill homes (traced, not solved)
Retail keeps the model address home at 92 and the switch's type copy (web 263) at 84. Ours puts them at 96 and 88.

New tool `tools/homesim.py` simulates f_spilltemps from the SAVELOG live-block sets of each web: homes are given
first-fit in bit order, and two webs conflict when they share a block. It reproduces our homes exactly. Its
conclusions, given that the frame must stay 728 (frame = align8(area + homes + 80)):

- **A: the named area stays 624.** This needs one more coloured expression web X. X must be created before the
  model address (lower bit) and must be live in block 57 or 58 (the `D_8014978C >= 0 && < 6` test), or across it.
  - Any contiguous live range that covers block 57 or 58 works; no other singleton block does.
  - Moving any existing home web earlier (a dead early initialiser) does not work. Probes `v/d1.c` and `v/d3.c`
    confirm it: model goes to 92 but the type copy stays at 88 (w17f found the same with its 10 candidates).
  - Retail has no unexplained register, so X would have to add no code. I found no candidate.
- **B: the named area grows to 632 (8 more bytes of locals or inline area) and `car` is not a spill-home web**
  (it stays a variable instead of being copy-propagated). With the existing webs only, every solution needs area
  632, and the 728 frame then forbids car's home.
  - Mini probes (`v/mini/`, single-file -O3 through `snap.sh` + `spill.sh`) tried ten forms: `register`, a
    second identical definition at the loop top or end, a definition inside the loop, `D_80152818 + slot`,
    `car = D_80152818; car += slot;`, statement versus initialiser, an escaping use, an s32 parameter, and others.
  - The only form that keeps `car` a variable is making `slot` address-taken, and that reloads `slot` from the
    stack (retail keeps it in s7). Not a candidate.
- Area 628 would give retail's homes exactly, but the area is 8-granular: one unused local more leaves 624, one
  less gives 616. Inline areas are 16 bytes (func_8008B2E4) and 8 bytes (func_8008E06C), so 628 is unreachable.

Best next hypothesis: B, with car reaching the loop as a non-propagated value. Two candidates:
- an inlined helper's parameter or return value (w15d: an inlined static's return value survives as a variable);
- the whole per-object body being an inlined helper that takes `car`.

No caller-less stub is next to the function, so such a helper would have to be non-static or come from another
file. Check `f_copypropagate` (uopt.trace.c line ~21731) for the exact availability rule before searching more.

## 3. Colour stores are probably inlined func_8008E06C (evidence, not deliverable)
func_8008E06C (locked, 11 words) is `D_8012E700[handle].rgba.word = *color`. If the six colour stores are written
`func_8008E06C(v->handle.word, &colorN)`, the colours become address-taken, which naturally explains why retail
keeps all six colour locals on the stack.
- With func_8008E06C re-spelled in the candidate as `D_8012E700[handle].rgba.word = *color`, its own body stays
  `EQUAL func_8008E06C: 11 words` and locked bodies that differ = 0. The locked spelling `&D_8012E73C + arg0*0x44`
  adds a `lui at` per inlined site.
- Stock umerge then inlines only 17 of the 19 func_8008B2E4 sites. umerge processes sites from the end of the
  source, so the first two rand sites lose.
- `tools/olim.sh` (oracle: rerun uld..as1 with `umerge -space_time 4`) inlines all 25 sites.
  `entity_spawn_init/e06c_probe.c` (pD00..pD37 only, eight fewer unused words, so the frame is 728) then gives
  **29 rows = the same two homes and nothing else**.
- So this structure is code-consistent with retail, and it replaces 8 words of disclosed filler with real inline
  area, but it does not fix the homes.
- umerge's budget is controlled by `-space_time` (default between 3 and 4; `-Olimit` and `-growth_limit` have no
  effect). If the budget is relative to whole-program size, the shadow unit (missing about 250 unmatched
  functions) under-budgets compared with retail. That is untested; I did not adopt the form because it cannot
  match in the current unit.

## 4. Frame residual (unchanged from w15e, still disclosed and not proven exact)
pB0..pB2, pC1..pC3 and pD00..pD45 reproduce every retail named-local offset (frame 728). Section 2B would make the
area 8 bytes larger. Section 3 would turn 8 of the pD words into func_8008E06C inline area.

## Arcade ancestor
game/visuals.c StartSmoke (per w15e). It assigns `car = &game_car[slot];` as a statement after the
`surface` initialiser. Changing ours to that order (`v/b1.c`, `v/c7.c`) leaves car copy-propagated: same 7 words,
or worse.

## Tools (lane dir `tools/`, diagnostic only)
- `sv.sh`, `oracle.sh`, `hl.sh`, `ct.sh`, `t.sh`: w17f's tools retagged to w20g. The oracle binary
  `bin/uopt_or` is rebuilt in the w20g builder scratch with w16d patch_spl + w16b splitpatch + w17f prio_patch.
- `bmap.py OR_SPL CT`: block-to-web map built from SAVELOG and ctrace output.
- `homesim.py OR_SPL`: spill-home simulator.
- `olim.sh LABEL "-space_time 4"`: umerge-budget oracle.
- `mdis.sh LABEL`: disassembles a single-file `snap.sh` probe.

## Notes
- Builder scratch: watchman2 `~/rush2049/scratch/frontier/w20g` (copied from base, toolkit installed with
  `--reuse wtk`, oracle snapshots `ol_*`). Variants are in `entity_spawn_init/v/` (ignored).
- No permission denials. Nothing was written outside the lane dir, the builder scratch and the session scratchpad.
