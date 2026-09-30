# reconnect_attempt  ->  really `func_8010BC84` (unregistered head)

## Current status (Rescored 2026-09-30 (Round 2 addendum), master d0891f3.)

**Not a MATCH. Builds; 0/232 words strict; 122/232 words match after alignment, structure 0.898**
(the headline below says 117/232: superseded). Scored with `python3 cloud/work/ipa-groups/reconnect_attempt/extscore.py --norm cloud/work/ipa-groups/reconnect_attempt`:

```
func_8010BC84  size 227/232  192/232 words differ (position-based)  exact after alignment 122/232
context (not counted): slot_state_setup 18/58, object_byte9_set 5/16, sound_update_channel 116/122 differ
```

Blockers: dead `move s0,v0`, the `state_utility` argument temp, register naming. Closure gaps (open):
`slot_state_setup` plus the audio/text cluster, and `object_bytes_sum_global`, `object_manager_update`,
`state_utility`, `Input_ApplyPadConfig`, `dispatch_handler` (all ABI here, not attempted as context).

**Result: not a MATCH. 227 of 232 words emitted; 117/232 words match exactly after
alignment, register-blind structure 0.898.** Hand-written from the retail words.
No strict score is possible with `score.py` (see "Targets").

```
python3 cloud/work/ipa-groups/reconnect_attempt/extscore.py cloud/work/ipa-groups/reconnect_attempt --norm
```

## What this is

`reconnect_attempt` (0x8010BE7C, "106 insns") is the **tail of an unregistered
function** that starts at **0x8010BC84** (`addiu sp,sp,-64`, ends at 0x8010C024,
232 words; a separate 2-word `jr ra; nop` follows at 0x8010C024). It lies in
the opaque `.incbin` run at the start of `asm/us/blob/blob_8010b5d0.s`. The label 0x8010BE7C is
inside the per-car loop, at the `subu s0,s0,s4` before the second `object_bytes_sum_global`
call. The member is therefore `func_8010BC84`.

- No caller by `jal`/`lui+addiu`; one raw word `0x8010BC84` in a descriptor table at
  **0x80117330** (same record shape as `dynamic_difficulty`'s). Root, in `keep`.
- Behaviour (a screen handler with `a0` = a screen record): mirrors
  `(D_801170FC != 6)` into `arg[26]` (calling `Input_ApplyPadConfig(arg)` on change);
  if `arg[26]` is set returns 1. Otherwise `render_helper(0.0f)`; scans the
  `D_8014A108` cars (`D_8014A118`, 0x4C bytes each, byte +1 = owner index into
  `D_80144030`, 0x304 bytes each, byte +6 = ready) and, if any is not ready, draws a
  centred string (slot 10, row 0 of `D_80117100`, `dispatch_handler(22)`,
  `object_manager_update`, `state_utility`). Then (slot 11) for each car draws its
  name (`*D_8014A118[i].text + 20`) and a second string chosen from `D_8017A4E0.song`
  (+0x2A8 or +0x2AC), positions from `D_80117100[count-1].e[i]` (0x18-byte cells:
  `s16 x` at +0x12, `s32 y` at +0x14), `render_helper(-1.0f)`, return 1.

## Targets

The words are not `.text.` sections in `asm/us/blob`; they come from the inflated
game image (see `../dynamic_difficulty/STATUS.md`, "Why the retail words are not
in the repo tools"). `group.json` has `"targets": {"func_8010BC84": {"addr":"0x8010BC84","words":232}}`,
read by `extscore.py`.

## What matches / what does not

- Structure, all data addresses (`D_80117100`, `D_8014A108`, `D_8014A118`,
  `D_80144030`, `D_801146AC`, `D_8017A4E0`), frame size (64), the s16 casts on the
  `y` values (`y = (s16)(...)`), the `srl` (unsigned) `>>1` for the call widths and the
  signed `/2` in block 1 all agree.
- **Register naming differs from the first instruction** (`$v0`/`$v1` swapped for the
  mode load and the `arg[26]` byte, `$s0`/`$s1` for `arg`/`all`, `$s7`/`$s8`, and the first
  loop's constants use `$a0/$t0-$t2/$a3` in retail but `$v0/$a3/$t0/$t1/$s8` here).
  Loop constants and the count live in different registers because IPA allocation
  depends on the register usage of every function in the module; tried
  declaration order, an explicit bound variable, both operand orders. A cast
  `(s8)(mode != 6)` flips the load order (mode in `$v0`) but adds `sll/sra`.
- Retail keeps a dead `move s0,v0` after each `slot_state_setup` (same unexplained
  behaviour as in `dynamic_difficulty`), and a copy `move s3,s5` of the string x
  across `state_utility`; IDO propagates my copies away.
- The `state_utility` arguments: retail materialises the first argument in a
  temp (`subu t7,...; sll a0,t7,16; sra t8,a0,16; move a0,t8`); mine computes it
  directly into `$a0` (`subu a0,...; sll; sra a0`).

## Closure gaps

`slot_state_setup` (IPA `$s2`), plus the whole audio/text cluster listed in
`../dynamic_difficulty/STATUS.md`. In addition this function calls
`object_bytes_sum_global` (0x800B3F50, two `sound_update_channel(0)` calls, `$t1`
kept across one), `object_manager_update` (0x800B3FA4, 133 words, saves `s0-s5`),
`state_utility` (0x800B71D4, `t2/t3` kept across `object_manager_update`),
`Input_ApplyPadConfig`, `dispatch_handler`: all ABI here, but their exact
register footprint feeds the caller's allocation. Not attempted as context.
