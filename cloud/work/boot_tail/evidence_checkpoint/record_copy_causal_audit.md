# Packed-record-copy causal audit

Outcome: BLOCKED-COMPILER-LOWERING. No new source candidate, build, flag trial, target claim, match credit, repository edit, or publication. Audit completed 2026-10-04 UTC against central 4d0f5b4a899496e6e1a45b4b1a4497fdbf975b91. Only existing candidate/native objects and preassembler captures were read. Native bytes/listings, objects, and external compiler source remain excluded from this report.

## Direct stock evidence

Existing source hashes remain:
- func_80015720: 12c277008bcd9dd866ed98eb7e5a1b7f97a5ffd6a22699589ab40fc9622b498e
- func_800158D8: e6044e6e6fe590c91723d453b9fbfaefd76a10e15bd514bd38def333e70b9882

Re-relocation of the unchanged scout objects through the unchanged strict scorer reproduces:
- 15720: 6/110 different words, 440-byte native body; offsets E0, E8, EC, F0, F4, FC.
- 158D8: 8/77 different words, 308-byte native body; offsets 38, 64, E8, F0, F4, F8, FC, 104.
- Both: zero relocation masks, unresolved symbols, unverified references, errors, or nonzero excess words. Existing captures give exact 440/308-byte ELF function extents. No fresh build was needed or claimed.
- 15F28 is the archived 308-byte twin; this audit did not recompile/re-relocate it.

Both existing stock preassembler listings already assign line 32 to the loop header and line 33 to the aggregate assignment. Both explicitly assign the first copied word to the assembler temporary under noat, before scheduling. The native copy instead uses ordinary temporaries: first/second carriers t3/t9 for15720 and t9/t8 for158D8. Candidate carrier pairs are at/t3 and at/t9 respectively.

The first copy divergence is therefore at15720+E0 or158D8+E8, not a mere store rotation. The candidate must keep at live until the first store, whereas the native loop can use at for the loop comparison earlier. That creates a real scheduling dependency explaining why its source-pointer update and comparison occupy different positions. Pure line regrouping or a FIFO phase adjustment that leaves the at fallback selected cannot repair the carrier class.

The two earlier158D8 differences, +38 and +64, are branch operand orientations and are independent of this copy problem. Do not advertise the rejected6/77 separate-struct alias view as the retained baseline.

## Source / object constraints

The verified caller14FEC passes the reloaded u16 identifier and successful lookup entry+8 to15720; there are exactly two genuine formals. Tables are direct globals, not caller-supplied record destinations. The canonical table base8003C618 and8-byte stride preserve word alignment. The first word is payload bits, with ID at+4 and reference count at+6. Native field operations retain packed/bytewise accesses while whole-record shifts use aligned words. The retained packed-member/aligned-union model satisfies these constraints; it is not proof of the original typedef spelling.

Changing alignment alone has no demonstrated missing load/store fact to fix: retained preassembler already uses aligned word transfers. The14 insertion controls and16 removal controls already cover union/aligned/packed aggregate copies, scalar/member copies, cursors/temporaries, and other obvious representations. No authentic volatile contract, alternate object geometry, original source macro, or new alignment fact was found. Fabricating a larger/aligned union member or aliasing view would not be justified.

## Concrete compiler-source lead, with version limit

Primary upstream source inspected at decompals/ido-matching-decomp commit4e9bd753d197e9f593475aab1e3c46701015bdfa:
- src/ugen/eval.p, eval_mov, lines1961–1979 and2033–2046.
- src/ugen/reg_mgr.p, free_reg_is_available, lines590–596.

The published eval_mov implementation first allocates the second-word carrier. It requests a second free register for the first-word carrier when the free list or large-offset conditions allow; otherwise it selects at explicitly. Alignment separately selects word versus unaligned transfers. This gives a concrete place to investigate the observed fallback, rather than suggesting another record-type sweep.

However, upstream labels ugen7.1 functions matched and5.3 unmatched; this source has no5.3 conditional implementation. It is NOT proof that the pinned5.3 takes the same branch or that register pressure is the actual cause here. The local5.3 ugen is stripped and no existing free-list/Umov trace was available. No replacement compiler, compiler patch, tracing build, flag variation, or new toolchain was introduced.

The vendored workbench at3f58a68db5d4cf343c76b8361dfc1786c501e521 documents FIFO allocation and per-procedure reservations (L12/L64), but its cfe-spelling owner label is expressly heuristic. It has no measured aggregate-copy fallback lever for these exact inputs. Do not infer allocator membership from register names alone.

## Reopen gate

Before another source trial, obtain a read-only trace or verified source-level correspondence for the approved stock5.3 aggregate-copy lowering on one unchanged retained source. It must identify:
1. The copy opcode and source/destination alignment/length metadata entering lowering.
2. The source/destination expression tree references and reserved/live/free registers at copy entry.
3. Actual first and second carrier allocation/free events, with source-line provenance, and the exact condition selecting at.
4. A native-compatible, genuine C construction (or original source/type evidence) that changes that condition without changing the established ABI,8-byte geometry, packed-field access contract, loop direction, or semantic domain.

A trace-only method must first reproduce the pinned stock object's relevant text/relocations byte-identically when observation is disabled; altered-codegen or replacement-version evidence is a lead only. Keep the experiment bounded to the earliest copy divergence. If the missing evidence cannot be obtained, retain COMPLETE-NONMATCH and stop. No source hypothesis currently meets this gate.

Primary links:
https://github.com/decompals/ido-matching-decomp/blob/4e9bd753d197e9f593475aab1e3c46701015bdfa/src/ugen/eval.p#L1961-L1979
https://github.com/decompals/ido-matching-decomp/blob/4e9bd753d197e9f593475aab1e3c46701015bdfa/src/ugen/reg_mgr.p#L590-L596
https://github.com/decompals/ido-matching-decomp/blob/4e9bd753d197e9f593475aab1e3c46701015bdfa/README.md
