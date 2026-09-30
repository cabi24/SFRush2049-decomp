# catchup_logic  ->  really `func_801084D4` (unregistered head)

## Current status (rescored 2026-09-30, master d0891f3)

**Not a MATCH. Builds; 0/375 words strict; 198/375 words match after alignment, structure 0.783**
(the first-draft figure 159/375, 0.715 is superseded). Scored with `python3 cloud/work/ipa-groups/catchup_logic/extscore.py --norm cloud/work/ipa-groups/catchup_logic`:

```
func_801084D4  size 363/375  326/375 words differ (position-based)  exact after alignment 198/375
context (not counted): slot_state_setup 18/58, object_byte9_set 5/16, sound_update_channel 116/122 differ
```

Blockers: dead `move s0,v0` after `slot_state_setup`, separate `0.0f` loads, register naming. Closure gaps (open):
`slot_state_setup`, `camera_shake_update`, `object_byte9_set`, `state_utility`, `func_800ED66C`, `dispatch_handler`,
and the unregistered head 0x80108154 (see "Closure gaps" below).

History: first full draft emitted 363 of 375 words (159/375 aligned, 0.715); current tuned figures are above. Hand-written
from the retail words.

```
python3 cloud/work/ipa-groups/catchup_logic/extscore.py cloud/work/ipa-groups/catchup_logic --norm
```

## What this is

`catchup_logic` (0x801089CC, "57 insns") is the **tail of an unregistered function**
starting at **0x801084D4** (`addiu sp,sp,-224`, `sdc1 f20/f22`, ends 0x80108AB0,
375 words). The label is in the char-drawing loop at
`beq s0,s8,...` (`bnez` chain over the buffer positions). It sits in the opaque
run of `blob_80107edc.s`. The INDEX 57 = the words from the label to the epilogue.
Also unregistered just before it: 0x80108154 (frame 152, 224 words); and the
neighbour 0x80107EDC is the `dynamic_difficulty` group's function.

- Root: one raw word `0x801084D4` at **0x80115238** (descriptor table). In `keep`.
- Behaviour: a countdown/results HUD. If not (`D_801174B4 & 8`) and `D_8015723C`:
  sets `D_80118E20/24 = 1/3`, `slot_state_setup(1 or 2)`, measures the digit widths
  with `camera_shake_update(56)+1` and `camera_shake_update(58)+1` (glyph `'8'`, glyph
  `':'`) while `object_byte9_set(0)` is active (and restores it). For each car
  (`D_80151AD0`): counts `D_80142740[i]` down by `D_8002EB94` when
  `D_801170FC == 0`; if it is `<= 0` or `D_80152818[i].state == 1` zeroes it and skips;
  else formats `D_80142770[i]*1000` as `MM:SS.mmm` (divisors 600000, 60000, 10000, 6,
  1000, 100, 10, `% 10`) into a 9-byte buffer, fades with
  `func_800ED66C((f32)(s32)(t*255/3))` when `t < 3.0f`, and draws each char
  (a `'8'` underlay for digits, then the char) with `dispatch_handler(0/22)` +
  `state_utility`, advancing by the wide width (`'8'`) or the narrow width
  `(w1+w2)/2` for buffer positions 1, 2, 4, 5. Finishes with `D_80118E24=3;
  D_80118E20=0; render_helper(-1.0f); return 1`.
- The `li a2,10` / `div zero,x,a2` with `break 7/6` checks are ordinary: uopt
  hoists the repeated constant 10 into a register, ugen then divides by a
  register. No special source is needed. Constants used once (600000...) stay `li at`.

## Targets

As for the other two groups: not in `asm/us/blob` `.text.` sections. `group.json`
`"targets"` + `extscore.py` (inflates `assets/us/data.bin`).

## Known differences (first draft, not tuned)

- Frame 168 vs 224: retail has more spill/local slots (spills at 80/92/96/180/196(sp)
  and an 80-byte hole at 100..179). Retail strength-reduces `D_80142740[i]` to a
  walking pointer (`$v1`, spilled at 92(sp)) and `D_80142770[i]` to a byte offset
  (96(sp)); mine indexes.
- `render_helper(0.0f)` and the `<= 0.0f` compare: mine CSEs one `0.0f`
  (`mtc1 zero,f22` at the top); retail loads `f12` and `f20` separately.
- Register naming, dead `move s0,v0` after `slot_state_setup` (see
  `../dynamic_difficulty/STATUS.md`).
- `camera_shake_update` is `extern` (ABI); its `t1`-across-`sound_update_channel`
  usage is not modelled.

## Closure gaps

`slot_state_setup` (IPA `$s2`), `camera_shake_update` (0x800BDDFC, `$t1` kept across
its `sound_update_channel` call), `object_byte9_set`, `state_utility`, `func_800ED66C`
(symbol at 0x800ED66C), `dispatch_handler`; same audio/text
cluster as `dynamic_difficulty`. Also registered nowhere: the head 0x80108154.
