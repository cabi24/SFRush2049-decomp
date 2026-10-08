# w14t results (wave 14, mass variant testing)

Assignment: `entity_update_callback` (2,184 B) and `func_800F7F3C` (1,396 B), both register-colour residuals.
Builder scratch: `~/rush2049/scratch/frontier/w14t` (base copy plus src/blob, include, tools/cloud, asm/us/blob,
blob_matched.lock.json from the Pi). Trace install: `--reuse ~/rush2049/scratch/frontier/wtk` (MANIFEST checked).
Flags: `-g0 -O3 -mips2 -G 0 -non_shared` everywhere (the -O2 run of entity_update_callback gave the same 424).

## Starting points (re-scored, one vbatch run per function)

- `entity_update_callback`: w6d, w7d and w10a `best.c` are byte-identical (md5 234f4b1a…). Standalone strict 424
  each (`vbatch` strict column). Unit figure from w10a: 423 of 546.
  Note on the old draft: its `if (obj == 0) {}` (line 116) is a compiled-out read of `obj` with no code. It is
  disclosed here and in the file header of any candidate that keeps it.
- `func_800F7F3C`: w1c, w10b and w11d `best.c` are byte-identical (md5 7d75daaf…). Standalone strict 117 each.

## entity_update_callback

| round | template | variants | best strict | notes |
|---|---|---|---|---|
| b1 | 8 choice points on w6d (decl order, `if` form, timer and alpha forms, glow cast, dx/dz, dscale) | 300 | **335** (mnem 10) | every top-10 file has the `&&` operand swap `(D_80156994 != 0 \|\| D_8014978C >= 6) && active_player_count < 4` |
| b2 | 12 choice points, the swap fixed as option 0 | 300 | 335 (control, = b1 best) | 30 files at 335, 10 at 337. No improvement |
| b3 | 13 axes plus a linked `car` local (`s16 car = obj->car`) replacing `obj->car` | 300 | 424 (control) | the `car` local is worse: 543 strict, size -74, many compile fails. Refuted |

Diagnose on w6d, re-run with `--flags "-g0 -O3 -mips2 -G 0 -non_shared"` (the first run had no flags and defaulted to -O2; verdict unchanged): `mixed(constant:1, structural:4, schedule:8, register:311)`,
lever `none-known`, strict 423 of 546. ctrace: phase-1 webs w5 (`obj->car` load, 56/6 = 9.33) and w7 (record
address, 26/5 = 5.2) are swapped v0/v1. Forcing `p1:w5=c2,p1:w7=c1` gives 399 positional rows, and with
normalised registers only 3 rows remain: a branch-likely `bnezl at,0x4b0` whose delay slot holds `addiu t9,v1,1`
(the `phase` update) where ours has `bnez`, plus `lui v1` vs `lui a1`.

Best strict (335) reached by the operand swap, but it moves the structure: `us.sh` on the 335 file gives 16 normalised
rows (w6d: 2). The original order matches retail's load order (`lh active_player_count` first). So 335 is a colour
gain bought with a structure loss; it is **not** a match and not a lead to ship.

One-edit climb on the 335 file (`wbgen --classes O,P` → `euc/g2`, 1,296 files, `--top 20`): best strict 335 =
control (`strict 335 mnem-missing 10 aligned-missing 324 size +0 v0000.c unverified=8`). Every other file is 335 to
382 (with `size +48` at worst). No strict improvement, so the climb stops (third round with no gain: b2, b3, g2 gave
no strict drop below the 335 start; b3's start was 424).

Stop reason for entity_update_callback: the best strict count comes from an operand swap that changes the branch
structure (16 normalised rows against 2 for w6d). No variant found reaches 0 rows in either lens. Residual: colour
(v0/v1 webs w5/w7, 399 rows under the forced oracle) plus the `bnezl` / delay-slot structure. `obj == 0` compiled-out
read is still in the file (w6d's `if (obj == 0) {}`); disclose it in any candidate header.

## func_800F7F3C

| round | template | variants | best strict | notes |
|---|---|---|---|---|
| b1 | arm-1 choice points: `s32` vs `s16` ia/ib, loop bound `j+1 < N`, compare mirrored, store order, `if (i) {}`, `+= 1`, mirrored tie compare | 128 | 117 (control) | no variant below 117; all inert |
| b2 | arm-1 named locals `f` and `c` inlined, `ia`/`ib` inlined into the sort compare (each choice linked) | 128 | 117 (control) | inlined forms: 286 strict, size +1. Refuted |
| b3 | arm-1 live limit local `s16 lim = D_8014A108 - 1` used by both sort loops | 2 | 117 (control) | 448 strict, size +122 (unrolls). Refuted, matches w1c's note |

Diagnose (`--flags -O3`, source f7f/s0/v0000.c): `mixed(constant:6, register:117)`, lever `none-known`. Mnemonic rows 0, so the residual is colouring only, as the w10b
force-oracle showed (`p2:w60=c12,p2:w201=c14,p2:w202=c15,p2:w205=c12` gives arm 1 register-exact; the rest is
temp-ring rotation).

One-edit climb (`wbgen --classes O,P`, `f7f/g1`, 15 files, scored with `--top 10`): control 117 is the top line, every other file is 9999 or worse. No improvement, so the function is stopped at 117.
