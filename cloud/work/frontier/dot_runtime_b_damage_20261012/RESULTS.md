# Image B opponent-damage match

Deliverable: the complete `group.c` and one-member kept `group.json` in this
directory, named `dot_runtime_b_damage_20261012`. Population **B**,
`func_8038D3A4`, `[0x8038D3A4, 0x8038D498)`: **244 newly strict-matched bytes**.
No prior accepted or preserved bytes are credited. The checker retains installation,
ROM verification and merging; the PR stays draft by the owner's instruction.

## Result and replay

From the current checkout with stock IDO 5.3 and GNU MIPS binutils installed:

```
python3 cloud/work/frontier/dot_runtime_b_damage_20261012/verify.py --score
python3 cloud/work/frontier/dot_runtime_b_damage_20261012/verify.py
python3 -O cloud/work/frontier/dot_runtime_b_damage_20261012/verify.py
python3 -m pytest tests/cloud/test_dot_runtime_b_damage.py -q
```

The first command binds the authenticated current-tree image in memory, then calls
unmodified `score.main()` in group mode without `--claims` or `--allow-unverified`:

```
Members:
func_8038D3A4:
  MATCH
    own .rodata verified at 0x80394DC4..0x80394DC8
```

The group route is deliberate: current master has no `asm/us/ovl_b_data` artifact,
so a bare standalone cloud submission cannot verify the actual local float. This
packet carries normal C and the installation-ready group, plus its image binding;
it neither replaces the literal with a false external nor weakens scoring. There
is no synthetic caller, private-register dependency or unclaimed context.
Standalone O3 also matches under the same authenticated data binding.

## Method and authentic boundaries

Current master `262e9f0f368914320da53df08102da9682187381`, live locks and open PRs
were checked before reservation. The initial typed O2 body was 36/61 words off;
canonical O3 left only three words: comparison operand order and the corresponding
two team-table address additions. Victim-first forms for both equality expressions
remove exactly those differences. No sweeps, padding, pressure declarations,
volatile qualification, inlined invented helpers or compiler patches are used.

The accepted `src/blob/menu_options_screen.c` has the real three-argument callsites
(two player pointers plus signed damage). The accepted `effect_cleanup` body at
0x800C55E4 takes three signed bytes and returns void; its source forwards that
triple to the image-B event producer. The new source uses this genuine prototype.
No static ROM TU is rebuilt by this packet; the runtime image uses pinned IDO 5.3.
Original names, whole-function arcade ancestry and original record spellings are
unknown. Field names describe the observed operations.

The function rejects disabled victims, self-hits and same-team hits; flag 4 scales
damage by binary32 0.2. It records damage, subtracts health, clamps depleted health
to zero, emits the one-point event, then marks the victim disabled. The final
player index is reloaded after the external event call, preserving helper effects.

## Evidence and limits

- Whole native body and STT_FUNC: 244 bytes, exact entry address, no missing body.
  Complete object/linked `.text`: 256 bytes, with exactly 12 checked zero alignment
  bytes. The 24-byte o32 frame and all nine relocations are checked.
- Actual owned literal: four bytes at 0x80394DC4. Complete `.rodata`: 16 bytes,
  with exactly 12 checked zero alignment bytes. These padding bytes are not claimed
  to overwrite neighboring image literals. Installation must use the normal
  owned-data placement/reuse gate.
- Independent GNU placement explicitly pins both `.text` and `.rodata`; all 61
  linked instructions and all four meaningful literal bytes equal the image.
  All external symbols resolve to their selected current target addresses.
- Receipts bind packet sources/verifier/harness, selected native extent/words,
  image and literal identities, flags and compiler family. No scorer, pipeline,
  lock, Makefile or assembly-text hash is pinned. Current-tree asset bytes are
  read only; no historical non-ancestor Git replay is needed.
- Reversing the two equality expressions again reproduces the three-word residual.
  Missing own-data and corrupted literal controls fail closed. The actual C source
  passes 96 bounded fixtures under UBSan/bounds and strict aliasing, plus a callback
  mutation proving the post-call index reload. Whole player/vehicle records are
  compared, including opaque fields.
- The single collected test is below one second locally; the shared missing-IDO
  guard is used. Optimized-Python replay is also checked.

The host fixtures do not execute the native body or real helper, and are not a
full-gameplay equivalence proof. Valid backed player/team/vehicle indices, defined
signed arithmetic and representable float-to-int conversion remain caller contracts.
Narrow signed-halfword conversions retain the fixed IDO target's behavior. No
image/splice/ROM/hardware execution, accepted byte gain or unrestricted safety is
claimed. Already integrated B:803908D0 and B:803936A8 are excluded.
