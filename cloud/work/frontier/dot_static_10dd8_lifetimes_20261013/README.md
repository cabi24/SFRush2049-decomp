# func_80010DD8: strict-next-quantum allocation

**MATCH: all 42 words / 168 native bytes, stock IDO 7.1.** The deliverable is
[`cloud/matches/boot_tail/func_80010DD8.c`](../../../matches/boot_tail/func_80010DD8.c).
This is static boot-tail evidence, not new cartridge coverage. No accepted lock,
ROM TU, target, linker script, or ROM hash is changed. The independent checker
owns promotion and the private ROM gate.

## Source cause

The old candidate used `count += 192 - count % 192;`. The matching source uses:

```c
count = 192 - count % 192 + count;
```

This exact expression also appears in the accepted adjacent
`cloud/work/frontier/w22c/sources/func_80010E80.c`. Both expressions are equivalent
unsigned arithmetic here, but their source expression trees differ. The direct
spelling causes stock IDO 7.1 to retain the copied inputs and shared quantum
register, including its divisor check. The old spelling is six instructions
short and differs in 36/42 positional words. Reverting just this line proves the
regression. No inline helper, false return type or formal, extra local, holder,
volatile, padding, register forcing, or patched compiler is in the deliverable.

The 23-entry bounded experiment ledger (`experiments.json`, replayed with
`experiments.py`) records the path to that result. Local/parameter lifetime
controls did not close it. Three hypothetical inline variants restored the
register-divisor structure but remained 32/42 words different and one instruction
short. Removing those helpers and spelling their expression directly closed the
function. The unused-inline control stays 36/42, so mere helper presence was not
the cause. These hypotheses are rejected as necessary explanations; they are
not claimed original helper identities or coverage. Experiment object identities
hash allocated sections plus symbolic relocations, excluding temporary-path
debug metadata, and reproduce across independent scratch directories.

## Native contract

- Public input is `(unsigned int rate, unsigned short duration)`, consistent with
  the native/accepted caller `func_80014198` in the same `lib_11640` TU.
- Zero duration clears only `D_800382F0`. It does not reset the old sample count or
  delay state and does not invoke the allocator.
- Positive duration multiplies in unsigned 32-bit arithmetic, takes the low
  product, divides by 1000, then rounds strictly up to the next multiple of 192.
  Even zero samples or an exact multiple gains a full block. This is not the
  ordinary ceiling operation.
- It writes the rounded sample count, clears the delay-state pointer, then calls
  the two-argument allocator `D_80038018(sample_count * 2, 0)`. The returned
  pointer, including NULL, replaces `D_800382F0` only after the call.
- The existing allocator prototype is shared by accepted neighboring functions;
  residual values in other argument registers do not establish extra formals.

## Verification

Run from repository root after the documented compiler/binutils setup:

```sh
IDO_DIR=tools/cloud/ido71 python3 tools/cloud/score.py fn \
  cloud/matches/boot_tail/func_80010DD8.c func_80010DD8 \
  --targets asm/us/boot_tail --flags '-g0 -O2 -mips2 -G 0 -non_shared'
python3 cloud/work/frontier/dot_static_10dd8_lifetimes_20261013/verify.py
python3 -m pytest tests/cloud/test_static_10dd8_lifetimes.py -q -rs
```

The input-bound receipt is `verification.json`; replay takes about one second
locally. `verify.py` freshly checks:

1. Actual TU-selected, fingerprint-identified stock IDO 7.1; canonical strict
   MATCH, 168-byte ELF function symbol, 8 zero alignment bytes, no excess nonzero
   instructions, own data, unresolved symbols, or masked relocations.
2. Independently GNU-assembled symbolic native target versus the compiler object:
   full allocated-section bytes and all ten symbolic relocation records agree.
   GNU ld resolves all four real globals at native addresses, and both linked
   bodies equal the protected target words. `SUBALIGN(4)` retains the native
   function address, which is not a 16-byte boundary.
3. 320 native/compiled-host-C/independent-contract comparisons (host `-O2` and
   `-O0` with undefined-behavior/bounds sanitizers): zero and high-bit
   duration, unsigned product overflow, quantum edges, normal audio rates, NULL
   and non-NULL allocator returns, prior state, callback-observed stores, stack
   canaries, and O32 callee-save state with all caller-save integer registers
   poisoned by the callback. All native instructions except the unreachable
   constant-divisor break execute. This is a bounded integer architectural
   model, not timing, audio output, hardware, or gameplay verification.
4. Negatives reject ceiling rounding, wrong allocation width, loss of the
   zero-duration branch, unsupported native opcodes, and a zero divisor.
   Short ELF extent and unowned-data mutations are refused. Reverting
   the shaping line restores the 36/42-word nonmatch.
5. The normal `rom_tu.h` context remains exact. A scratch-only promotion through
   the existing tools recompiles current `src/rom/lib_11640.c` before/after and
   preserves all 17,136 text bytes, allocated data and symbolic relocations.
   This checks every neighbor, not just the new function. The context tool's
   non-elided objdump row count is not used as function extent evidence.

Receipts bind the delivered source, verifier, semantic fixtures, focused test,
experiment ledger/script, accepted neighboring source, protected target
words, extent and required symbol addresses. They do not hash volatile tooling,
lock files, generated assembly text, or the whole evolving ROM TU. Normal replay
compares the entire receipt. Packet gates use explicit checks and also pass under
Python `-O`; they cannot be disabled by removing assertions. Scratch object,
assembly, compiler, and ROM bytes are not included in this packet.

## CI isolation regression

The first draft CI run exposed shared scorer state left at the static population,
which broke later game-code tests. `inputs()` and `run()` now scope and restore
both target population and compiler in `finally`, including nested calls and
failures. The one packet test checks successful and forced-failure restoration.
An ordered replay with the existing texture native-parser test reproduced the
old failure and passes with this fix. The matching source and evidence are
unchanged; the input receipt was rebound for the verifier/test correction.
