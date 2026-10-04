# BT03 initializer store-rotation retry

**Strict whole-function MATCH: one new body / 432 bytes.** This is source
matching evidence, not cartridge integration or ROM coverage.

Exclusive claim: `func_8001C1D8`, native interval `8001C1D8–8001C388`.
The clean retry base is `8cccffb88f99656d3948a71e15517be63fb2405a`, reusing the
closed caller/stream worktree on `dot/boot-tail-bt03-store-rotation-retry`.
The existing [reviewed initializer packet](../BT03-high-runtime-lists/README.md),
its four historical forms, tests and review remain frozen. Central cut15 is
unchanged and is not included in this retry's verification claim.

## Evidence-led repair

The full native/source/control history was inspected before editing. Earlier
relative channel-bank indexing reduced 7 differences to 4; unsigned loop-index
and signed halfword controls then plateaued. Fresh stock IDO compilation and
unchanged workbench diagnosis reproduced four differing words, no excess, the
same registers/control flow/frame, and an exact 432-byte ELF function extent.
All four differences were the final 16-halfword table-clear stores. Native
scheduling put the first unrolled store in the branch delay slot; the retained
source put the last there.

The archived loop put its header and store on one physical line. Stock IDO
pre-assembler output assigned all four generated stores to that line. The
single new control expresses the same scalar loop with an ordinary braced body
on the next line. The original store now carries the body line while copied
stores carry the header line, and the stock assembler produces the native
rotation. All 108 relocated words match, with a 432-byte STT_FUNC extent.
Two post-match orthogonal controls isolate the cause: braces on the original
single line retain 4/108, while a multiline body without braces also matches.
Their exact replacement recipes and hashes are replayed by the verifier.
No pointer rewrite, manual unrolling, fictional field grouping, added operation,
new local, type change, volatile qualifier, fabricated argument, ABI trick,
compiler flag sweep or compiler modification was needed.

This source-line-provenance hypothesis was informed by the repository's
vendored workbench field guide/compiler laws and the externally read
[Snowboard Kids IDO notes](https://github.com/cdlewis/snowboardkids-decomp/blob/main/DECOMPILATION_LEARNINGS.md).
The actual Rush `-O2 -mips2` stock pipeline independently establishes the effect;
the external notes are only a lead and are not copied here.

## Semantics and validation

The exact source delta is mechanically checked as one scalar `for` statement
gaining a braced, multiline body. Consequently the genuine configuration-word
ABI, all existing callbacks and alias behavior, every store, all untouched
bytes, and the prior domain are unchanged. The old signed halfword declaration
and naturally aligned word-array slice remain as reviewed.

The actual new source runs the original C89 ASan/UBSan fixture. It checks the
entire 32-voice and 32-channel result byte for byte, untouched bytes, all native
special channel banks, the 16-halfword clear, exact callback order, and callback
mutations before the clear and after final initialization. LeakSanitizer alone
is disabled in this execution environment, as in the frozen fixture.

`verify.py` pins both source hashes, validates the immutable 439-function /
99,120-byte target population, independently obtains ELF function size, and
replays the old and new full-function O2/O1 results. O2 must be respectively
4/108 and strict 0/108 with exact 432-byte extents and no excess, unresolved,
unverified or relocation-error output. O1 remains a rejected control.

```sh
python3 cloud/work/boot_tail/BT03-store-rotation-retry/verify.py
python3 -m unittest discover -s cloud/work/boot_tail/BT03-store-rotation-retry -p 'test_*.py' -v
```

The packet contains source, tests and source-free receipts only. Raw native
listings, objects and pre-assembler output are temporary and untracked. No
targets, scorer, symbols, layouts, locks, shared headers, earlier packets,
central ledgers, restricted helpers or implementation bodies are modified.
Independent exact-source review and aggregate exact-head CI are required
before checker-owned integration; this packet does not merge anything.

Independent review PASS binds source commit
`a87864a4ab2630771d527fb460a010dcd9588d96`, tree
`56ca28d383f7487e16d92e9edc5336f6ac94cbe6`. The reviewer independently replayed
all four O2/O1 rows and both orthogonal controls, reproduced both new tests and
all six prior ASan/UBSan groups, checked the complete 108-word native body,
caller and four callee ABIs, and compiled 32 native layout assertions. Separate
full-text relocation and ELF review confirms exactly 432 bytes with no masks,
unresolved references, errors or padding credit. Stock pre-assembler instruction
sequences are identical; only store source-line ownership changes. The receipt
is `independent_review.json`; the C source remains byte-for-byte frozen.
