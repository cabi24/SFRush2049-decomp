# Register-rotation investigation

## Result

The two-word residual in `func_800CCE5C` is controlled by the integer register
reservation range handed from **uopt to ugen**. It is not explained by declaration
order or by choosing `-O3` for a single exported function.

A controlled replay of the actual project IDO 5.3 passes on watchman2 changed
only one integer in the optimized ucode scratch file. Increasing the reserved
range from registers 2–7 (`v0` through `a3`) to 2–13 (`v0` through `t5`) makes
the temporary allocator wrap from `t9` back to `t6`, exactly as retail does.
All 40 relocated instructions then compare equal. Intermediate reservation
lengths produce `t1`, `t2`, `t3`, `t4`, and `t5` in the same two operand positions.
This isolates a cause for this particular residual; it does not prove that
all eight reported functions have the same cause.

**This is diagnostic evidence, not a matching C submission.** The unmodified C
still differs by two words. The replayed object was never submitted, spliced,
locked, or committed. No retail bytes, compiler intermediates, or objects are
included here. `replay.json` records only comparison summaries and marks every
row `diagnostic_only`.

| Reserved range length | Last temporary | Strict residual |
|---:|---|---:|
| 6 | t0 | 2/40 |
| 7 | t1 | 2/40 |
| 8 | t2 | 2/40 |
| 9 | t3 | 2/40 |
| 10 | t4 | 2/40 |
| 11 | t5 | 2/40 |
| 12 | t6 | 0/40, diagnostic replay only |

## Reproduce

Use a scratch checkout on the documented 4 KB-page builder, with the protected
assembly and manifest copied intact. From its root:

```bash
IDO_DIR=/home/cburnes/rush2049/repo/tools/ido-static-recomp/build/out \
  python3 cloud/work/register-ring/replay.py --json-out /tmp/ring-replay.json
```

The script compiles `func_800CCE5C.c` with normal `-O2` settings and retained
scratch, finds exactly one expected integer `Uregs` record, and replays **ugen
and as1 only** with reservation lengths 6–12. It fails if the record is absent,
ambiguous, or different. All intermediates stay in a temporary directory.
The first replay is the unchanged control. The checked-in source combines the
near-miss TU declarations with the hand agent's two-word-residual body.

## Why a group can matter even when ABI classification says ABI

The independently decompiled IDO 7.1 optimizer is a useful source-level model,
not proof of the project's 5.3 implementation. At revision
`d068e439f52615763a3facd6944873899ebad2fd`:

- [uoptemit.c](https://github.com/n64decomp/ido/blob/d068e439f52615763a3facd6944873899ebad2fd/src/uopt/uoptemit.c#L797)
  emits `Uregs` in the function prologue. Ordinary functions use the highest
  caller-save register selected by the optimizer; interprocedural functions
  reserve the larger register range.
- [uoptreg1.c](https://github.com/n64decomp/ido/blob/d068e439f52615763a3facd6944873899ebad2fd/src/uopt/uoptreg1.c#L1881)
  can raise a caller's reservation ceiling when a visible interprocedural
  callee uses higher registers.
- [uoptreg2.c](https://github.com/n64decomp/ido/blob/d068e439f52615763a3facd6944873899ebad2fd/src/uopt/uoptreg2.c#L2176)
  tracks the highest register chosen during global coloring.

The replay confirms the relevant reservation effect in our real 5.3 toolchain.
An ABI-looking function can therefore depend on compilation context without
reading a non-ABI parameter or visibly preserving a caller-save register.
That is a working hypothesis for the other reported rotation cases, not a
reason to relabel them or claim them matched.

## What did not solve it

The `-Wo,-regr,0..9` sweep at both `-O2` and `-O3` changes global allocation and
other instructions; none matches (`sweep.json`). This option is not equivalent
to changing only the generator's temporary reservation.

The smallest real-callee group probe uses the best target body plus an m2c
`func_800A1910` and a two-site stand-in. Keeping that callee exported preserves
the original two-word residual. Localizing it changes argument allocation and
the body to 32 differing words / 42 emitted words. Neither group claims a
match (`group-results-fixed.json`). Larger exploratory groups with
`format_string_parse` and `slot_state_lookup` remain blocked by inferred
signatures/types (`group-results.json`); their `claims` lists are empty.

The remaining engineering problem is to reproduce the required reservation
through ordinary C and a valid whole-program group, while preserving retail
parameter registers and all instructions. A useful next experiment is to trace
the `Uregs` output while varying real callee/context membership and keep lists.
The compact replay now gives a precise target for those experiments instead
of trying more unrelated source permutations.
