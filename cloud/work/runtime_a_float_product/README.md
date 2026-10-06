# Image A selected car-stat vector product: bounded nonmatch

Research only. `candidate.c` is not a match, is not registered for splicing,
and must not replace the native function in a caller group. Zero accepted bytes
and zero cartridge-coverage credit are claimed.

## Native boundary and purpose

Image A `func_8039D300` spans `[0x8039D300, 0x8039D494)`: 404 bytes,
101 instructions. The protected manifest and extent identify an actual `jal`
entry. SHA-256 of the complete native body:
`e0bf3d0731f618da18552bced27d621bec55be5f5cc274d1554c3a9656b046bc`.

The one ordinary integer input is `player` in a0. For each of five signed-byte
selector tables, the routine reads `13 * player + D_803B9FD0[player]` and uses
that signed selector to choose a four-float row. The table pairs are:

| Signed-byte selector base | Four-float vector base |
|---|---|
| 0x80110E85 | 0x803B28C8 |
| 0x80111049 | 0x803B2948 |
| 0x8011108D | 0x803B2978 |
| 0x801111A9 | 0x803B2A08 |
| 0x8011123D | 0x803B2A58 |

For each signed-halfword loop index 0 through 3 it updates
`D_803BA190[player][component]` with three separate stores:

1. `(first * second) * third`
2. Reload destination, then multiply by fourth
3. Reload destination, then multiply by fifth

There are four binary32 multiplications and three stores per component.
The original loads fourth before the first store and fifth before the second.
The natural C's generated loads are scheduled differently; the bounded
behavior claim therefore requires disjoint backing and stable inputs.
No algebraic reassociation was used in the candidate.

The related consumer is the complete 524-byte BLIT callback
`A:803A4134`, which reads the output using player/bar indices. The source
antecedent for its presentation purpose is
[`game/select.c:AnimateBar`](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/select.c#L2383).
That arcade callback has two fixed integer car-stat arrays; it does not supply
this N64 five-vector producer. A search of that donor's `select.c` found no
matching producer. This is an N64-specific reconstruction, not an assertion
that the original source text, typedefs, or translation unit were recovered.

## Exact caller contract blocks replacement

There are no calls, stack accesses, hidden incoming GPR/FPR values, or
callee-saved writes in the native body. Its only incoming registers are a0 and
the ordinary return address. The complete executed write sets are:

- GPR: at, v0, v1, a0–a3, t0–t2, t6–t9
- FPR: f4, f6, f8, f10

In particular t3–t5 and f16/f18 are preserved. The complete protected-image
scan finds exactly two direct calls:

- `func_803A0FD8 + 0x114`, call at 0x803A10EC
- `func_803A1EAC + 0x19D4`, call at 0x803A3880

The first caller loads t3/t4/t5 with the pointers 0x803B28B8,
0x803BA048, and 0x803BA038 before the call and increments those same registers
at 0x803A1100, 0x803A1108, and 0x803A110C afterward. This is concrete
preservation dependence, not an allocator-style guess. No direct caller
dependence on f16/f18 is claimed, but their native preservation is still
explicitly checked.

The ordinary O2 seed caches the selected-byte value once and changes all
five preserved registers in every one of the 1,231 bounded runs. It is unsafe
as a native replacement despite reproducing the scoped output and store
values. Genuine caller context is a justified next boundary to investigate.
The bounded experiments do not prove standalone matching impossible.

## Whole-object result

Canonical IDO 5.3 recipes use `-g0 -mips2 -G 0 -non_shared` and the existing
`-Wab,-r4300_mul` correction. All twelve named address anchors resolve through
24 HI16/LO16 records. Independent ELF inspection covers the entire function,
complete text section, every relocation, undefined symbol set, function size,
exact GNU placement, alignment, and nonempty allocated sections. GNU ld at
0x8039D300 agrees with the scorer's resolved object across every text byte.
There are no owned data sections or unknown/unverified references.

| Optimization | Actual function bytes | Strict native comparison |
|---|---:|---|
| O1 | 516 | 100/101 different, 28 nonzero excess words |
| O2 | 384 | 99/101 different; 20 native bytes missing |
| O3 | 384 | 99/101 different; 20 native bytes missing |

These are full-body failures, not relocation-blind scores or shortened-prefix
successes. O1 has 12 zero alignment bytes; O2/O3 have none. Workbench diagnosis
classified the ordinary result as a structure mismatch (101 native versus
96 candidate instructions), consistent with the changed selector-load count.

Two bounded exploratory controls made only the selector volatile, then all
six signed-byte inputs volatile. They restored 404-byte extent but still
failed at 96/101 and 97/101 words respectively. These unproven qualifier
changes were discarded; no volatile guess, fake caller, invented parameter,
register-pressure expression, padding, or protected-file edit is in the seed.

## Behavior proof and explicit domains

The replay uses the manifest-authenticated native words and a separately
compiled and fully relocated O2 object. It also compiles the unchanged seed
into a C89 host harness with UBSan/bounds, strict floating-point options and
no contraction. Three independent paths agree on output bits in 1,231 cases:

- All 4 player × 13 selection cells
- All 3^5 synthetic vector-row combinations
- Both signed zeros in every factor and all five-factor sign combinations
- 512 deterministic cases with distinct component values and normal results

Every native instruction and both loop outcomes execute. The native and
source-built MIPS executions agree on all twelve ordered store address/value
pairs, not just final memory. They leave every other backed byte unchanged,
including native canaries and all input tables. The host check separately
verifies every input array and all sixteen output cells.

The backing domain is deliberately synthetic and explicit: player 0..3;
selection 0..12; each of five vector selectors 0..2; four float components per
row; four output rows. These ranges are valid in the harness's actual C
arrays and disjoint byte allocations at the native addresses. They do not
claim the original game's array lengths or every reachable selector value.
Distances between table base addresses are not sufficient proof of array
bounds. Negative or larger selectors, unstable globals and arbitrary aliasing
are outside the C equivalence claim.

Floating arithmetic is exactly modeled with integer significands and
round-to-nearest ties-even. Inputs and intermediate results are finite normal
binary32 values or signed zeros. The tests compare 10,000 additional normal
products against an independently rounded host conversion. The sign of every
zero product is checked bitwise.

Infinity, both NaN encodings, nonzero subnormal inputs, overflow and underflow
results are explicitly rejected by the bounded arithmetic model. Those
rejections are not proof of hardware arithmetic behavior. No VR4300 FCSR,
trap, NaN payload/quieting, denormal, alternate rounding-mode or exception-flag
equivalence is claimed. In particular the standalone compiler exchanges the
operand order of the second multiply; finite commutativity does not establish
NaN behavior. A final native byte match remains necessary.

Source mutations that reassociate the first three factors, replace the fifth
factor, or combine the three stores into one are freshly compiled and
rejected by numerical output or ordered-store traces. The coalesced-store
control is rejected even when its final float result agrees. Unknown
instructions, missing backing, invalid domains and optimized-Python execution
also fail closed.

## Reproduce

From a normal checkout with the documented IDO and MIPS GNU toolchain:

```sh
PYTHONDONTWRITEBYTECODE=1 python3 cloud/work/runtime_a_float_product/verify.py --check
PYTHONDONTWRITEBYTECODE=1 python3 -m pytest -p no:cacheprovider -q tests/cloud/test_runtime_a_float_product.py
```

A sparse research worktree can pass `--repo /path/to/full/checkout` to the
verifier; the candidate and harness still come from the packet's own tree.
For its test command, set `RUSH_RECOVERY_ROOT` to that full checkout. The
receipt binds source, verifier, model, harness, compiler, selected native
bodies, protected manifest, full resolved code and relocations. It excludes
path-sensitive raw debug-file hashes. The current focused result is 12 tests
passed; no broad suite, image/deflate/ROM gate, or hardware run is claimed.
No ROM bytes or raw assembly dumps are committed.
