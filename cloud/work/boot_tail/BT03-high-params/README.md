# BT03-high parameter and controller helpers

Four strict O2 matches, **448 B / 112 words**; two complete nonmatches, **208 B**.
Exclusive central claim `03098119`, branch `dot/boot-tail-bt03-high-params`, base
master `301d9e7552ad4fd7f54a38796db84671e1000d35`.

| Function | Bytes | O2 differing words | O1 differing words |
|---|---:|---|---|
| 80018E6C | 72 | MATCH | 16/18 + 1 excess |
| 80018FEC | 128 | MATCH | 31/32 + 6 excess |
| 80019ED0 | 120 | 7/30 | 29/30 + 1 excess |
| 8001B154 | 124 | MATCH | 29/31 |
| 8001B744 | 124 | MATCH | 31/31 |
| 8002021C | 88 | 12/22 | 19/22 |

All accepted words have complete relocated equality, with zero nonzero excess,
unresolved symbols, unverified references or relocation errors. The full final
bodies receive twelve O2/O1 comparisons. Archived controls receive no extra body
or match credit. No candidate extent or protected input changes.

## Actual contracts and source domains

`18E6C` walks all eight 4088-byte voice records and passes each full word at +0
to genuine `18D40(u32)`. Identifiers are loaded when visited, so helper mutations
of later records remain visible. The record layout is shared with `18FEC`:
identifier +0, active byte +0xFC0 and flag byte +0xFEE. The complete stride and
these offsets are checked on the host and by native IDO equality.

`18FEC` calls actual `17644(u32)`, whose output is -1 or a validated row0..7
plus an optional high tag bit. It rejects -1, sets the active byte for an untagged
row, or explicitly removes the tag and clears flag bit3 for a tagged row. Unlike
an unguarded tagged subscript, every retained array index is the validated row.
All sixteen row/tag combinations and the invalid sentinel are tested.

`1B154` conditionally computes the native rate word and calls no-argument
`1A658`, whose entry initializes its work from globals. The native multiply is
low32 and its division is signed. The source therefore multiplies as u32, uses
the documented N64 two's-complement u32-to-s32 conversion for division, and shifts
the quotient as u32. It never invokes signed multiplication/left-shift overflow.
Valid division requires a nonzero denominator and excludes INT_MIN/-1; the
native explicitly traps those invalid divisions. Portable C trap emulation is
not claimed. The host test uses independent wider-long arithmetic across positive,
negative and wraparound fixtures, retaining the zero-rate no-call path.

`1B744` has exactly two genuine opaque state pointers. It invokes real
`20DA8(u8, state*, state*)` five times for controllers7,10,91,128,132. The callee
reads the packed full identifier at +96 from both pointers and masks the first
byte formal; no artificial scalar argument or copied callee body is supplied.

`2021C` has a genuine byte channel and signed-halfword replacement. It keeps only
the low four channel bits, enters through no-argument14594, reads/replaces one
of sixteen signed halfwords, leaves through145DC and returns the prior halfword.
The prior value is an actual result that survives the final call, not a keeper.
Both synchronization helpers were previously audited across their full bodies.
The native in-place argument mask versus the compiler temporary remains open.

`19ED0` has three genuine byte inputs. It rejects channel255, checks real
`21028(channel,set)`, normalizes the key to seven bits, calls real three-byte
`19C8C`, and preserves either the returned identifier or the all-ones sentinel.
Both helper entries home/read exactly those narrow slots; no formal was widened
or invented to influence register allocation. Array validity inside those
external services remains their established caller contract.

## Bounded controls

All seeds used O2 first and O1 second. `diagnosis.json` records workbench results
before refinement, plus the improved19ED0 checkpoint. Raw objects and instruction
dumps remain outside the packet. Workbench relocation-layout warnings are not
match evidence; strict relocated scoring is authoritative.

- `18E6C`, `1B154`, `1B744`: first-form O2 matches.
- `18FEC`: initial 10/32 allocation residual arose from assigning the untagged
  row back to the local. Using the natural inline untagging subscript closed it.
  Two forms total; full validated-row semantics are unchanged.
- `19ED0`: ordinary parameter normalization improves15/30 to7/30; inline
  normalization gives the same7/30 and stops. Three forms total. The remaining
  home/mask copy and scheduling require authentic source/compiler context.
- `2021C`: the initial12/22 residual is the known narrow-formal in-place-mask
  plateau. No repeated register/K&R/prototype/padding sweep was attempted.

Six C89 ASan/UBSan tests check live identifier mutation, all row/tag addresses,
low-word signed-division arithmetic, exact ordered five-call forwarding,
all256 masked channels with synchronization mutations, and all256 normalized
keys over disabled/success/sentinel returns. Test helpers are synthetic contracts;
no original helper implementation or cartridge behavior is claimed. LeakSanitizer
alone is disabled under ptrace; the harnesses allocate no heap.

```sh
python3 cloud/work/boot_tail/BT03-high-params/verify.py
python3 cloud/work/boot_tail/BT03-high-params/verify_controls.py
python3 -m unittest discover -s cloud/work/boot_tail/BT03-high-params -p 'test_*.py' -v
```

Fresh manifests, all439 extents/99,120B and the existing getter pass. Only the
four matching submissions and this packet change; prior sources, central STATUS
and D10, targets/scorer, symbols/layout/locks, runtime image/farm and forbidden
helper remain untouched. No ROM/raw assembly/object/private data is published.
Independent source/ABI review and exact aggregate-head CI precede checker-owned
merging. This is local matching evidence, not cartridge promotion.

Independent paired review PASS at source commit
`740aa62287eb84fec297484acb08d07bf5826360`, tree
`2848a5511fa4e73aab62fb598ea73363d77fde57`. All twelve final rows, eight archived
controls and exact source hashes replayed independently, and all six sanitizer
tests passed. Actual whole-body/source ABI, validated tag bounds, live reads,
rate arithmetic and genuine two-pointer forwarding were approved. The receipt
is `independent_review.json`; source bytes remain frozen.
