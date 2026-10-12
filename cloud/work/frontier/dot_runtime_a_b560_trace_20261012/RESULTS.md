# Image-A B560: 936 new exact bytes, ownership migration blocked

## Deliverable and boundary

`group.c` and `group.json` compile with stock IDO 5.3 at
`-g0 -O3 -mips2 -G 0 -non_shared`. The canonical group path includes the
repository's normal `-r4300_mul` assembly setting. No acceptance build uses a
patched compiler, object edit, forced register, false return prototype, added
formal, artificial call, unused frame local, or assembly source.

New: `func_8039B560`, complete `[0x8039B560,0x8039B908)`, 936 bytes / 234 words.
Preserved: A448 140 B, B120 244 B, B214 836 B, BE48 760 B, totaling 1,980 B.
These four were already locked and earn no new credit. Every exact body is in
both `members` and `claims`. The genuine sole kept root C140 remains complete
nonmatching context (475/496 positional words); it is never claimed here.

This is a separate research draft. BE48's existing scene-init owner and the
three other members' existing owners remain untouched. The group's source is
an additive research copy; do not install duplicate definitions or reassign
ownership automatically. The coordinator must explicitly reconcile overlapping
owners and preserve every existing locked claim before installation. No
protected files, production source, lock, target, asset or prior packet changed.
No new image-spliced or ROM-verified bytes are claimed.

Base: master `1a2e0f005`. The exact target was claimed before coding. Current
CLAIMS and overlay locks plus a live indexed open-PR search showed no B560
worker. The predecessor source was the archived object-initializer packet.

## What closed the residual

Fresh baseline: `14/234 words differ`, correct 72-byte frame and integer
allocation. Workbench diagnosis found 234 instructions, 12 FP-register rows,
and the expected raw-object relocation caveat. The strict scorer supplies the
actual residual; relocation-blind diagnosis is never acceptance evidence.

A diagnostic UOPT build reproduced the stock `.opt` and symbol-table bytes
exactly before applying any oracle. Its color trace showed:

- Named Y snapshot: total saving 6 / 3 counted blocks = 2; selected f20
- Named yaw snapshot: 6 / 6 = 1; selected f22
- Literal 100.0f: 3 / 4 = 0.75; selected f24
- Literal 0.5f: 1 / 3; selected f26

Forcing only the observed FP colors reduced 14 differences to four load-schedule
words. This isolated allocation but was deliberately not treated as a match.

The inherited draft had incorrectly modeled native float literals as external
reads. The current tracked source asset authenticates all eight B560 floats.
Replacing the actual anchors with their float values changes saving priorities:
100.0f becomes highest (0.75), yaw next (4/6), and the two Y literals next (1/2).
This naturally gives native f20/f22/f24/f26. The two equal Y values occupy
separate native literal slots; `55.2f` and `55.20f` preserve those separate
compiler constant webs. The two corresponding pairs of position stores use
direct literals. Retaining a named Y copy generated an extra absolute-address
store, while the direct stores recover every scheduling word. The unused Y
local was then removed with no code or frame change.

The minimal authentic BE48 caller proves B560 first. Adding that unchanged
initializer to a copy of the current scene-init context preserves all four
older exact bodies. The already-correct returning `sign_extend_call` remains
explicitly prototyped; it stays an external main-image function and is never
inlined across images. No original-source names or original TU boundary are
claimed.

## Full proof and replay

`python3 cloud/work/frontier/dot_runtime_a_b560_trace_20261012/verify.py --deep`

The verifier authenticates current-tree `assets/us/data.bin`, its loader
pointer and the image-A SHA-256, without historical checkouts or external
inputs. Image bytes exist only transiently in memory. The standard own-data
checker receives that authenticated image; neither its acceptance checks nor
the compiler are modified. Output:

```text
func_8039A448: MATCH
func_8039B120: MATCH
func_8039B214: MATCH
func_8039B560: MATCH
  own .rodata verified at 0x803B9594..0x803B95B4
func_8039BE48: MATCH
  own .rodata verified at 0x803B95B4..0x803B95B8
PASS: 936 new exact bytes; four old exact bodies preserved; ownership migration blocked
```

A bare `tools/cloud/score.py group ... --targets asm/us/ovl_a` reports the same
code matches, but correctly leaves 18 local-section relocations unverified
because no standalone overlay-A data artifact is committed. It therefore exits
nonzero. `verify.py` supplies only the authenticated current-tree image data to
the ordinary strict comparison, proving all 18 sites and all 36 meaningful
owned-data bytes. No `--allow-unverified` acceptance is used.

The deep check independently parses ELF, re-sections each complete original
STT_FUNC extent, and asks GNU MIPS assembler/linker to resolve all external,
internal-call and owned-data relocations at original native VMAs. It compares
every linked word against the protected target, checks no relocation remains,
and verifies all nine owned floats, including BE48's prior 1.01f. The 12 bytes
after them are zero compiler alignment, not additional owned data.

The bounded negative control merges only the two 55.2 spellings. It must fail
strict comparison/owned-data placement. The unchanged compiler-produced
positive source is the only accepted source. All checks remain active under
Python `-O`.

`verification.json` pins source/recipe/verifier, parsed complete target words,
relevant metadata manifest rows, and authenticating input hashes. Mutable
scorer/conveyor tools, production locks and assembly text are not pinned.
No static ROM TU is compiled; the only acceptance compiler is identified as
IDO 5.3 by the repository compiler helper.

One routine regression:
`REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_a_b560_trace.py -o addopts='' -q`

It recompiles the group and one negative control. GNU linking is optional
`--deep` research, not a second collected routine test. Missing IDO uses the
shared conftest skip policy; REQUIRE_TOOLCHAIN makes a missing compiler fail.

## ABI, storage and limits

B560 takes no arguments and returns no value. It uses a 72-byte stack frame,
saves/restores ra, and deliberately clobbers s0..s7/fp and f20/f22/f24/f26 in
the image's private whole-program calling convention. It is not standalone O32.
The genuine C140 root and BE48 caller provide that context; all static B560
calls remain internal relocations to its exact native entry. The packet does
not certify the nonmatching root's complete semantics.

The slot layout is 64 bytes: two words at +0/+4, angle +8, a 3x3 matrix +12,
position +48, and the alpha byte +60. The initializer walks twelve slots,
allocates missing handles, copies identity matrices, applies the color value,
binds four textures, initializes six special transforms, and marks initialized
and dirty state. The exact recovered floats occupy +0..31 of the owned region;
the preserved caller owns the final four meaningful bytes. Other globals and
resource labels remain address-bound external storage.

The source assumes valid arrays/resource indices and ordinary helper contracts.
Full code/relocation/data identity is established, not arbitrary invalid-memory
safety, independent full rendering behavior, actual gameplay, full-image
integration, compression or cartridge SHA-1. Merging and integration remain
with the independent checker.

## Independent peer review

The separate image-B lane reviewed frozen source/research commit `df0698e99`
and independently reran deep GNU verification under normal and optimized Python,
the routine test, the missing-IDO skip, and the bare strict group score. It
confirmed all five complete extents, 247 GNU relocations, 36 owned bytes and
12 zero alignment bytes, with no blocking findings. Its small stale sidecar
comment finding was corrected; one duplicate identical prototype was removed,
and the full acceptance receipt/tests were regenerated without body changes.
