# championship_standings

**BUILDS.** One member matches.

```
$ python3 tools/cloud/score.py group cloud/work/ipa-groups/championship_standings
championship_standings:   41/108 words differ
func_800DC120:            MATCH
func_800DC1AC:            26/39 words differ
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

## Remaining differences

- `func_800DC1AC`: loop shape now matches (no unrolling, one load of the
  position per iteration, `i++` first), but registers differ: the target
  copies `value` to `$a3` and reuses `$a0` as the counter. Tried `for`,
  `do/while`, a `pos` local and `D_801170F4++` forms; best is 26/39.
- `func_800DC120` matched with a plain `while (i < D_801170EC)` loop and no
  hoisted `1 << D_801170F0` (the hoisted local and the `for` form both miss).
- `championship_standings`, `tournament_trophy_award`: not tuned yet.

## Relocations (for review until F2 lands)

`D_8012E618`, `D_801170E8`, `D_801170EC`, `D_801170F0`, `D_801170F4`,
`D_80116FE8`, `D_80116FE4`; calls `func_800DC120`, `func_800DC1AC`.
