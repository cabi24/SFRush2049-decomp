# BT03 mapped pause-volume parameter dispatch

**COMPLETE-NONMATCH research: one full 904-byte body reconstructed, zero new
matching functions and zero verified-body bytes.** The compiler's six-entry
jump table now reproduces the authorized mapping exactly. The body still has
76 of 226 fully relocated words different, and the unchanged stock scorer
continues to mark its local-rodata relocations unverified.

- Base: `cf4b9c619c72e83aa5da2e8b5c110765f9b03693`
- Exclusive interval: `func_8001BE14`, `[0x8001BE14, 0x8001C19C)`
- Claim acknowledgment: central `690e879b18a9e4dd2919e6138942ff19a94a6131`
- Retained source: `func_8001BE14_NONMATCH.c`
- Edited scope: this research directory only. No matching submission, shared
  ledger, protected target, scorer, symbol map, or production layout is changed.

## Native contract

The three real O32 inputs are unsigned byte volume, unsigned halfword duration,
and unsigned byte group selector. The sole canonical caller, `func_800203EC`
at offset 48, forwards those widths after its activation gate and lock helper.
The full target homes these inputs, changes zero duration to one, copies the
normalized duration into a word local, and calls `func_8001E930(&local)` once.
That complete native callee multiplies the unsigned word by 256. The original
normalized duration, rather than the converted duration, divides the ramp delta.

`D_8004F300` is a 32-entry array of 40-byte master-fader records. The existing
matched initializer `func_8001C1D8` independently supports the stride, group-type
byte at 20, and pause volume at 24. This body only writes pause target at 28,
pause delta at 32, and converted pause time at 36. The 20-byte normal-fader prefix
and three other header bytes are preserved, not fabricated stack padding.
Native fixed-point target is volume shifted left 16. Subtraction wraps as a
32-bit word; that difference is interpreted as signed and divided with truncation
toward zero. Unsigned field storage plus an explicit signed difference cast
retains those native bit semantics without signed-subtraction overflow in C.

Selector behavior from the authenticated table and whole body:

- 255: update all entries whose type is 0 or 1
- 252: update all entries whose type is 2 or 3
- 250 / 251 / 253 / 254: update type 2 / 3 / 0 / 1, respectively
- Other selectors: update exactly the indexed entry, without filtering its type

Defined C equivalence requires group 0..31 or 250..255 and live aligned array
storage. The native code has no bounds check for 32..249; it indexes beyond the
32 records into later memory. Those selectors are replayed against explicitly
mapped synthetic backing only. They are not asserted valid source-domain inputs
or safe production storage. No guard or new behavior was invented.

## Public source-family evidence

The pinned [AxioDL/MusyX synthPauseVolume source](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/synth.c)
provides the same three-input family, zero-duration normalization, selector
ordering, type tests, and shared case-label organization. Its later floating-point
faders, additional active flags, and newer storage representation are not imported.
The N64 full body remains authoritative. The source blob was freshly fetched and
SHA256 checked; its revision, Git blob, digest, and
[CC0-1.0 license](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE)
are pinned in `input_pins.json`. No public source is claimed to be the original
N64 translation unit. The available repository supplies no arcade equivalent;
unavailable arcade source was not consulted or cited.

## Complete, unmasked relocation proof

`dispatch_proof.json` contains only this function's source-free extract from
the independently approved receipt. It records the 24-byte table at `8002D8E8`,
its hash, and the six absolute destinations. This worker opened no asset data.
The original receipt's total 68-byte read covered three separate claimed tables;
this packet does not broaden that authorization or assign production storage.

For the final O2 candidate, `verify.py` resolves all 17 text REL entries and all
six `.rodata` R_MIPS_32 entries independently. A separate temporary GNU ld link
is compared byte-for-byte with that calculation. It uses native text/table
addresses and four-byte input subalignment so IDO's section alignment does not
silently move the evaluated function. The source object is unchanged by linking.
This test placement is not a production linker or ownership edit.

All 24 compiler-generated table bytes equal the authenticated mapping, whose
SHA256 is `a1b9a25984b45dee83746aee148a632d9c8dcd9a86d20dc74e131e760839356f`.
The function symbol is exactly 904 bytes; its padded text section is 912 bytes
with eight zero alignment bytes. The eight trailing rodata alignment bytes are
also zero, but are not claimed to be authenticated native table data. There are
no unresolved linked relocations, no copied target instructions, no masked bits,
and no nonzero text excess in the O2 proof. Nevertheless, the complete text
comparison has 76 differing words, so it is not a match.

The stock scorer independently reports 76/226 differences, zero nonzero excess,
and two unverified local-rodata relocation sites. That result is preserved.
This packet's separate table proof does not grant strict scorer admission.
Even a future exact text candidate needs the maintainer's supported intake
route for local rodata. Production table ownership remains unassigned.

## Bounded compiler investigation

Sixteen natural source forms, including the retained source and 15 archived
controls, were evaluated. Both IDO 5.3 O2 and O1 are replayed for each form
(32 source-bound compilations in `verification.json`). No fake keeper, extra
formal, padding local, volatile barrier, dummy helper body, hand assembly,
manual unrolling, or flag sweep was introduced.

The public-family-shaped simultaneous index/pointer loop compiled with four-way
unrolling and repeated local-time loads; a pointer-end loop lost the counted
native shape. Direct indexed array accesses with a genuine signed loop/index
carrier recovered the native two-record unroll and exact function size. Using
that same index for the single-entry default retained the smaller entry shape.
Ordering the independent time store before the delta assignment improved the
whole-body residual from 97 to 76 words. Signed/unsigned word typedefs, pointer
versus indexed type access, explicit duration mask, index signedness, default
pointer/group spelling, field representation, and type-carrier width are
preserved as measured controls rather than recommendations to repeat them.

The unchanged workbench was run at the first near-shaped candidate before
further refinement. A final equal-address, fully relocated ELF diagnosis is
summarized without native instruction dumps in `diagnosis.json`: both frames
are 48 bytes and both bodies have 226 instructions. Entry narrowing/register
allocation differs, followed by a temporary-register-ring shift. Workbench's
source-reachable classification is a heuristic, not original-source proof.

O1 for the retained source is 1,004 bytes with 220/226 differences and 22 nonzero
excess words. The stock comparison cuts off two HI16/LO16 pairs at its target
boundary and reports them unpaired; the independent whole-object link resolves
all of them correctly and still fails both body and table equality. This is
reported rather than hidden. O2 remains the stronger native-family hypothesis.

The bounded source-shape attempt stops here. Reopening needs authentic N64
source/compiler context or a specific newly evidenced narrowing/allocation
mechanism, not another pass over these archived forms.

## Reproduction and tests

With pinned IDO 5.3 in `IDO_DIR`, MIPS GNU binutils on PATH, and GCC available,
run from repository root:

```
python3 cloud/work/boot_tail/BT03-mapped-parameter-dispatch/verify.py
python3 cloud/work/boot_tail/BT03-mapped-parameter-dispatch/test_semantics.py
python3 cloud/work/boot_tail/BT03-mapped-parameter-dispatch/test_host.py
python3 cloud/work/boot_tail/BT03-mapped-parameter-dispatch/test_mutations.py
```

- Protected manifest, compiler hashes, source/context pins, full target hash,
  native caller discovery, and the existing getter strict match all pass.
- Eleven IDO layout assertions prove word/pointer widths, the 40-byte record,
  and its accessed offsets without changing candidate code or relocations.
- 5,632 full native cases and 5,632 linked-candidate cases agree with an independent
  memory oracle. These cover all byte selectors, dirty upper argument bits,
  zero/max duration, positive/negative/wrapping differences, complete byte state,
  exact writes, selected table access, the complete real scaling callee, restored
  stack and callee-saved registers. The valid-domain subset also passes 1,272
  actual-source host differential calls.
- 76,024 actual-source ASan/UBSan host calls cover every u16 duration, every valid
  selector and every possible type byte. Leak detection is disabled because the
  execution environment uses ptrace; the source and harness allocate no dynamic
  memory. Address and undefined-behavior sanitizers pass with empty stderr.
- Four temporary negative controls are rejected: wrong selector type, wrong
  divisor, omitted real conversion call, and swapped compiler dispatch cases.
- The bounded integer interpreter executes 198/226 native PCs. The 28 omitted
  PCs are arithmetic-trap paths and their checks that require a zero/negative
  divisor; normalization guarantees a positive 1..65535 divisor. Unsupported
  instructions, unaligned/unmapped accesses, uninitialized stack reads, and
  runaway control flow fail closed.

The receipts bind the actual source hash. These are synthetic research fixtures,
not audio-output, original-runtime, hardware-timing, cartridge, full-engine or
production-integration proof. No ROM bytes, raw assembly dumps, objects,
credentials or unrelated private information are publication artifacts.
D1248/helper work and T050 implementation remain untouched. Independent review approved frozen source commit
`2615d377e8b4a538473fd9b22ac041a41e8378a5` and tree
`abfb94c30b1743d780f2721a238596b6a65b84a8` with no blocking findings.
`independent_review.json` binds the reviewed source and all four exactly replayed
receipts. The follow-up changes only review/status documentation; no merge is done.
