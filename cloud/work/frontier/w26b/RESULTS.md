# w26b — static boot tail (IDO 7.1, trace71): synthHandle, func_80018448, func_80012D18

Builder scratch `watchman2:~/rush2049/scratch/frontier/w26b` (deleted at the end; re-create with base copy + rsync of src/{blob,rom}, include, tools,
asm/us/{blob,boot_tail,nonmatchings}, cloud/work/{boot_tail_promotion,frontier/tools}, locks, symbol files, workbench;
`t71/` and `tgt/` copied from w25f's scratch, read only there). 2 cores. Nothing committed, locked, promoted or spliced.
No permission denials.

| function | bytes | before | after | scorer line |
|---|---|---|---|---|
| func_8001A658 synthHandle | 2812 | `LOCK: true=2770 strict=2770 reloc_blind=83 objdump-dr DIFFERS(319 rows) (749 rows)` (w23b) | **MATCH** (needs 2 reloc_addrs entries + target refresh, and a layout-neutral TU edit) | re-split target `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (749 rows)`; TU `TU: true=0 strict=0 reloc_blind=0 objdump-dr identical (749 rows) RESULT PASS`; current target `LOCK: true=10 strict=10 reloc_blind=1 objdump-dr DIFFERS(3 rows) (749 rows)` |
| func_80018448 | 492 | w25f best | unchanged (traced; reset hypothesis refuted) | `LOCK: true=310 strict=310 reloc_blind=24 objdump-dr DIFFERS(23 rows) (129 rows)` (re-run) |
| func_80012D18 | 3148 | `LOCK: true=12735 strict=14066 reloc_blind=716 objdump-dr DIFFERS(679 rows) (810 rows)` (w23a best, re-run) | true unchanged; strict 13017 with frame fixed (`q4_decl_order.c`) | `LOCK: true=12735 strict=13017 reloc_blind=617 objdump-dr DIFFERS(671 rows) (810 rows)` |

Flags `-g0 -O2 -mips2 -G 0 -non_shared`, toolkit 4586d230.

## synthHandle func_8001A658 (2812 B): MATCH (with two reloc_addrs entries + a layout-neutral TU edit)

`sources/func_8001A658.c` (= `func_8001A658/best.c`), flags `-g0 -O2 -mips2 -G 0 -non_shared`, toolkit 4586d230.
`prove.log` (builder, `tools/prove.sh`):
- vs the current inventory target: `LOCK: true=10 strict=10 reloc_blind=1 objdump-dr DIFFERS(3 rows) (749 rows)` (the
  only difference is the fader end pointer's relocation: retail is split as `D_8004F800`, IDO emits `D_8004F300+0x500`)
- vs the re-split target (`reloc_addrs_w26b.txt` applied, w19b `resplit_target.py`: `2 operands renamed, gate_target OK`):
  `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (749 rows)`
- TU context (edited TU, ctx row, re-split target): `TU: true=0 strict=0 reloc_blind=0 objdump-dr identical (749 rows) RESULT PASS`
- TU edit alone: `lib_1a660: IDENTICAL (2630 lines: .text words+relocs, data sections)`; source spliced into the edited TU
  vs the current TU with re-split GLOBAL_ASM: `lib_1a660: IDENTICAL (2630 lines: .text words+relocs, data sections)`
- `reloc_split check --file reloc_addrs_w26b.txt`: `all 2 reloc_addrs entries encode their ROM words`

Before: w23b best `LOCK: true=2770 strict=2770 reloc_blind=83 objdump-dr DIFFERS(319 rows) (749 rows)`.

### How (trace, then lever; about 2 h)
1. **ucode oracle first** (w25b's x1 `r.O`, `uedit.py` + `urun.sh`; `tools/kc.sh` = cc -K + traced ugen/as1 on the
   reduced TU, `tools/kb.sh` batch). Writing retail's ucode by hand - `sv->midi4A` loaded into a temp (a0) at the top
   of the volume block, each D_8004F2F8 arm `midi = temp` with the compare `(0xFF-reg, temp)`, the third test
   `(0xFF-reg, midi)`, and `span = 0` re-inserted on the `== 0`/midi==0xFF path - matched the whole voice loop
   (61 -> 38 rows, all 38 in the fader loop). So the voice-loop residual was exactly: load PRE of midi, compare
   operand order, and the second span store.
2. **Compare order = operand kind** (w16a's readnxtinst rule: order kept only if op1 is a plain variable). Retail's
   `beq s5,a1` on the midi *variable* means midi is not a plain variable at that point: **`u8 midi`** (the use is a
   cvt). With u8 midi all three compares come out retail's way. The frame then needs exactly 12 named 4-byte slots
   (retail 136): dropping `modulation`, `portamento` (inlined calls) and `smf`, with `aux` declared second, gives
   retail's frame and every home (0x48 midi, 0x58 vrange, 0x68 span).
3. **midi read once in the volume block** (`midi = sv->midi4A;` after `pan = sv->panning38;`): with u8 midi this is
   retail's a0 load + `move a1,a0` per arm (the s32 form of this had made midi a memory variable, w25b m1-m5).
4. **Second span kill**: any store to span at the head of the `== 0` arm keeps the midi==0xFF `span = 0` (probe:
   `span = 1;` -> fader-only residual). The disclosed, semantically neutral form: `if ((span = midi) != 0xFF)`.
   A `span = 0` in the `!= 0` arm (GameCube's `lastSPan = 0`) must NOT be there (49 rows).
5. **Fader loop**: `force.sh p1:w471=s` showed the &D_8004BE90 constant web (one web over both loops) was the cause;
   retail's fader loads are a0 *load webs* hoisted above the volume clamp, which needs the fader stores not to kill
   the D_8004BE90 load: **index the global directly** (`FADER_8001A658[j].f`, a cast view of D_8004F300) instead of
   through the `smf` pointer (WAVE_BRIEF wave-9 rule: a store through a pointer local aliases every global) - 38 -> 5
   rows; `(time >> 8) * delta` (operand order) -> 1 row = the end-pointer relocation name.

### Integration (coordinator; order matters)
1. **reloc_addrs**: append `cloud/work/frontier/w26b/reloc_addrs_w26b.txt` (2 entries, rom 0x1BC04 HI16 / 0x1BC0C LO16,
   `D_8004F300 + 0x500`, was D_8004F800 - same mechanism as func_8001C1D8's entries) to `reloc_addrs.us.txt`;
   `python3 -m tools.conveyor.pipeline.reloc_split check`; `make extract` (only func_8001A658.s's two operands change);
   ROM gate; `python3 cloud/work/boot_tail_promotion/refresh_split_targets.py func_8001A658` (as w23i step C).
2. **TU edit** (layout-neutral, proven alone): `python3 cloud/work/frontier/w26b/tools/tu_edits.py src/rom` from the
   repo root hoists `typedef struct ChannelState {...} ChannelState;` + `extern ChannelState D_8004F300[32];` (the
   func_8001B9F8 block's context) above the func_8001A658 slot. Without it the spliced TU fails with
   `redeclaration of 'ChannelState'`.
3. **Lock** (after step 1's target refresh):
   `python3 -m tools.conveyor.pipeline.lock add cloud/work/frontier/w26b/sources/func_8001A658.c:func_8001A658 --flags "-g0 -O2 -mips2 -G 0 -non_shared" --toolkit 4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557`
4. **Context row**: `cloud/work/frontier/w26b/ctx71_O2.jsonl` (1 row, ok, ido7.1, computed against the unedited tree;
   `--prefix-dedup` drops the hoisted lines).
5. **promote_batch** (dry run first):
```
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w26b/ctx71_O2.jsonl 0x1a660 --only func_8001A658 --prefix-dedup rom/lib_1a660 --dry-run
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w26b/ctx71_O2.jsonl 0x1a660 --only func_8001A658 --prefix-dedup rom/lib_1a660
```
6. No unit_overrides (static), no data-symbol changes, no owned data, nothing superseded. D_8004F800 is no longer
   referenced by this slot (other passthroughs may still use it).

### Disclosures (also in the source header)
- Shaping devices, both reuses of the existing `span` local, semantically neutral: (1) `span = cpitch;
  func_80014A0C(i, span); span = 0;` (w24c/w25b x1; stops store PRE hoisting `span = 0` to the loop top);
  (2) `if ((span = midi) != 0xFF)` in the `D_8004F2F8 == 0` arm (keeps the midi==0xFF `span = 0`).
- `FADER_8001A658` is a macro for `((SynthFader_8001A658 *)D_8004F300)` (the TU declares D_8004F300 as ChannelState;
  later TU code uses the same cast pattern with its MasterFader view).
- No unused locals, pad arrays, implicit int or false prototypes.

## func_80018448: ugen has no free-list save/restore; the second half needs several hidden pops (about 1 h, stopped)

- **w25f's hypothesis refuted.** The traced 7.1 ugen calls only `f_define_label`, `f_jump` and `f_check_no_used` at
  labels/jumps (the full call census for the procedure: no save, restore or reset of the free list; `f_add_to_free_list`
  runs only in `f_init_regs`). The temp ring is one linear FIFO across the if/else arms and the join.
- **Greedy FIFO with renaming** (`tools/greedy_fifo.py UGT 'retail pops'`; maps each of our allocations to retail's
  register so our FREE events free the retail register): rows 1-52 need nothing; line 54 needs 2 hidden pops (t8, t9)
  after line 45; line 59 (`(first << 8) | second`) needs 3 more (t7, t8, t9), line 62 two more, and the join (line 67,
  retail `t8 t7 t9`) another 6. So the second half is not a hidden-pop residual alone: retail frees in a different order
  (different operand evaluation/free order in lines 54-69), not just extra deleted temps.
- ucode probes (`tools/ugrid.sh`): narrowings (cvtl 8/16) after every lod of first/second/change in lines 48-65, singly and
  in pairs: none gives retail's line-54 `addu t2` (24-43 rows). cvtl 16 on the s16 `change` (t0) costs no allocation.
- Best next hypothesis: write retail's free order directly: permute operand order of the `add`/`ixa` records in lines
  54-67 of `r.O` (`uedit.py` del+ins) and replay through `urun.sh`; the commutative operand order of `base + i*40`
  (`addu X,t,s1`) decides which temp is freed first. Then look for the C spelling (`&tracks[i]` vs index order).

## func_80012D18: declaration order fixes the frame, not the score (about 30 min, stopped)

- w23a's `probe_named_position.c` (`position`/`excess` named, `position = pos` in both branches) has retail's cfe locals
  at retail's offsets but a 128-byte frame. Declaring `double pos` before `mainRem`/`loopRem` (`q4_decl_order.c`)
  gives frame 120 and retail's single home at sp+44: `LOCK: true=12735 strict=13017 reloc_blind=617
  objdump-dr DIFFERS(671 rows) (810 rows)` (best `true=12735 strict=14066`). Retail keeps mainRem/loopRem at
  sp+60/62 and cmd at sp+64, which this order does not. `true` is dominated by 7 missing instructions (788 vs 781 rows),
  not by the frame; not a true improvement, best.c stays w23a's.

## What generalises

- **Compare operand order is a type lever.** uopt keeps `(var, const)` only when op1 is a plain variable; a `u8`
  local is read through a cvt, so its compares come out `(const-reg, var)` like a load's. When retail compares a
  register variable with the constant register first, try the narrow type before searching for a load form.
- **Direct global indexing restores load webs in loops.** Writing a struct array through a pointer local (`smf->x =`)
  kills every global load; indexing the global (`ARR[j].x`, or a cast view of it) does not. Here that turned the
  &D_8004BE90 constant web into retail's per-site a0 load webs and freed s3 for the loop end pointer.
- **Write retail's ucode first.** For synthHandle one `uedit.py` run proved the whole voice-loop residual was three
  ucode facts (load-PRE temp, compare order, one store); each then had one C lever. `tools/kc.sh`/`kb.sh` make the
  `cc -K` + traced ugen/as1 loop a batch command.
- **Store PRE kills can be any store.** A dead store of anything to the variable on one path keeps a later
  redundant-looking constant store (here `if ((span = midi) != 0xFF)`); a value store elsewhere (the other arm) breaks it.
- **IDO end-of-array pointers** (`&ARR[N]`) are split by the inventory as the next symbol; expect a reloc_addrs entry
  (true=10 / reloc_blind=1 on the current target, 0 on the re-split one).
- ugen 7.1 has no free-list reset at labels: a ring residual across if/else is free-order, not a reset.

## Files

- `sources/func_8001A658.c` (match, = `func_8001A658/best.c`), `ctx71_O2.jsonl`, `prove.log`, `reloc_addrs_w26b.txt`.
- `tools/`: w25f's set with paths changed to w26b, plus `kc.sh` (builder: reduced TU, `cc -K`, traced ugen+as1 on r.O),
  `kb.sh` (Pi: batch kc.sh), `ugrid.sh` (builder: batch uedit+urun), `greedy_fifo.py`, `tu_edits.py` (lib_1a660
  ChannelState hoist), `prove.sh` (rewritten), and w19b/w21b's `resplit_target.py`/`resplit_asm.py`.
- `func_80012D18/q4_decl_order.c` (frame probe). Variants are under `*/v/` (git-ignored).
