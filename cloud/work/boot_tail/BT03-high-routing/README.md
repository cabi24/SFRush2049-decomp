# BT03-high routing, tagged state and identifier helpers

Five first-form strict O2 matches, **704 B / 176 words**, and one complete
NONMATCH, **128 B**. Exclusive central activation `f46ac52e` covers exactly this
six-member packet. Intended branch `dot/boot-tail-bt03-high-routing` is based on
master `301d9e7552ad4fd7f54a38796db84671e1000d35`.

Candidate work began in an isolated Git-free staging directory during coordinated
shared-object maintenance. Only the named source/packet files were written;
canonical inputs and pinned compiler were read through unchanged references.
`staging_preflight.json` records manifest, all439 extents/99,120B and getter proof.
The final branch is created from the exact base after the maintenance hold, and
source hashes plus all results are replayed there before handoff.

| Function | Bytes | O2 | O1 |
|---|---:|---|---|
| 800175B4 |144|MATCH|36/36 + 12 excess|
| 80018F20 |132|MATCH|32/33 + 14 excess|
| 800190AC |152|MATCH|37/38 + 26 excess|
| 8001A5D8 |128|19/32|32/32 + 5 excess|
| 8001B968 |144|MATCH|35/36 + 1 excess|
| 80020494 |132|MATCH|22/33 + 1 excess|

The five accepted bodies have complete relocated equality and zero nonzero
excess, unresolved symbols, unverified references or errors. They contain no
callee bodies, fake formal, keeper, local padding, forced register or assembly.

## Actual operations, ABI and valid-input limits

`175B4` obtains the old unsigned sequence counter, increments it modulo32bits,
then retains its low31 bits. It retries a reserved all-ones candidate or any
identifier used by an active one of eight 4088-byte records, finally writing the
selected record's identifier. Its slot input is a genuine full word, valid0..7.
Inactive records do not reserve an identifier. The counter's wrap and retry are
unsigned operations, not signed-overflow assumptions. Independent caller audit
also finds178B0 scanning indices0..7, rejecting8, then passing that selected
index to175B4 at caller+0x440; the documented slot domain has native caller proof.

`18F20` and `190AC` call real `17644(u32)`. That translator returns -1 or a
validated row0..7 with an optional bit31 tag. Both reject -1 and explicitly strip
the tag before array access. Untagged stores go to halfword+0xFC2 or word pair
+0x110/+0x114; tagged stores go to halfword+0xFEC or word pair+0xFE4/+0xFE8 and set
flag0x20/0x10 at+0xFEE. Both halves preserve unrelated bytes. The shared natural
4088-byte record's layout is checked by host offsetof/sizeof and IDO equality.

`1B968` forwards a real word key to the previously reviewed `1EDF4(u32)` lookup,
then rejects an all-ones result, stale full identifier or state flag2. Otherwise
it returns packed halfword+0xC2 from the 416-byte state selected by the low ID byte.
The existing lookup's signed-word return declaration is preserved; bitwise tests
and full-word equality retain the actual identifier bits. As in the native
routine, registered handles must identify an allocated voice record; no arbitrary
malformed handle or unallocated low-byte index safety is claimed. No lookup body
or resource table is reconstructed here.

`20494` has real byte/halfword/byte/byte inputs. Under the runtime gate it enters
through no-argument14594, optionally calls actual five-input1B9F8 for selectors21
and22 with fourth/fifth zero, and exits through145DC. Prior full callee audit
proved the genuine incoming fifth slot. The helper is not falsely shortened or
padded, and both optional calls preserve their original order.

`1A5D8` reads a packed full word at state+0x5C. Actual `1E50C(u8,u32)` returns a
word; only its low halfword forms this routine's16.16 base. A nonzero fractional
part calls real `1E440(u16)`, whose return paths normalize to u16. Difference,
product and addition are unsigned low-word arithmetic. The external helpers'
floating tables and constants do not become caller literals or fake float
parameters. No table mapping or helper implementation is asserted by the host
synthetic contracts. This complete body remains NONMATCH at19/32.

## Bounded interpolation diagnosis

O2 precedes O1 for every source. `diagnosis.json` records the unchanged workbench
on the initial near-match before refinement. The initial body has the correct
40-byte frame and word count but differs in narrow-argument lowering, scalar
allocation and stack-home order. Workbench relocation-layout warnings are not
proof; strict relocated scoring remains authoritative.

Four natural interpolation forms were tried: fixed-point result with a scoped
whole value; implicit byte argument conversion; ordinary outer whole-value
computation; and an explicit u16 base followed by result construction. Best O2
remains19/32, so the final explicit-base body is frozen. No declaration sweep,
artificial stack slot, register/K&R experiment or extra formal was tried. The
three rejected controls remain separately hash-bound with zero matching credit.
All other five members matched their first source form.

## Tests and preservation

Six strict-C89 ASan/UBSan tests cover identifier collisions/wrap/inactive slots,
all sixteen tag/row combinations and invalid sentinel for both setters, packed
416-byte getter stale/flag conditions, enabled/disabled optional call combinations,
and low-word interpolation with high helper return bits and boundary fractions.
Whole-object byte comparisons check untouched fields. The getter tests use32
allocated records and valid handles. Test helpers are synthetic contracts only.
One initial wrap-fixture expectation incorrectly counted the reserved all-ones
entry as a low-ID collision; the test expectation was corrected to seven and all
six tests passed without a candidate source change. LeakSanitizer alone is
disabled under ptrace; these harnesses allocate no heap.

```sh
python3 cloud/work/boot_tail/BT03-high-routing/verify.py
python3 cloud/work/boot_tail/BT03-high-routing/verify_controls.py
python3 -m unittest discover -s cloud/work/boot_tail/BT03-high-routing -p 'test_*.py' -v
```

Only five named matching files and this research packet are published. Prior
source packets and central STATUS/D10, targets/scorer, symbols/layout/locks,
runtime image/farm and forbidden helper work stay unchanged. No ROM bytes, raw
native listings, objects, credentials or unrelated private material is included.
Independent source/ABI review and exact aggregate-head CI precede checker-owned
merging. Local matches are not cartridge promotion.

Independent paired review PASS is bound to source commit
`a110153cc17fe562396a133a67053bc8c071ccd9`, tree
`dc5003cdd4b87b5ff54538b8537b1d59b693f40a`. All twelve final rows, six archived
controls and all six sanitizer tests passed independently. The source blobs and
verification receipt at that immutable head equal the independently reviewed
staging hashes exactly. Actual native/source ABI, caller slot bounds and stated
handle domains were approved; `independent_review.json` records the result.
