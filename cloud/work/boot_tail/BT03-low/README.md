# Packet 3: BT03-low, batch A

Base: `76780b3a1b3e26c54b86e1f153344e92dac15b20` (Packet 2 PASS), branch
`dot/boot-tail-p3-bt03-low`. Explicit source-stack dependency:
[PR #59](https://github.com/cabi24/SFRush2049-decomp/pull/59), head
`21e104a22575cf4d639261d2f6d9535913074e6a`, still unmerged at claim time.
Owner-merged #52/#54 are present through master `b16dd93b`.
Central STATUS/D10 updates belong to the coordinator; this packet provides
`status_delta.csv` rather than editing shared ledgers.

## Result

Nine strict local matching bodies, **260 B**, and one **COMPLETE-NONMATCH**,
48 B. These are source-matching bytes, not integrated cartridge coverage.
The manifest and existing `func_80010A00` getter were freshly replayed before
work. `verification.json` records exact source/scorer/manifest hashes and
both optimization levels. All nine submissions have zero differing words,
zero excess words, and no unresolved, unverified, or errored relocations.
Remote CI and independent review remain required before VERIFIED-BODY credit.

| Address | Bytes | Behavior supported by native body | O2 | O1 |
|---|---:|---|---:|---:|
| 80014624 | 44 | blocking receive on queue D_80038368 | MATCH | MATCH |
| 8001467C | 48 | normalize byte 0 of a 104-byte slot to boolean | 2/12 | 3/12 + 2 excess |
| 800146AC | 8 | return zero | MATCH | MATCH |
| 800149BC | 32 | narrow index to u16 and call 80011C84 | MATCH | 6/8 |
| 800149DC | 40 | clear slot byte 1 | MATCH | MATCH |
| 80014A04 | 8 | unused incoming word parameter, no body effect | MATCH | MATCH |
| 80014BB0 | 40 | clear slot byte 0 | MATCH | MATCH |
| 80014C18 | 40 | return slot word at +20 | MATCH | MATCH |
| 80014C40 | 32 | write back sample buffer, twice the sample count | MATCH | 7/8 + 4 excess |
| 80014D1C | 16 | set D_80038291 to one | MATCH | MATCH |

## ABI, storage and flag audit

- `14624` consumes no incoming argument and passes queue address, null message
  destination, and blocking flag 1 to counted-static `osRecvMesg`. Its caller
  `13DEC` does not use the result. This is a simple queue wrapper, not an
  asserted recovered SDK identity; no third-party source was imported.
- `146AC` returns zero in v0; its caller `202C4` passes no arguments and tests
  that result. Signedness is not distinguishable from this constant body.
- `149BC` has a full-word incoming index and explicit low-16-bit conversion.
  There is no incoming-home spill, unlike a narrow formal. Its callee `11C84`
  consumes the u16 index and updates the indexed slot, and caller `1A658`
  ignores the return. The wrapper is therefore declared void.
- `149DC`, `14BB0`, and `14C18` share the same partial 104-byte slot layout:
  byte 0, byte 1, word +20. Unknown byte arrays describe unrecovered storage
  in the observed stride, not stack padding or synthetic live variables.
  `14C18`'s caller `1C3CC` treats the word as an unsigned sample position.
  Slot names and the audio role remain HYPOTHESIS, not recovered source names.
- `14A04` natively stores the incoming a0 to its argument home then returns.
  One unused word formal is directly evidenced by that store; there are no
  known direct callers, and signedness is unresolved. No extra fake formal
  was invented to control allocation.
- `14C40` receives buffer and sample count; its caller `1C3CC` derives both
  from the same sample-ring state. The callee is counted-static
  `osWritebackDCache`. Doubling uses unsigned arithmetic before the ABI int
  byte-count conversion. No unused a2/a3 formals are inferred.
- `14D1C` only stores one byte and has no arguments.
- O2 is selected for all submissions. The native frameless accessors and
  optimized wrapper argument setup support it. Seven bodies are identical
  under O1, so those tiny bodies alone do not identify the original level.
  `149BC` and `14C40` distinguish O2 from O1; this is a flag observation, not
  proof that all BT03 functions came from one translation unit.

## Bounded nonmatch: 8001467C

Whole body is archived as `func_8001467C.c`, never submitted under matches.
O2 differs only at boolean-result register selection: native uses v0, while
candidate uses a temporary followed by the byte-return mask. O1 also emits
two excess words. No missing data/table proof or relocation remains.

Workbench `diagnose` was run before refining the first residual and reported
allocation mismatch. Its target-object relocation warning is expected: the
read-only native words were placed in a temporary object, while the candidate
contains symbolic relocations; strict `score.py` resolved them successfully.
Fifteen natural source forms were bounded: direct comparison; byte-local
normalization; if/else returns; ternary; integer/byte boolean locals; explicit
byte return conversion; double negation; greater-than-zero; pointer-local
field access; assignment-return form. None improved on 2/12. Artificial
register pins, dummy formals, no-op expressions and padding were not tried.
The canonical direct comparison is retained. Next useful hypothesis: recover
an authentic reference declaration/expression for the boolean-return ABI,
or inspect compiler intermediate allocation with unchanged target/source
semantics. Do not restart a blind type/flag sweep.

`14C18` initially used a byte-pointer cast and differed in 7/10 allocation
words. Diagnose identified crossed pointer/index operand loads. The shared
partial typed slot layout fixed that residual without changing semantics.

## Reproduce

Set `IDO_DIR` to the pinned IDO 5.3 toolchain, then from the repository root:

```
(cd asm/us/boot_tail && sha256sum -c SHA256SUMS)
python3 cloud/work/boot_tail/BT03-low/verify.py
python3 cloud/work/boot_tail/BT03-low/test_semantics.py
python3 tools/cloud/check_submissions.py --base 76780b3a --head HEAD
```

The host C89 tests cover queue arguments, narrowing, slot stride and adjacent
field preservation, sample-byte conversion, the constant setter/return, and
all 256 byte values for the nonmatching boolean source. Host tests establish
only the documented C behavior, not native execution or ROM identity.
No ROM/image bytes, native dumps, objects, protected tools, locks, layout,
runtime-image matches, or farm files are part of this packet.
