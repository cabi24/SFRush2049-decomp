# BT03 low resource dispatch and release

One locally strict O2 match covers **296 bytes**. Two complete bounded nonmatches
cover **696 bytes of research**. These are source matching results, not cartridge
coverage, promotion, or maintainer acceptance. Exact aggregate-head CI remains
central-coordinator work.

Independent source/native/ABI review PASS binds reviewed source commit
`acb8d95e471b4312677f973cc2c20366be586399` and tree
`24219e851857de49308e0e9200f4be9988eff3e5`; see `independent_review.json`.
The reviewer replayed all six scores, native layouts and 2,400 C89 cases and
ran an additional 2,400 actual-source ASan/UBSan cases successfully. A metadata-only
CSV newline correction preserved all original `e75f6a16` source bytes.

- Base: `16763d4b4dd2f6d131f5ba31246094054121fb62`.
- Branch: `dot/boot-tail-bt03-low-registers`.
- Owner: `/root/match_boot_tail_low_registers`.
- Exact target ranges: `[8001558C,800156E8)`, `[80015C0C,80015D68)`,
  `[800163A8,800164D0)`; 348, 348, and 296 bytes respectively.
- Central STATUS and every live claim were checked before selection; all three
  were open and unclaimed. `claim_scan.json` records the exact input hashes.
- Parent activated only these targets; central persisted that claim at `1535b793`.
- Only the named match source and this work directory are edited. Central STATUS,
  D10, specs, symbols, shared types, targets, scorer, locks and layouts are unchanged.
- No reference source was copied; names are behavior hypotheses from native bodies.

## Retained results

| Function | Behavior | O2 | O1 |
|---|---|---:|---:|
| `8001558C` | Locate a resource bank and packed program, dispatch with an optional guard | 33/87 | 83/87, 14 excess nonzero words |
| `80015C0C` | Decrement a packed record reference count and compact on zero | 23/87 | 85/87, 26 excess nonzero words |
| `800163A8` | Release matching subrecords in the first matching registry entry | MATCH, 74/74 words | 73/74, 4 excess nonzero words |

All six retained flag controls have no unresolved, unverified, or erroneous
relocations. Both retained O2 research bodies have no nonzero excess words, but
that is not an exact-geometry claim: `15C0C` ends one instruction later, with a
zero-valued final delay slot beyond the native extent. Its copy-loop discrepancy
is reflected in the strict differing-word count. Only `163A8` is submitted under
`cloud/matches/boot_tail/`. The locally matched row remains claimed for central
exact-head CI rather than claiming maintainer acceptance.

All functions use ordinary o32. The claimed bodies have no indirect calls,
floating literals or compiler-owned switch tables. Every referenced datum is an
address-named external runtime object, resolved by the unmodified scorer. No
local data relocation, substitute literal, or missing-table workaround is used.

## ABI and data evidence

`1558C` has five genuine arguments: two unsigned 16-bit IDs, a stream pointer, an
options pointer, and an unsigned byte choosing unguarded dispatch. Its four
register arguments have home stores; the fifth is read from its o32 stack slot.
The call in `19490` supplies two `lhu` IDs, the stream pointer, an options block,
and a stack value of one. `156E8` supplies zero in the fifth slot. Callee `178B0`
consumes all five ordinary arguments: two bank pointers, a packed program view,
a stream pointer, and nullable options. Its returned word is the caller-visible
handle or all-one failure sentinel. The resource header's used offsets are
4/6 for ID/type and 28/32/36 for relative bank/program offsets. Payload bases add
8 to those offsets. Programs advance 132 bytes and stop at ID 65535. The bank
search stops on its first matching ID even if its type/program is unsuitable.
Local opaque arrays describe real intervening record bytes, not frame padding.

`15C0C` is `int(unsigned short)`. Caller `151D0` normalizes the ID; the body reloads
that unsigned halfword after calling `14594`. `14594` and `145DC` are ordinary
`void(void)` guard operations; their real bodies and already matched C were read.
The table at `8003CE20` has 12-byte records, ID at +4 and reference count at +8.
Fields are accessed as packed halfwords; complete records copy as aligned words.
The source explicitly separates that packed field view from its word-aligned
storage view. The count is signed and valid runtime domains have nonnegative
counts and bounded active table storage. Reference underflow wraps as unsigned
16-bit arithmetic. Only the first matching record is affected.

`163A8` is `int(unsigned short)`, confirmed by caller `152A8` and the entry mask.
The registry has unsigned count at `800385A0` and 12-byte descriptors at
`800385A8`; each descriptor starts with a pointer to 28-byte subrecords. Each
subrecord has aligned ID/reference halfwords at +0/+2, size at +4, and release
data beginning at +12. ID 65535 terminates each subrecord list. Every matching
ID in the first matching descriptor is decremented; zero reference counts call
`14D08(data,size)`. `14D08` is an ordinary two-argument void no-op whose body
stores its two homes and returns. If all subrecords are unused, `161A0` receives
the original list pointer and removes that descriptor under the guard. Its real
body consumes a pointer in a0 and returns a Boolean word. There is no hidden
argument or speculative preserved-register ABI. All these helpers remain
read-only. The source preserves the post-callback reference reload and the
outer count reload when no descriptor matched.

## Verification

`preflight.json` pins the protected target inputs, scorer/setup, getter and all
24 files of the already approved IDO 5.3 installation. No compiler download or
copy was made. Manifest verification, all 439 extents / 99,120 bytes and strict
getter replay pass. `verify.py` replays all retained O2/O1 results and verifies
IDO-native record sizes/field offsets independently of host pointer width.
`verification.json` binds exact source hashes and all scorer fields.

`test_host.py` compiles the actual retained sources as C89 and runs 2,400 behavior
cases. The controls check guard balance, first-bank behavior, packed record
compaction, preserved opaque bytes, duplicate subrecord keys, reference wrap,
sentinels, genuine fifth-argument forwarding, callbacks and descriptor removal.
Directed fixtures ensure callback/removal paths are exercised; branch-coverage
assertions prevent random inputs from silently missing those paths. Host tests
validate modeled behavior, not MIPS layout or cartridge execution.

Run from the repository root, using the already pinned IDO directory:

```
python3 cloud/work/boot_tail/BT03-low-registers/verify.py --ido-dir "$IDO_DIR"
python3 cloud/work/boot_tail/BT03-low-registers/test_host.py
```

## Bounded controls and diagnosis

The initial `163A8` natural body was a strict O2 match and was frozen without
further source variants. O1 is a negative flag control.

`1558C` used an initial body and eleven directed natural-source controls. A
shared return after the guarded/unguarded branches moved 55 to 34 differing
words; comparison spelling moved that to 33. Formal-register hints, result
signedness, explicit key promotion, count initialization, and natural index
reuse did not close the residual and were rejected. No formal type sweep,
artificial local, dummy call, fake argument, assembly or compiler-context hack
was attempted. The retained 56-byte frame is native; the remaining signature
starts at narrow-formal allocation and cascades through temporary registers.

`15C0C` used an initial body and nine directed controls. Comparison spelling
moved 25 to 23 differing words. Aligned scalar/struct storage representations,
explicit word copies, pointer-driven copy loops and a direct packed array did
not improve the retained result. The packed-array control adds unaligned-copy
work and is rejected. The residual is the aligned record-copy lowering and its
loop scheduling, not a missing data relocation. Known eight-byte removal
plateaus elsewhere were not reopened.

Workbench diagnosis was run before controls and again after measured movement.
`diagnosis.json` summarizes its heuristic conclusions. Canonical target and
relocated candidate objects were temporary and are not committed. Its function
length/alignment and symbol-name heuristics are advisory; all numerical match
claims use the strict scorer's complete relocated comparisons.

Next hypotheses require new native/source evidence: the original resource
lookup's formal/loop-liveness spelling for `1558C`, and its original packed
record declaration/aligned-copy convention for `15C0C`. Another declaration or
flag sweep is not justified. Both are complete nonmatches with zero match credit.
