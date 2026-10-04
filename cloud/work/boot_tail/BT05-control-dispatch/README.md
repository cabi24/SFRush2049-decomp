# BT05 controller-entry selection

**Complete NONMATCH, 196 B; zero matching bodies or verified bytes.** The final
natural source differs in22/49 O2 words. This is a bounded research result,
not a body eligible for submission or promotion.

- Fresh master base `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Branch `dot/boot-tail-bt05-control-dispatch`, central activation `d5ee62f2`.
- Exact singleton `[0x80023754,0x80023818)`, 196 B; all prior sources stay frozen.
- Only this packet changes. Central status/claims remain separately owned.

## Genuine operation and ABI

The four inputs are the state pointer, embedded control-record pointer, aligned
two-word command pointer and 32-bit state-flag mask. The nine previously matched
wrappers at `23818` through `23978` supply these roles directly and ignore this
void helper's incidental register contents. No formal is invented.

The function reads the command's combination byte and packed state flags +0x24.
When the mask is absent or the combination is zero, it sets the flag and resets
the control's count. While count<4, it saves the prior count, increments it, calls
`21548` with command bits8..15, and writes a four-byte entry containing the result,
combination and signed scale. The scale is the signed command high halfword,
multiplied by256 and divided by100. The four entries occupy bytes0..15, and the
count is byte16. This is a minimum record prefix, not a guessed array stride.
The saved index and combination both survive a real call and are meaningful
local variables. Object gaps are not stack-padding locals.

`21548` is declared only. Its native entry narrows its genuine input to a byte,
and its observed return paths produce byte-domain results. This caller does not
assert a case mapping or copy any table or callee body. The separate21548
SOURCE-LEAD/table-proof blocker remains unchanged. Strict relocation of23754's
external call is resolved; this body itself requires no local rodata proof.

The signed halfword ranges from-32768 to32767, so multiplication by256 is defined
in signed32 arithmetic. Division follows the pinned N64/IDO implementation's
truncation toward zero. This avoids the undefined negative signed left shift
that a literal instruction-shaped source would introduce. All packed offsets
and command alignment follow native accesses; wider-host layouts are not claimed.

## Bounded diagnosis and stop

The seed was scored at O2, followed by O1. The unchanged workbench diagnosed
fresh relocated candidate and target objects in temporary storage. The O2 frame
is correctly32 bytes, with zero nonzero extras or unresolved/unverified/error
fields. Residuals include native word-versus-source byte extraction, byte-spill
slots, and the known narrowed-argument temporary allocation pattern.

Only two supported source controls followed diagnosis:

1. Explicit word-width combination extraction preserves the native load width,
   but introduces conversion temporaries and worsens the score to46/49.
2. A meaningful signed scale local with separate bounded multiply/divide keeps
   that46/49 result. It does not manufacture a stack slot or improve allocation.

The initial natural source is retained at22/49. No register, K&R, alternate ABI,
SDK-word declaration, fake local, volatile barrier, assembly or flag sweep was
tried. The known plateau is frozen after three natural forms. Additional work
would need authentic compiler/source context or a new measured mechanism.

Final O1 remains48/49 words with23 nonzero extras. Both final controls have zero
unresolved symbols, unverified relocations and errors. All matching credit is
zero; the source stays only in `nonmatch/`.

## Reproduction and preservation

Run `python3 cloud/work/boot_tail/BT05-control-dispatch/verify.py` from this
checkout. `verification.json` binds both final controls to the retained source
hash; initial and directed numerical receipts record the complete bounded run.
`preflight.json` records protected target hashes, all439 starts/sizes (99,120 B),
the preexisting getter and unchanged source/compiler hashes. Static locks and
whitespace checks pass. Independent peer review precedes central integration;
the central integrator owns exact aggregate-head CI.

No matching submission, target, symbol, lock, layout, scorer, compiler, runtime
image, farm, spec or production gate is changed. Accepted800D1248 and restricted
helper work remain untouched. No ROM, raw disassembly, object or credentials are
published.

The paired reviewer reproduced both final rows and the exact source hash at
`85b8aa283f50c044eff68c5d31b07f61756e9f23`, tree
`068129f9c7528a6a3597c91f55dde964cddb660d`. All49 native words and the four-input
count/entry ABI passed actual-source review. Independent C89 ASan/UBSan checks
also passed all65,536 signed scale values and1,024 count/combination/flag cases
with a synthetic translator contract. `independent_review.json` records PASS
for complete nonmatching research only. The21548 table blocker is unchanged.
