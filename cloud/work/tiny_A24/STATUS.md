# A24: three fresh ordinary game matches

Frozen full TUs with literal flags `-g0 -O2 -mips2 -G 0 -non_shared`. Three claims total77 words/308 bytes. Each strict0, exact emitted extent, zero extra words, unverified/unresolved relocations or errors; captured canonical score.py fn exit0. No duplicate current shared lock at freeze.

| Claimed target | Retail/emitted words | Full TU SHA256 |
|---|---:|---|
| func_800FE73C | 19 | `a69f045b02a91f07322b33779e7b5e4aa80c5b8ff3b3383bc57cb618682a2771` |
| func_800B2CB4 | 27 | `c6ec386448fce22bf6397c5e2ef74bf751416da811b2d67289fb13e66adb8815` |
| func_800AF844 | 31 | `2b7d654bb140972c964155aec5c3cb60de6c0207b7b91d2152bd9908c442733c` |

Twenty candidates inventoried;13 ordinary full TUs built,3 matched. inventory.json and scores.json explicitly retain10 NONMATCH sources separately from the three claims. Five small audio-reverb callers were excluded from ordinary compilation because actual callee's unsaved s0/s1 require real context, not an ABI-only declaration. Two other targets deferred for ambiguous home/callee evidence. All sources/metadata are disjoint frozen A22/A23 and static/B ownership work.

## Exact claimed semantics

FE73C is a real all-inactive predicate: signed32-bit count at0x801460F4, true pointer at0x80144C48 to24-byte entries, signed byte offset0. Return0 at the first nonzero active byte; return1 when all are zero, including count<=0. No guessed capacity or new validation. The actual pointer/global count are correctly typed, and canonical bytes confirm the target's one initial count/pointer load and loop behavior.

B2CB4 scans the actual32-byte input records from pointer0x801525EC for the current unsigned16-bit count0x8015267C. Full-word input id is compared to record's unsigned halfword+0. Matching record's unsigned halfword+2 indexes the real24-byte output records through pointer0x801497F8, storing unsigned halfword15 at+0. Count/global-pointer reload behavior follows the actual target exactly after writes. Structures contain only actual offsets/strides and no invented allocation/read. Both input and output pointer words are32-bit under IDO.

AF844 consumes a real pending-record pointer whose field+0 points to destination. It reads float triples at pending+4/+8/+12 and+16/+20/+24, computes each midpoint using factor0.5, writes destination floats+16/+20/+24, sets full word destination+32 to1, and clears pending's pointer+0. Destination's byte padding simply describes actual fixed offsets; it does not force a stack frame. The emitted three-iteration loop, float operand/load order and final stores exactly match retail. Baseline source8/31 became4/31 after the ordinary commutative source spelling produced the actual emitted load order; readable separate statement lines close the four scheduling differences. Exact body lines must be preserved through integration and byte gates; no added runtime operation/guard is used. For NaNs or overlapping records, acceptance scope is exact emitted instructions rather than a presumed algebraic equivalence.

## Bounded controls and honest residuals

Thirteen typed faithful baseline builds; FE73C/B2CB4 match immediately. Eight directed source controls are controls.json: actual count/pointer iteration forF7564; real type3 constant for956BC; result flow forD3430; actual final flag address for958B8; true loaded word and conditional byte cache forstate_update_global; two midpoint spelling/line controls. Only final midpoint matches and replaces its baseline. Others remain original best source; no broad formal/coloring/reflow or ancestor sweeps.

Final unclaimed differences: F756419/19+2extra;8B42414/20;956BC19/20;D343011/23;958B821/25;sync_init_conditional10/25;BDD9015/27+1extra;state_update_global7/28;ADCE029/30+1extra;BE74429/30+1extra. All compile; none is eligible for acceptance. ADCE0 retains actual big-endian13-bit value/3-bit run extraction, comparison on each incremented value, opposite-end writes, and both unusual run subtraction and per-output count decrement from retail; it is not asserted as a conventional decoder. Unknown bounds/remaining semantic interpretations are not promoted as runtime claims.

Reproduce each claimed NAME: `python3 tools/cloud/score.py fn cloud/work/tiny_A24/NAME.c NAME --flags '-g0 -O2 -mips2 -G 0 -non_shared'`. scores.json contains sanitized counts/errors/source hashes/canonical exits only; no ROM/object instruction stream. Raw score/disassembly and private controls stay ignored build/codex-A24 or Rocky A scratch. No shared source/layout/lock/integration state or accepted source changed. Root must independently score and pass image/full-ROM gates.
