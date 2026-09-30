# championship_standings

**1 of 4 members MATCH** (`func_800DC120`, 35 of 279 words), rescored
2026-09-30 (`zbuild.py --as1=-r4300_mul`). `func_800DC120` is spliced
(`src/blob/groups/championship_standings`); the rest are not.

| function | target words | result |
|---|---|---|
| `func_800DC120` | 35 | **MATCH** |
| `func_800DC1AC` (`keep`) | 39 | 3 differ |
| `tournament_trophy_award` (`keep`) | 97 | 40 differ |
| `championship_standings` (`keep`) | 108 | 41 differ (emits 109) |

## What this is

A 5-bit password codec, not championship logic (labels are historical).
`group.c` is hand-written from the assembly.

| label | role |
|---|---|
| `championship_standings(u8 *str)` | decode `str` into the bit buffer, verify checksum; 1 if valid |
| `func_800DC120()` | checksum of the payload bits |
| `func_800DC1AC(u32 value, u32 nbits)` | append the low `nbits` bits of `value` (max 32) |
| `tournament_trophy_award(char *out)` | append checksum, encode buffer as characters |

Globals: `D_8012E618` bit buffer, `D_801170E8` size in bytes, `D_801170EC`
payload bits, `D_801170F0` checksum bits, `D_801170F4` write position in bits,
`D_80116FE8` char-to-value table (0xFF invalid), `D_80116FE4` pointer to the
32-character alphabet.

## Closure

`func_800DC1AC` is also called by `func_800F42C8` (outside the group) and the
ROM uses the standard ABI (`value` `$a0`, `nbits` `$a1`), so it is in `keep`
itself (a stand-in made IPA reassign to `$a2/$a0`; this took
`championship_standings` from 108 to 41 differing words). No other gap known.

## Techniques

- `func_800DC120`: plain `while (i < D_801170EC)`, no hoisted `1 << D_801170F0`
  (the hoisted local and the `for` form miss).
- `func_800DC1AC`: `for` with an **`s32 i`** stays rolled (`u32 i` unrolls to 98
  words); with no `pos`/`p` locals
  (`D_8012E618[D_801170F4 >> 3] |= (value & 1) << (D_801170F4 & 7); D_801170F4++; value >>= 1;`)
  every register matches. Only the order of the last three instructions differs:
  target `sh; move a3,t8; bnez; sb (delay)`, ours `sb; sh; bnez; move`.
  Getting that order needs the byte store last with named `pos`/`bit`, which
  then takes other registers (22/39). Statement orders, all local int types,
  `++D`/`+= 1`, pointer forms and a random search did not give both.

## Remaining

`func_800DC1AC` (order of last three instructions); `championship_standings`
and `tournament_trophy_award` not tuned yet.
