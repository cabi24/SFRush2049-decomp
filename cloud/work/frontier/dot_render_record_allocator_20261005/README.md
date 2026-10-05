# Renderer-record allocator: typed reconstruction, NONMATCH

Target: `func_800A79F4`, **[0x800A79F4, 0x800A7AE4), 240 bytes / 60 words**.
Base: `e0e734babdac3c6a79d2f87f7f895e34aa170148`, 2026-10-05.
**NONMATCH. Zero matching, accepted-byte or ROM-coverage gain.**

Refreshed onto `31b2799ebb821a7ec0983a34d2611bba2cedaab9` after PRs #107/#108
were integrated. Selected target, setter, caller and scorer inputs are unchanged;
the complete compiler and behavior receipt was replayed successfully there.

## Useful result

The compact complete source is **23/60 differing words at O3**, at exactly the
native 240-byte ELF function extent. The first 128 bytes agree; payload argument
allocation and store scheduling remain different. All eight relocations resolve.
An independent GNU link reproduces exactly the same 23 differing offsets.
There are no owned literals, data, tables or trailing text alignment bytes.

The actual native body, GNU-linked candidate, unchanged host C89+UBSan and an
independent record oracle agree on **390 cases / 780 native executions**. Every
one of the native and candidate bodies' 60 instruction offsets executes. Five
wrong-contract source mutations are rejected. This proves the bounded behavior
below, not instruction equality or original C type recovery.

## Why this source experiment was justified

The allocator's seven final payload fields correspond exactly to the real
`Input_InitPadHandlers` interface: record index, texture/index halfword, two
opaque native words, two coordinate halfwords and width/height halfwords.
The accepted setter is already genuinely inlined in `Input_ApplyPadConfig`.
Those accepted sources and the actual `func_800B3704` call establish a concrete
composition hypothesis rather than an invented helper.

The caller's identification as N64 NewBlit is grounded in the previously
reviewed [NewMultiBlit packet](../dot_new_multiblit_20261005/README.md) and
[arcade LIB/blit.c NewBlit](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/LIB/blit.c#L129-L157).
This primitive is a N64 renderer-record allocator; a direct arcade implementation
of its expanded seven-value interface was not recovered.

The archived [W2 pilot](../../workbench_pilot_W2.md#func_800a79f4---not-matched-best-strict-4460-aligned_total-41)
reported 44/60 after direct-source controls and artificial priority probes. That
44-word result is historical, not a fresh replay in this packet. The current
near-miss seed has obsolete global-context declarations and different field
views. No claim is made that absence from the searched archive proves freshness.
The new direct source uses a natural counted first-free scan and a native-sized
record. It contains no extra locals, dead checks, padding, volatile qualifiers,
stand-ins or compiler/keep-list changes.

The unsigned payload parameters are **bit carriers**, consistent with the
accepted setter. They make subtraction and truncation defined for every input
word. They do not prove the old opaque-pointer prototypes were semantically
wrong or identify the original source typedefs. No pointer payload is dereferenced.
A fixed historical-caller-type control remains a NONMATCH.

## Fixed outcomes and stopping point

| Condition | Differing words | Complete ELF function bytes |
|---|---:|---:|
| Retained direct O3 source | 23/60 | 240 |
| Same source, O2 | 37/60 | 244 |
| Genuine accepted setter body before remaining initialization | 58/60 | 252 |
| Genuine accepted setter body after remaining initialization | 59/60 | 260 |
| Historical caller's u16/opaque-pointer/signed payload types | 48/60 | 244 |

Both helper compositions preserve the complete accepted setter as strict MATCH.
They fail to improve the allocator, and their full extents fail independently of
the scorer's count of nonzero excess words. No further spelling, allocation or
formatting sweep was run. Workbench diagnosis preceded these controls and
identified the first retained-source divergence in payload allocation. Its
symbol warnings arise from comparing a raw-target object with a relocatable
object; the separate GNU proof resolves the actual relocations.

A genuine two-body O3 group keeps the allocator and its only observed direct
caller, the full archived NewBlit reconstruction. The allocator remains 23/60,
and NewBlit remains its existing 14/57 at 228 bytes. The caller body is unchanged.
Only its old prototype is normalized in the generated temporary group to the
new bit-carrier interface; the exact old/new declarations and generated-source
hash are recorded. The original caller file is not edited, and no NewBlit
improvement or acceptance is claimed. This is a scoped compiler-context check,
not a shared-type or full-game closure proof.

Reopen for a concrete original declaration, caller convention, or independently
supported source boundary that predicts the remaining payload-register choices.
The failed setter composition is not a reason to invent another initializer.

## Recovered bounded contract

- Search records below the signed high-water count for the first signed state
  byte equal to two. Other byte values, including zero and negative values,
  are occupied. The record stride is 32 bytes; capacity is 200.
- If the selected index is at least 200, return minus one without any writes.
- Otherwise, increment the count when selecting its end, and raise the separate
  maximum only when the new count exceeds it.
- Store the two native words and five payload halfwords, clear the halfword at
  offset 14, initialize alpha to 255, clear flip/state and both leading crop
  coordinates, and store height-minus-one and width-minus-one in the trailing
  crop coordinates.
- Preserve every other record, the selected record's flags byte at offset 23,
  the argument-home area, stack pointer and callee-saved registers. Return the
  selected index.

Fixtures cover each possible reusable slot, an exhausted pool, all occupied
state-byte classes, multiple free slots, count and maximum boundaries, low-half
truncation, zero/extreme unsigned dimensions and random 32-bit payloads. Counts
minus three through minus one deliberately preserve the native malformed-count
behavior (index zero, increment the negative count); they are not evidence that
such counts occur in gameplay. The supported domain is a fully accessible
200-record pool and count -3..200. Out-of-bounds counts, concurrent mutation,
invalid memory, rendering and actual caller execution are not tested.

The five host-source counterexamples change the reusable state, reduce capacity,
omit maximum growth, exchange a crop axis, or omit clearing flip. The interpreter
also rejects unknown instructions. Full output memory and scalar state are
compared, not just the return value.

## Scout closed before this target

`func_80109468` (procedural minimap, 1,528 bytes) was inspected first. The current
w6c full source and a single control replacing its explicit visibility block
with the pinned arcade `Hidden` body both freshly gave 249/382 with equal function
extents. The helper inlined without changing the callback's residual. No allocator
sweep or new matching claim followed. That target is not reserved by this packet.

## Reproduce

Use the unchanged project IDO 5.3 and GNU MIPS environment, from the repository root:

```sh
python3 cloud/work/frontier/dot_render_record_allocator_20261005/verify.py
python3 -m pytest tests/cloud/test_render_record_allocator.py -q -o addopts=''
```

Normal verification compares the complete source-bound functional receipt and
never rewrites it. `--record` deliberately creates a new reviewed receipt;
`--output PATH` writes a separate result. Temporary native words, disassembly,
objects and linked binaries are generated locally and never submitted.

Nine focused tests pass. The final packet/scorer/submission/protected-path test
selection passes **716 tests**, with no skips or failures. All **402 static locks**
pass their integrity guard. The full local repository suite and remote CI were
not run for this packet. No protected target, source, lock, symbol, production context,
recipe, scorer or build gate changes. No source splice, complete shadow unit,
linked game image, compressed-stream or ROM SHA-1 claim. Independent review and
any future integration remain with the checker.
