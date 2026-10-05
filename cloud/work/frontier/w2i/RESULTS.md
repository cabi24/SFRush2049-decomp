# w2i results — object_render

| Function | Bytes | State | Flags | File |
|---|---:|---|---|---|
| `object_render` | 10,048 | **strict MATCH**, and EQUAL in the whole-program unit | `-g0 -O3 -mips2 -G 0 -non_shared` (also MATCH at `-O2`) | `cloud/matches/object_render.c` |

Not spliced, not committed. Semantics, sections and globals: `STRUCTURE.md`.

## Scorer output (exact)

Strict form (`cloud/matches/object_render.c`), on the builder in `~/rush2049/scratch/frontier/w2i`:

```
$ python3 tools/cloud/score.py fn cand/object_render.c object_render --flags "-g0 -O3 -mips2 -G 0 -non_shared"
object_render:
  MATCH
$ python3 -m tools.conveyor.pipeline.blob_unit --tag w2i score object_render --with cloud/matches/object_render.c
  EQUAL object_render: 2512 words (kept, c_object_render.c)
blob_unit score: 1/1 equal; object build/blob_unit/w2i/unit.o (4.1s)
```

Natural form (`object_render/best.c`, key written with `<<`):

```
object_render:
    +0x0fc  want 0c003665 jal 0xd994                    got 0c000000 jal 0x0
    +0x118  want 0c003665 jal 0xd994                    got 0c000000 jal 0x0
    +0x134  want 0c003665 jal 0xd994                    got 0c000000 jal 0x0
  3/2512 words differ (unresolved symbols: __ll_lshift+0x0 at +0xfc, __ll_lshift+0x0 at +0x118, __ll_lshift+0x0 at +0x134)
```

Aligned difference: 0 words in both forms (the three words of the natural form are the same `jal` with an
unresolved target, not a code difference).

## The two forms

The two files differ in one statement:

```c
tile = ((s64)uls << 48) + ((s64)ult << 32) + ((s64)lrs << 16) + lrt;                 /* best.c */
tile = __ashldi3(uls, 48) + __ashldi3(ult, 32) + __ashldi3(lrs, 16) + lrt;           /* cloud/matches */
```

IDO compiles `<<` on a 64-bit value to a call to `__ll_lshift`. Retail calls 0x8000D994 there, which this
repository labels `__ashldi3`, so neither the scorer nor the splice link can resolve the compiler's name.
The strict form calls the routine by the repository's label; it is the same code, but the helper call is
not what the original source said. **Owner decision:** either add `__ll_lshift = 0x8000D994` to the symbol
tables (then `best.c` is the source to land), or land the strict form as is. I did not touch
`symbol_addrs.us.txt`, `asm/` or the scorer.

## History (4 compiles to code-identical)

1. First complete draft: 18 / 2,512 words. Two causes only.
2. Shift operand order (`uls << 48` must be written first): 14 words, all in the last 9 instructions.
3. Tail: retail stores the 64-bit key with one `lui at` for both halves, the draft used two (function one
   word too long). Cause: as1 shares the `lui` only when it knows the symbol's alignment, i.e. when the
   variable is defined in the unit. `s64 D_8012E688;` (a definition instead of `extern`) fixes it; also
   true inside the whole-program unit, where nobody else defines it. `blob_splice.link_function` already
   rebinds defined `D_` symbols to their image address, so this should splice unchanged (not tried: I do
   not splice).
4. Helper symbol, as above.

No residuals remain, so there are no per-region notes.

## What generalises

- **A 64-bit global store with a single `lui at` means the global must be defined, not extern.** Look for
  `lui at; sw hi,X(at); sw lo,X+4(at)`. `sound_init` (unmatched) stores the same variable the same way and
  will need the same definition; the shared declarations (`extern s32 D_8012E688; extern s32 D_8012E68C;`
  in ~470 files) describe one `s64`.
- **IDO runtime helpers have compiler-fixed names** (`__ll_lshift`, and presumably `__ll_rshift`, `__ll_mul`,
  `__ll_div`, `__ull_*`, `__d_to_ll` …). The repository uses libgcc names for these addresses
  (`work/libgcc/`), so any function using 64-bit arithmetic will show unresolved `jal`s. Aliases for the
  whole family would remove the need for explicit-call spellings.
- **Size was not difficulty.** The 632-byte frame with one slot per macro `_g`, and `0xFD/0xF5/0xE6/0xF3|0xF4/
  0xE7/0xF5/0xF2` command runs, identify SDK `gDPLoadTexture*` macros; writing them as macros on
  `D_80149438++` reproduced all 2,500 words of register allocation and scheduling without any tuning.
  Scout the command bytes before assuming a large function needs the large-function tools.
- Frame-slot arithmetic is a quick check of macro count: distance between two macros' `_g` slots / 4 =
  number of `_g` declarations between them (it confirmed 7-command loads and the 2-command type-5 tail).

## Tools left here

`mk.sh` (prepend `object_render/gbi_min.h` to a body), `sc.sh`, `full.sh`, `o3s.sh` (w1b copies retargeted
to the `w2i` scratch). `object_render/gbi_min.h` is a self-contained extract of the SDK texture-load macros
usable by the other functions of this family.
