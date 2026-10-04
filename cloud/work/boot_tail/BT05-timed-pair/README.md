# BT05 timed loop and modulation research

**Two complete NONMATCHs, 924 B; zero matching bodies or verified bytes.**
The retained O2 sources differ in 35/119 and 57/112 words. Correct frames,
semantics tests and source refinements do not turn these into matches.

- Activation `976c3e0d`: `80021F84+476`, `80022A98+448`.
- Branch `dot/boot-tail-bt05-timed-pair`, explicitly stacked on frozen reviewed
  trio `0baed23f011b1b75bcdac34f4d7c7200fa102d76`; original master base
  `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Only this packet differs from the stack base. Previous sources, central
  ledgers, shared tools and protected inputs remain unchanged.

## Whole native operations and domains

Both handlers have genuine state and aligned two-word command inputs and return
zero. Partial packed state layouts preserve native accesses. Unknown arrays are
object storage gaps, not local padding.

`21F84` uses the packed halfword counter at +0x68. On first entry it obtains a
command limit or a genuine u16 RNG remainder, handles the FFFF sentinel, then
increments/decrements the finite count according to the native first-iteration
rule. Subsequent finite iterations decrement; zero exits without advancing the
program. FFFF bypasses the decrement. The source-family `goto` is retained for
this actual sentinel route; it is not an invented call or artificial keeper.

When the loop remains active, the first command gate clears the counter if state
flag 8 is set and flag 0x40000000 is absent. The second gate checks flag 0x20 and
real `1467C(word_index)` activity, again clearing the counter if appropriate.
Otherwise the current pointer becomes the saved program start plus the command's
low halfword, using eight-byte command stepping. `1E790(void)` genuinely returns
u16. Its promotion preserves native signed remainder. Random initialization
requires a nonzero limit; the native division trap case is not replaced with a
fabricated fallback. Pointer stepping assumes valid offsets within the actual
macro-program object on the N64 32-bit pointer ABI, not arbitrary malformed
bytecode or wider-host offsets.

`22A98` changes flag 0x8000 from the command's high two control bits, converts a
real address-taken u32 duration local through `1E930` or `1E940(pointer,state)`,
and changes flag 0x4000 according to the resulting duration. For a nonzero duration
it stores the period at +0x70 and interprets the two input range bytes as signed8.
The negative-key, zero-key/negative-cent, positive-key/negative-cent and ordinary
nonnegative branches retain the exact native range and half-period rules. The
fields at +0x78/+0x79 are unsigned bytes, accommodating the absolute value 128
and native `100 - negative_cent` values through 228. Unary negation is promoted
to int, and the decrement only applies to a positive signed byte; neither can
overflow. Period halving remains unsigned for all 32-bit durations. Zero duration
leaves period/range fields unchanged while clearing the enabled flag.

The timer helpers consume the genuine local pointer, and the activity helper
consumes a full word slot index. All callees are declared only and resolve in the
canonical symbols. No local table, fake float parameter, hidden argument or
callee implementation is introduced.

## Bounded source controls

Seeds were scored at O2, then O1. Before refinement, the unchanged workbench
compared fresh relocated candidates with immutable targets in temporary storage.
Both native frames, 24 and 32 bytes, are correct.

- Loop seed: 54/119. Splitting the actual high-bit/flag condition gives 45/119.
  A guarded block with a common zero return improves to 35/119. Full-word offset
  extraction gives 36/119, so the better complete source is retained. Four forms.
- Timing seed: 57/112. Explicit signed-byte extraction, as in the public source
  family, leaves 57/112 unchanged. Two forms, then frozen.

Remaining load-width, scalar allocation and lowering differences are documented
rather than forced. No prototype, SDK-word, register, K&R, padding, volatile,
assembly or alternative flag sweep was attempted. Initial/directed receipts
preserve all numeric controls without raw native listings or objects.

The CC0 [MusyX source family](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/synthmacros.c)
contains `mcmdLoop` and `mcmdVibrato`. Revision
`78d2e16e4905fc675952162d331c24d5198b2687`, source blob
`a3203f9e0032fa5e9d08fa5e51da80acf4cab0c0`, license blob
`0e259d42c996742e9e3cba14c677129b2c1b6311`. This is not authenticated N64 source.
Native 32-bit flag values, packed fields, return behavior and helper contracts
remain authoritative; newer 64-bit flags and layouts are not substituted.

| Function | Final O2 | Final O1 |
|---|---:|---:|
| 21F84 |35/119|118/119 +29 extras|
| 22A98 |57/112|111/112 +28 extras|

Final O2 sources have no nonzero extras, unresolved symbols, unverified
relocations or errors. Both are still complete NONMATCHs under `nonmatch/`.

## Tests and replay

Two strict-C89 ASan/UBSan groups compile the actual retained sources. The loop
checks 8,736 valid combinations of first/subsequent counters, FFFF sentinel,
random limits, gates, activity and program offsets; zero random divisors are
explicitly excluded. The timing group checks all signed-byte pairs across both
conversion paths and three duration values, totaling 393,216 calls, including
live helper flag mutations and unchanged fields on the zero-duration path.
Synthetic helpers express contracts, not original implementations. Host pointer
semantics do not establish N64 pointer offsets. LeakSanitizer alone is disabled
under ptrace; no heap is allocated.

```sh
python3 cloud/work/boot_tail/BT05-timed-pair/verify.py
python3 -m unittest discover -s cloud/work/boot_tail/BT05-timed-pair -p 'test_*.py' -v
```

Use normal pinned IDO setup or `IDO_DIR` for a verified existing copy. Four final
rows/source hashes are bound in `verification.json`. Preflight passes protected
manifests, all 439 extents/99,120 B and the existing getter. All 161 static locks
and whitespace checks pass. No matching submission, target, symbol, lock, layout,
scorer, compiler, runtime image, farm, spec or production gate changes. Accepted
800D1248 and restricted helper work remain untouched. No ROM bytes, raw assembly,
objects or credentials are published. Independent peer review precedes central
integration; central owns exact aggregate CI and the checker alone merges.

Independent paired review PASS binds source commit
`56c231fd4d21ffb2774342899d4dee5b599836a9`, tree
`f0760bdd154216d6ad069ca7d0ad156b0c783fdc`. Fresh immutable source copies reproduce
all four rows and both sanitizer groups. The reviewer inspected both complete
native/source bodies and all four direct callees. FFFF handling, live reloads,
valid program stepping, signed-byte branches and unsigned half-period agree.
The tick converter retains its external nonzero-tempo contract; malformed
runtime configuration is not asserted safe. `independent_review.json` records
924 bytes of complete nonmatching research, with zero matching credit.
