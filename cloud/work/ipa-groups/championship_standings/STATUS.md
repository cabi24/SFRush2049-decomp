# championship_standings

**BUILDS.** One member matches.

```
$ python3 tools/cloud/score.py group cloud/work/ipa-groups/championship_standings
championship_standings:   41/108 words differ
func_800DC120:            MATCH
func_800DC1AC:            3/39 words differ   (cloud pass 4; was 10/39)
tournament_trophy_award:  40/97 words differ
```

## What this is

A 5-bit password codec, not championship logic (the labels are historical):

| label | role |
|---|---|
| `championship_standings(u8 *str)` | decode `str` into the bit buffer, verify the checksum; 1 if valid |
| `func_800DC120()` | checksum of the payload bits |
| `func_800DC1AC(u32 value, u32 nbits)` | append the low `nbits` bits of `value` (max 32) |
| `tournament_trophy_award(char *out)` | append the checksum, encode the buffer as characters |

Globals: `D_8012E618` bit buffer (bytes), `D_801170E8` its size in bytes,
`D_801170EC` payload length in bits, `D_801170F0` checksum length in bits,
`D_801170F4` write position in bits, `D_80116FE8` char-to-value table
(0xFF = invalid), `D_80116FE4` pointer to the 32-character alphabet.

`group.c` is hand-written from the assembly (not the m2c seed).

## group.json change

`func_800DC1AC` is also called by `func_800F42C8`, outside this group, and the
target uses the standard ABI for it (`value` in `$a0`, `nbits` in `$a1`). With
only a stand-in in `keep`, IDO's IPA reassigned its parameters (`$a2`/`$a0`).
It is now in `keep` itself and `__standin_func_800DC1AC` is removed. That
moved `championship_standings` from 108 to 41 differing words.

## Cloud pass 4: func_800DC1AC 10 -> 3

Found by trying more shapes with the fast loop `zbuild`: a `for` loop with an **`s32 i`**
stays rolled (a `u32 i` unrolls to 98 words), and with **no `pos`/`p` locals** at all
(`D_8012E618[D_801170F4 >> 3] |= (value & 1) << (D_801170F4 & 7); D_801170F4++;
value >>= 1;`) every register matches the target (`i` in `$a0`, position in `$v0`,
`t0` = `&D_801170F4`). The only difference left is the order of the last three
instructions: target `sh; move a3,t8; bnez; sb (delay)`, ours `sb; sh; bnez; move (delay)`.
That order needs the byte store to be the last statement of the body with the position
store and `value >>= 1` before it: written that way (`pos = D_801170F4; bit = value & 1;
D_801170F4 = pos + 1; value >>= 1; arr[pos >> 3] |= bit << (pos & 7);`) the instruction
order matches exactly but the named `pos`/`bit` webs take other registers (22/39). Every
statement order, `pos`/`bit`/`old` locals with all int types and declaration orders, `++D`,
`+= 1`, pointer forms, and a random search did not give both.

- `func_800DC120` matched with a plain `while (i < D_801170EC)` loop and no
  hoisted `1 << D_801170F0` (the hoisted local and the `for` form both miss).
- `championship_standings`, `tournament_trophy_award`: not tuned yet.

## Relocations (for review until F2 lands)

`D_8012E618`, `D_801170E8`, `D_801170EC`, `D_801170F0`, `D_801170F4`,
`D_80116FE8`, `D_80116FE4`; calls `func_800DC120`, `func_800DC1AC`.
