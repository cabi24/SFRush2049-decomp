# Clipped rectangle: alias-boundary scheduling cause

`func_80087110` remains unaccepted by this diagnostic packet. The frozen C
baseline is four differing words out of 445, with no extra executable words,
unresolved relocations, or unverified data. A listing-only diagnostic isolates
a compiler-metadata cause and is **not eligible for promotion**.

## New finding

The source of the residual is not a missing real read of `v1` on the taken
path. The native taken path does not read it before replacement. In the stock
baseline, the compiler's own liveness diagnostic likewise marks it dead there.

At the end of the stretched **both-flags** arm, uopt closes the display-list
address register's stack no-alias relation *after* an unconditional exit.
The resulting metadata-only block has no real instructions, but as1 retains it
as an extra predecessor of the next, x-flip-only test.

The assembler's cross-basic-block (XBB) weighting then assigns the x-only test
weight **65534** and its body weight **1**. The edge addition is not hoisted.
Final branch filling takes the display-list address operation instead.

Moving that same alias close across the following label, or removing it only
for diagnostic isolation, eliminates the extra predecessor. The x-only test
then has weight **13**, its body **16**, and XBB explicitly hoists the existing
edge addition into the test block. Stock as1 output then has **zero differing
words out of 445**, zero extras, and no unresolved/unverified references.
This does not constitute a C match: the input to as1 was deliberately changed.

Prior `w4a/f87110/as/e1.s` and `e2.s` tested the **following** arm's alias
boundary. Replaying that following-boundary change leaves the original residual
and causes 53 differing words overall. The successful boundary is the one
immediately before the historical `$46` label, not `$47`.

## Evidence and controls

`verify.py` compiles the unchanged historical C through stock IDO, retains its
stages under ignored `build/`, and runs stock as0/as1 on five bounded listing
controls. `verification.json` records source/tool identities and numeric full
comparisons. It also rebuilds the three exact ordinary-C controls: `both_body_do.c`,
`height_before_do.c`, and `setup_do.c`, expecting 8, 174, and 29 differing
words respectively, and checks each source hash against its allocator receipt. Neither script nor receipt changes targets, flags, scoring,
compiler code, or promotion state.

- Historical C and identity listing replay: 4/445.
- Remove alias close before x-only test: 0/445, diagnostic only.
- Move alias close across x-only test label: 0/445, diagnostic only.
- Corresponding removal/movement at the y-only test: 53/445.

The print-only assembler diagnostic was enabled using the existing
`cloud/work/frontier/w4a/tools/patch_as1_printf.py` wrapper support. The generated
as1 compiler source was unchanged. Both the ordinary `-R` trace and deeper
`-r -peepdbg 5 -xbbdbg 5` trace produced whole objects byte-identical to stock
as1 on the same inputs. Raw listings and logs remain in ignored build storage.
Only the stock toolchain's output is used for numerical comparison.

The deeper trace independently confirms the earlier local scheduling result:
for the pointer-high node and edge-add node, start time, initial best time,
and latency tie; critical-path lengths 34 versus 30 select the pointer. The
important new distinction is that a successful edge delay slot is made by XBB
hoisting before final branch filling, not by changing that local tie-break.

## Compiler source path

The v1.2 source of the already-used `decompals/ido-static-recomp` toolchain was
inspected at commit `9c242adc890beef098020149d9554f48208f699d`.
No code-generation modification is part of this packet.

- uopt `base_in_reg` creates no-alias metadata from actual base provenance.
- uopt boundary cleanup `func_4247a4` emits alias closure when the previous base
  differs from the next graph node's register data.
- ugen translates the resulting `Ualia` metadata to alias/no-alias directives.
- as1's `func_429534` computes 16-bit block weights. Its predecessor weight
  adjustment can subtract three from one, producing the observed 65534.
- as1 performs XBB movement before final branch optimization.

## Natural-source follow-up

The source lead independently found that a `do { ... } while (0)` body wrapper
around the complete stretched both-flags arm moves alias closure **before**
the unconditional exit. Retained stock stages confirm it removes the metadata
predecessor. It fixes the original scheduling residual, but initially exchanges
height and offset registers at eight sites.

A diagnostic global-color trace (existing workbench profile, no force controls,
optimized Ucode byte-identical to stock) explains that exchange precisely:
height's net estimated saving stays four, while its normalized divisor
(`nocs`) rises from three to four. Its priority falls from 4/3 to 4/4; offset
remains 4/3 and takes the first available register.

Important: `nocs` is **not** the raw live-block count. The compiler computes
X = occurrence count + live-IN block cardinality, then uses X when X < 3, or
2 + ((X - 2) >> 2) otherwise. The trace values establish the divisor and
priority changes, not a literal one-block lifetime increase. The net-saving
numerator is likewise not a source-reference count.

Moving the genuine vertical-edge update before the wrapper was tested and
rejected: although height returns to divisor three, its interference count
falls from 20 to 16 and it moves from first-phase to second-phase coloring.
Offset still wins the first register. A packet pointer also fits into a
register that previously required a stack reload, changing the instruction
population and downstream temporary-register rotation (174 differences).

Wrapping the genuine height/stretch initialization as well restores the
height/offset priority tie at 4/4 and the desired register pair, with the
scheduling fix intact. It exposes a separate 29-site exchange: horizontal
texture input `s` changes from net-saving/divisor 16/10 to 16/11, while step stays 6/4 and now
wins the former `s` register. This identifies a narrowly bounded next source
question: preserve `s`'s original live extent or lengthen the genuinely
initialized step value, without dummy operations or additional pressure.

Final C validation and independent acceptance belong to the source lead;
listing diagnostics are never substituted for them.

## Source identities

- `baseline`: `f8355db6884b85c77a25b21c7aae746be236ed10a3dc8fee93264259cf4c6f8f`; source comparison 4/445, unaccepted.
- `both_body_do`: `98a0adc8027464275b43845a6e223269c96a7cef2e1ec00395d264e0754b9c35`; source comparison 8/445, unaccepted.
- `height_before_do`: `99a3c8b36bb9284084899cd43fcc900c64e0a91a4ab43408ecde331851ac891f`; source comparison 174/445, unaccepted.
- `setup_do`: `3fb16b926eb91443d08b7836685658c76fb683df3143d021f78984aacebd2ea1`; source comparison 29/445, unaccepted.

`allocation_receipt.json` preserves the four relevant allocator decisions for each source, without machine-code arrays or raw listings.

The three complete source controls above are included byte-for-byte so both the compiler comparisons and allocator source bindings can be reproduced.

## Exact lifetime membership follow-up

The compiler's native `-zdbug:5` listing resolves the divisor threshold:

- Baseline height: four occurrence-block records plus five default-live blocks
  gives X=9 and divisor 3.
- Drawing-body wrapper height: four plus six gives X=10 and divisor 4.
- Baseline offset: four plus four gives X=8 and divisor 3.
- Drawing-body wrapper offset: four plus five gives X=9 and divisor 3.

Both values gain one default-live block. Only height crosses the normalization
threshold. The added block is the wrapper entry, graph node 35 in that source;
it has no occurrences or local definitions, but both values are live through
it. This is why moving a used value outside the wrapper can affect register
pressure and the divisor independently.

The exact occurrence and default-live block IDs are saved in
`allocation_receipt.json`. They are per-source graph IDs, not instruction or
ROM addresses. The stock runtime's float-formatting wrapper aborts this native
listing; implementing its diagnostic-only ecvt/fcvt wrappers made the listing
complete. The generated uopt compiler source was unchanged, and each completed
optimized-Ucode output was byte-identical to the ordinary stock run. These
listings are explanatory diagnostics, not alternative match evidence.
