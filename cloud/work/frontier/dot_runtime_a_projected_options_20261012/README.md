# Image A projected-options callback: 59 → 3 differing words

## Deliverable and exact status

`groups/dot_runtime_a_seven_option_group_20261007/renderer.c` is the improved,
complete reconstruction of **A:func_803A6B50**, native extent
`0x803A6B50..0x803A6F14`, 964 bytes. It is **NONMATCH research**, not an
installable new match. The only residual is the 160-byte versus 144-byte frame:
prologue, incoming callback-argument home and epilogue, at offsets 0, 44, 952.
Every other instruction and all five owned angle literals are exact.

The self-contained genuine group preserves the existing name and complete
`func_80399394` member (88 bytes), with that member also in `claims` so the
installer cannot silently unlock it. Existing complete renderer/root context
`803993EC`/`80399D10` is copied unchanged from the installed group. The new
callback is complete, kept context, **not a claim**. No locked or production
source is changed. This draft is research; do not count the preserved helper
again or install the nonmatching callback.

Base: current master `1a2e0f005462f191ad755ccdcc3482673fc39a07`.
Newly matched bytes: **0**. Existing match preserved: **88 bytes**.
Accepted-image and full-ROM bytes: **0**. No behavior, hardware, recompression,
or gameplay equivalence is claimed.

## Trace-first source changes

The inherited `cloud/work/lean/runtime_a_menu_options_20261006` standalone
source reproduces **59/241 words differ**, same complete extent. Workbench
`diagnose` and the literal word comparison identify a temporary-register region
beginning in the nested text-X draw, plus live stack homes and a final draw
expression. No bulk sweep was used.

1. Compile the callback with its **actual 80399394 eligibility helper** and
   that helper's existing real callers. Without changing the callback, the
   score becomes **27/241** and the locked helper remains strict. This resolves
   the missing private-ABI context; no fake caller, extra formal or forced
   register is introduced.
2. Move the existing scalar declarations before the arrays. This makes every
   live point/text address native and gives **19/241**. Neither capacity changes.
3. Reuse the existing `title` pointer for the final menu label, then pass that
   pointer to the draw. This closes the final expression's evaluation/scheduling
   region and gives **3/241**. It adds no load, call, scalar declaration or storage.

Stock `ugen` diagnostics give M=92, T=20, P=4 and frame=160 at every improving
step. The complete diagnostic object is byte-identical to the ordinary stock
object. A bounded lexical-scope probe keeps M=92/frame160 and worsens the used
homes to 11/241; it is not retained. Further loop/title scopes also failed to
reduce the frame. Widening the real signed-short X carrier loses code identity
and adds words; it is not retained.

The final text base is native sp+92; its unchanged 50-byte object ends at 142,
inside the native 144-byte frame. The 4-byte projected point is native sp+88.
The 16-byte excess is consistent with the declared scalar-home set above the
live buffers. This is a measured residual, not proof of the original local
names, types, counts, or capacities. Shrinking the live text array would be an
unsupported shortcut and was not tried. No no-op reads, padding, unused locals,
compiler patches or guessed helper return values are used.

This result also leaves **A:803A7180** unchanged: its prior 2/296 frame residual
has no newly established safe source lever here, so old probes were not repeated.

## Verification and reproduction

The input-bound receipt authenticates the current-tree asset, loader pointer
and complete image A using protected metadata. There is no historical commit
or external checkout replay dependency. No ROM bytes, raw assembly, compiled
objects, extracted image or secrets are published.

```
python3 cloud/work/frontier/dot_runtime_a_projected_options_20261012/verify.py
python3 -O cloud/work/frontier/dot_runtime_a_projected_options_20261012/verify.py
python3 cloud/work/frontier/dot_runtime_a_projected_options_20261012/verify.py --score
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_a_projected_options.py -o addopts='' -q
```

The authenticated unmodified canonical scorer runs **without `--claims` or
`--allow-unverified`**. Its member and relevant context output is:

```
Members:
func_80399394:
  MATCH

Context (informational; excluded from exit status):
func_803A6B50:
  3/241 words differ
    own .rodata verified at 0x803B9820..0x803B9834
```

The existing other two nonmatching contexts remain informational. A zero group
exit status only preserves the already-matched helper; it does not accept the
new callback. Without authenticated image data, the callback also has ten
unverified section-relative references. Missing data and a corrupted literal
remain fail-closed controls.

The independent GNU route re-sections both complete ELF STT_FUNC bodies,
preserves every REL record, maps local text calls by their function owner,
and places the complete 20-byte angle pool at its authenticated address. GNU
as/ld confirms the exact same three callback differences and zero helper
differences. It does not hide frame immediates or accept a shorter function.
All remaining alignment bytes must be zero. The receipt pins source, verifier,
trace metadata, image-qualified target words and relevant protected metadata;
it does not pin tools, locks, Makefile or generated assembly text.

Optional finite stage/source probes, outside the routine test:

```
python3 cloud/work/frontier/dot_runtime_a_projected_options_20261012/trace_probes.py
```

The independent checker retains ownership of any future source acceptance,
installation, ROM validation and merging. Keep this research PR a draft.

## Independent review

A separate lane reviewed the frozen executable source, confirmed the existing
context is byte-identical to production, and replayed stock/GNU, Python `-O`,
the single test and the exact finite trace receipt. It requested explicit GNU
section-address and STT_FUNC address/size checks; these are now enforced for
both complete functions, with address/content checks for the literal section.
Routine replay passes in 0.15 seconds; no-IDO control skips in 0.07 seconds.
