# Filtered vertex bounds: portable reconstruction and semantic proof

`func_800B9740`, extracted game target at `0x800B9740`, remains **NONMATCH**.
There are **100 differing words out of 102** and **3 nonzero excess words**.
Target extent is 408 bytes. Candidate function is 420 bytes, followed by 12
bytes of assembler padding (432-byte `.text`). No match, accepted coverage,
image splice, or ROM identity is claimed. `claims` is empty.

## New contribution and provenance

The earlier `cloud/work/tiny_A35/func_800B9740.c` is explicitly acknowledged:
its baseline was 100/102 + 7 excess, with a documented cached-count control
97/102 + 1 excess. This contribution does **not** claim to beat that control.
It removes the old `(u32)` pointer truncation from the semantic reconstruction,
uses genuine pointers to three-halfword vertex arrays, and adds new executable
native/linked-C differential proof, independent selection oracle, portable host
regressions, sanitizer checks, ABI preservation, and write-sequence verification.
The current natural indexed-axis variant has 100/102 + 3 excess.

Names and record views are inferred from the complete native instructions;
there is no arcade source checkout here and these are not established game
class names. No existing accepted source, locks, target bytes, scorer, or
protected paths were edited.

## Semantics and contract

- Minimum starts at +32767; maximum starts at **-32767**, not -32768.
  An eligible all--32768 point thus leaves maximum at -32767. Empty selection
  preserves all sentinels. Tests explicitly protect this unusual behavior.
- Total vertex count is an unsigned halfword at `D_801527A4`; primary count is
  halfword offset 0 of `D_801407F0`. Vertices below primary count are eligible.
- Remaining vertices are eligible if contained in any **kind 1**, half-open
  range. The byte range count is at mesh offset 8; range pointer is at 12.
- N64 range records have stride 16, kind byte 0, count halfword 10 and vertex
  pointer 12. Vertices have stride 6 and three signed-half coordinates.
- All ranges must point into the same valid vertex array (including one-past
  for empty ranges), with endpoints inside/one-past it. This explicit precondition
  makes C pointer comparison/subtraction defined and portable. It is an inferred
  data-model contract, not a claim of equivalence for arbitrary raw addresses.
- Bounds, mesh, vertices and descriptors must be valid nonoverlapping ordinary
  memory. No volatile/MMIO, concurrent mutation, or adversarial aliasing is
  modeled. C eagerly caches primary count even for zero total count; native
  skips that load in the empty case. The mesh remains valid under this contract.
- Every eligible vertex performs all six min/max stores, including stores of
  retained values. The differential harness compares their exact sequence and
  validates no other non-stack memory changes, and callee-saved registers/SP.

## Causal controls and remaining work

The array-axis/cached-count source uses only meaningful typed locals. O3 emits
the same residual as O2. Replacing the axis indexing with natural coordinate,
minimum and maximum cursors worsens excess to 6 (100/102). An uncached portable
translation of A35 reproduces 100/102 + 7. O1 worsens size dramatically and has
an unpaired HI16 rejected by the canonical scorer; it is not accepted evidence.
The workbench reports structural/register differences; unresolved object-local
relocations in that diagnostic are not proof. The authoritative proof below
uses GNU ld to resolve every external symbol before full-word comparison.
No pressure padding, dummy helpers, forced registers, fabricated parameters,
or optimizer-steering casts were tried or retained.

## Reproduce

Use the pinned repository IDO setup, with GNU MIPS binutils on PATH (and its
library directory on LD_LIBRARY_PATH if installed outside system directories).

```sh
python3 cloud/work/dot_vertex_bounds/verify.py
python3 -m pytest tests/conveyor/test_dot_vertex_bounds.py -q
cc -std=c89 -pedantic -Wall -Wextra -Werror -fsanitize=address,undefined \
  -fno-sanitize-recover=all -g cloud/work/dot_vertex_bounds/host_test.c -o /tmp/bounds
ASAN_OPTIONS=detect_leaks=0 /tmp/bounds
```

`verify.py` freshly compiles with IDO, checks protected target hashes through
the canonical scorer, GNU-links real addresses, asserts full function/text
extent and residual, then executes all target and linked words in a fail-closed
integer MIPS interpreter. 1,000 deterministic cases cover zero/nonzero counts,
0/1/255 ranges, overlapping and empty ranges, inclusive starts/exclusive ends,
kind rejection, unsigned primary count 65535, signed-half extremes, exact write
sequence and ABI preservation. This is bounded semantic testing, not a general
MIPS emulator or a proof of all possible inputs.

The host harness checks 20,000 additional deterministic cases with up to 128
vertices and 255 ranges, native-width pointers, C89 warning-clean compilation,
ASan and UBSan. Leak detection is disabled only because this execution container
uses ptrace and the program performs no allocations; address/UB checking stays
on. Host layout offsets that depend on pointer width are conditional; the
N64 layouts are independently exercised by the complete linked target replay.

Independent lane C reviewed source hash
`046aa583b8c02a8bf1695703102fb1ccd9585adb7ed10a5803ebc8cb152d27ef`, native
instructions, ABI and scope, and independently replayed all 1,000 differential
and 20,000 sanitizer cases: PASS as research, not a match.
