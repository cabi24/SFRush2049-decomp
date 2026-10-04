# C13 byte-clear scheduling retry

Exclusive central retry claim: `func_80020820`, 484 bytes. Branch
`dot/boot-tail-c13-byte-clear-retry` begins at clean recovered checkpoint
`7f751e7523776d8e5f35899bacd9f240ee32bb05`, tree
`b4f4af3fc313eb05f89dfc0c92263eff9d0d7707`. The published
`C13-controller-pair` packet, checkpoint branch, shared ledgers and cut15 remain
unchanged. This packet contains one new matching source; it does not claim ROM
coverage or maintainer promotion.

## Result and narrow source delta

The canonical `cloud/matches/boot_tail/func_80020820.c` is strict relocated
**MATCH: 0/121 words**, with an exact **484-byte ELF function symbol**. All
relocations resolve over the entire function, no relocation masks or unverified
fields are used, no extra words are present, and section-alignment bytes are zero.
The unchanged archived source reproduces 10/121 with the same function size.
O1 remains a nonmatching explicit control at 120/121 and 448-byte function size.

Only the two existing one-line scalar `for` loops become ordinary three-line
braced blocks. No expression, type, declaration, ABI, loop limit, call argument,
call order or storage selection changes. The first new form matched. Two
orthogonal post-match controls distinguish the cause: same-line braces remain
10/121; multiline bodies without braces also match 0/121. All controls have
source hashes, full scores and exact function sizes. No repeated exhausted loop
form, arbitrary blank-line padding, fake line directive, keeper, dummy local,
new formal, volatile trick, inline assembly or compiler modification is used.

## Compiler evidence

The prior six source forms and twenty scored rows were inspected and the unchanged
workbench diagnosis was rerun before source changes. It reports the same 121-word
geometry and equal 32-byte frames; positional strict scoring identifies only the
two byte-clear schedules. The heuristic workbench classification does not itself
prove the scheduling mechanism. The new concrete lead was the independently
observed stock-IDO source-line scheduling closure of `8001C1D8`.

Stock IDO 5.3 `cc -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul -S`
shows the baseline's four unrolled stores owned by the same header/body line
(13 for the regular row and 15 for the external row). Ordinary multiline source
separates the original store onto body lines 14 and 18, while each loop's other
three unrolled stores belong to header lines 13 and 17. The final assembler moves
the original store into the backward branch delay slot and compensates its
address for the prior pointer advance, exactly reproducing both native loops.
This transfer is measured at this function and these flags, not claimed as a
universal IDO formatting rule. `verify.py` reproduces the line-ownership records;
compiler listings and objects are temporary and not published.

## Native and semantic contracts

The complete native body, retained source, old controls and reviewed helpers
were inspected. Two actual unsigned-byte inputs select a channel and set. Set255
selects the external table; otherwise the real regular table has sixteen channels
per set. Rows have134 bytes and set strides are2,144 bytes. The function clears the
entire selected row before ten controller setter calls, in native order, followed
by the actual three-byte `20F4C(channel, set, 255)` and
`20FDC(channel, set, 0)` calls. The real `24988` caller supplies a registered external
slot and set255. Unknown table contents and storage stay external. No arcade
source attribution is claimed; that checkout is unavailable here.

The valid registered-row domain from the prior reviewed packet remains the
precondition. No arbitrary malformed index safety or fallback is invented.
The actual-source strict-C89 ASan/UBSan fixture reuses the reviewed reset case body
with an explicit canonical-source include:320 regular/external channel-pattern
cases verify all134 bytes clear before the first helper, all twelve calls and
arguments, a deliberate helper mutation, and all untouched rows. LeakSanitizer is
disabled under ptrace; this fixture performs no heap allocation.

A new fail-closed bounded MIPS-II replay executes the canonical native words and
freshly compiled fully relocated final words through640 cases (the same320 valid
row/pattern combinations with zero and nonzero incoming upper argument bits).
It checks complete clear coverage, helper order and byte normalization, memory
results, untouched rows, delay slots, and O32 stack/callee-save restoration under
volatile-register clobbering. External helper callbacks are synthetic contract
checks; they are not reconstructions of helper internals. Unsupported instructions,
unmapped accesses, uninitialized stack reads and unsupported control flow fail.
A negative unsupported-opcode test confirms fail-closed behavior.

## Reproduction and scope

Use the existing pinned compiler through `IDO_DIR` and run:

- `python3 cloud/work/boot_tail/C13-byte-clear-retry/verify.py`
- `python3 -m unittest discover -s cloud/work/boot_tail/C13-byte-clear-retry -p 'test_*.py' -v`
- `python3 cloud/work/boot_tail/C13-controller-pair/verify_controls.py`
- `python3 -m tools.conveyor.pipeline.lock check --quiet`

`input_pins.json` covers the old packet, scorer, setup, immutable target manifest,
inventory, locks and compiler files. All439 canonical target extents/99,120 bytes
are validated. Only this new packet and the one canonical C file are changed.
No protected inputs, production gates, generated layout, symbol tables, targets,
shared ledgers, prior sources, restricted helper work or T050 body are modified.
No ROM bytes, raw assembly dumps, objects, credentials or unrelated private data
are published. Independent immutable-head review and exact integration-head CI
remain required before central acceptance. Merging belongs to the user's checker.

Independent paired review **PASS** binds immutable source commit
`7ed7c730b8f3faf7bf241744080bdd76745ea913`, tree
`e697d64a7086bdaddcb38b92d5dac62b56cd53aa`. A separate ELF parser reproduced
484-byte function size and12 zero alignment bytes. The reviewer independently
replayed ten new O2/O1/control rows, all four old final rows and twenty old
controls, all three new test groups and both old sanitizer groups; full native,
caller and three-callee ABI inspection passed. Stock preassembler instructions
are identical before/after formatting; measured line ownership differs as
reported. The reviewer also passed the protected-scope and submission gates and
all161 locks. `independent_review.json` retains the source-bound findings.
Integration-head hosted CI and merging remain separate; no source changed after
review.
