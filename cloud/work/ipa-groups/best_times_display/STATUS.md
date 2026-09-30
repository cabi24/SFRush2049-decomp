# best_times_display -> 34/56 words aligned (real function, extent 56 words)

## Current status (Rescored 2026-09-30 (Round 2 addendum), master d0891f3.)

**Not a MATCH. Builds; 0/56 words strict; 34/56 words match after alignment, structure 0.889.** Scored with
`python3 cloud/work/ipa-groups/best_times_display/extscore.py --norm cloud/work/ipa-groups/best_times_display`:

```
best_times_display  size 52/56  47/56 words differ (position-based)
```

Blockers: the unfolded `li a0,1; a0*24` induction variable for entries 1..4 and the parameter register
(`s1` vs `s2`). Closure (approximate `closure.py`): real callers `func_800D5E64` and `mode_select_handler` are
absent (stand-ins used); the group matches the caller convention only when the loop is split, which enlarges code.

Real head 0x800D5BB0 (`addiu sp,-24; sw s2,24(sp)`), IPA parameter in `$s2` (an s16 player
index; it re-extends with `sll/sra 16`). Callers: `func_800D5E64` and `mode_select_handler`
(0x800DEF6C: `jal` with `move s2,t7` in the delay slot). INDEX's 1068 insns
(`mode_select_handler`) is the caller. This function resets one 120-byte slot of five 24-byte
entries `{float a; ...; float b @16; s32 c @20}` at `D_80140808[idx]`, calls
`scheduler_recv(D_80140AE0[idx])`, sets `D_80140AE0[idx] = -1`, `D_80140A08[idx] = 0` (s16) and
zeroes floats `D_80140B10/BE0/80142518[idx]`.

Draft: `bt.c`. Verify: `python3 cloud/work/ipa-groups/best_times_display/extscore.py cloud/work/ipa-groups/best_times_display --norm`

Difference (52 vs 56 words, register `s1` vs `s2` for the parameter):
- Retail reaches entries 1..4 with `li a0,1; a0*24 (sll/subu/sll); addu v0,v1,...` i.e. an
  unfolded induction variable, then stores at fixed offsets from that one pointer. My loop
  (`for`, `do/while`, down-counting, unrolled by hand, split in three loops) all constant-fold
  to `addiu v0,v1,24` or produce real loops. No variant tried reproduces the unfolded `a0=1`.
- With three separate loops the parameter lands in `$s2` as in retail but the code is much
  larger, so the current file keeps the single unrolled-looking loop.
- Parameter register `s2`: depends on the caller stand-ins; the real callers use `s2`.
`scheduler_recv` (40 words) is a plain extern.

Blockers: source form of the entry-1..4 initialisation.
