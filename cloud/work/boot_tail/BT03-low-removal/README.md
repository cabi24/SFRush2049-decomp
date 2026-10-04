# BT03-low-removal: complete bounded nonmatch research

Three fresh Packet 5 functions, 872 bytes. **Zero verified-body or cartridge credit.**

| Function | Bytes | Role hypothesis | Final O2 residual | Final O1 residual |
|---|---:|---|---:|---:|
| func_800158D8 | 308 | Release a referenced packed resource ID and compact when its count reaches zero | 8/77, no excess | 74/77, 26 excess |
| func_80015F28 | 308 | Equivalent release operation for a second resource table | 8/77, no excess | 74/77, 26 excess |
| func_8001661C | 256 | Remove an ID from a packed record table | 31/64, no excess | 63/64, 16 excess |

All final comparisons resolve every relocation, with no unverified data. O2 preserves the native frames (24, 24, and 32 bytes), body lengths and overall control structure. O1 is a negative control: it expands instruction streams and excess words. Sources are standalone C89/IDO-compatible and remain exclusively in this research packet.

## Freshness and authorization

The original 64–255-byte low band had no fresh functions. Parent explicitly activated these three 256–1023-byte functions after the entire small band was attempted. Central recorded the exact set at claim commit `ce487f1e`, initially labeled BT03-low-records, then aligned the label to BT03-low-removal. No other interval was entered. All three were open, had no central live-claim collision, and had no candidate or nonmatch C file across the existing local Rush checkouts before this claim. Their earlier appearances in helper ABI receipts were read-only inspection, not matching attempts.

## Source and storage model

Only mutable address-spelled tables/counters are referenced; there are no switches, indirect calls, FP literals or local rodata. All callees are already verified token guards: `void func_80014594(void)` and `void func_800145DC(void)`. Native entry/return use and peer-reviewed walker/resource receipts establish one unsigned-halfword argument and integer 0/1 status; original typedef names are not claimed.

The two release functions enter their guard before searching. The first matching ID wins. A 16-bit reference count decrements modulo 65536; only zero after decrement triggers stable left compaction, count decrement and a return of one. All paths release the guard. The third searches before taking the guard; a miss performs no guard calls. A hit preserves its index across the guard and rereads the live count before compacting. The count is stored before guard release. Stale final slots are not cleared.

Records are eight bytes. The first two have opaque payload bits at +0, ID +4 and reference count +6. The third has ID +0, opaque metadata +2 and opaque payload +4. Word-aligned union storage supports full record copies, while a pointer converted to its actual packed member supplies the bytewise field access contract. No allocation-only locals, extra formals, asm, artificial padding or dummy calls are used. The valid domain is nonnegative active counts within allocated table storage; no bounds or fallback behavior is invented.

## Bounded controls and diagnosis

Sixteen named source forms including the initial seed were tried, below the twenty-form bound. `controls.json` records compiler evidence and source hashes. Controls covered a compound search condition, genuine separate compaction index, packed versus aligned record representation, field/record copies and genuine copy cursors/temporary. No flag sweep, narrow-formal declaration sweep or compiler modification was used.

An aligned separate-struct cast view reached 6/77 for the first pair, but is rejected as an unnecessarily ambiguous aliasing representation. The retained union/member view scores 8/77 and passes host strict-aliasing optimized tests. Do not replace it merely to improve the residual.

`tools/workbench.py diagnose` was run before continuing each measured near-match and again on final objects. It reports a CFE-spelling/structural residual with matching frames. For the first pair, two branch operand orientations and the aligned two-word copy's temporary/schedule account for eight positional differences. The third additionally preserves the known halfword-formal home/mask temporary discrepancy; no unsupported retry of that narrow-formal plateau was attempted.

**Stop:** no evidence-backed source change remains in this session. Next useful input would be an original source or independently established aligned-record-copy idiom that explains the two-load/store schedule while preserving the actual packed-member access domain. Avoid aggregate-copy type guesses and blind allocation controls. For 1661C, an independent source-level explanation of the halfword home/mask convention is also needed.

## Reproduction and checks

Reuse existing pinned IDO via IDO_DIR or the explicit argument; do not download or copy a toolchain.

    python3 cloud/work/boot_tail/BT03-low-removal/verify.py --ido-dir /path/to/existing/ido
    python3 cloud/work/boot_tail/BT03-low-removal/test_semantics.py

A sparse staging checkout may add `--repo /path/to/read-only/full/checkout` to verification. Verification checks the target SHA manifest, all 439 extents/99,120 bytes, every one of the 24 installed compiler-file hashes, and the existing getter's strict match. It then recompiles all three final sources at O2 and O1 and uses compile-time IDO assertions for native eight-byte layouts and ID offsets. The host suite executes the exact final C for 4,500 deterministic cases, including empty/missing, first/middle/last/duplicate IDs, 16-bit decrement wrap, retained references, whole-storage preservation, guard counts, count-store ordering and a valid guard-time count extension for 1661C.

Host tests are semantic evidence, not native byte-match evidence. No ROM, raw assembly dump or object file is part of the packet. Target/scorer/shared-type/layout/lock files are unchanged. Independent peer review is required before central integration; publication belongs to the aggregate lead.
