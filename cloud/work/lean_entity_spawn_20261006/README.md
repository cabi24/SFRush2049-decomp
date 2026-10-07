# entity_spawn_init: complete smoke/effect-spawn research

Research base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `entity_spawn_init`, `0x8008EA10`, 5,544 bytes / 1,386 words.

## Observed result

The complete C body compiles in a genuine O3 group. The canonical scorer reports
**1,356 / 1,386 words differ**, **5 nonzero excess words**, **22 unverified
section-relative relocation sites**, no unresolved symbols, and no additional
comparison errors. It is a broad **NONMATCH**. The emitted frame is 520 bytes;
retail uses 728. The local scratch repaired decompiler baseline was 1,370 / 1,386
with 9 excess words. A signed-surface control was 1,339 / 1,386 with 6 excess;
the submitted source instead preserves the native unsigned-halfword initial
load and signed comparisons. These are local compile observations, not tests,
acceptance, original-source proof, or cartridge coverage.

This replaces the old `bigfish/entity_spawn_init.md` missing-switch-table
blocker with a complete source lead. The current protected own-data artifact
supplies the eight actual case targets. Genuine calls to the accepted
`func_8008B2E4` source reproduce the repeated inlined random operations. No
invented caller, inline assembly, pressure-only local, or artificial stack
array is added to the candidate. Position, delta and previous-position locals
are actual three-float vectors. The six color snapshots are actual entry loads.

The behavior is related to `StartSmoke` in arcade `game/visuals.c` at donor
commit `845329d7b36f5a384c5625ed9a0aef584ab46139`; this is a behavioral ancestry
lead, not a claim that the expanded N64 routine is that donor verbatim. N64
source includes eight effect types, repeated spawn counts, state-bit gates,
randomized offsets/scales, texture selection, interpolated side effects,
viewport visibility and per-record color updates.

## Exact recipe and required real context

Candidate flags: `-g0 -O3 -mips2 -G 0 -non_shared`.
Canonical group scoring also passes mandatory `-r4300_mul` to `as1`.
Use the project's documented IDO 5.3 toolchain. The following command, run from
this repository, stages the unchanged accepted context directly from the
immutable research base and scores the submitted candidate. It writes only a
temporary directory. Every helper below is real; `claims` is intentionally empty.
The eight context files are references rather than duplicated large headers.

```sh
python3 - <<'PY'
import json, shutil, subprocess, tempfile
from pathlib import Path
base = 'f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2'
packet = Path('cloud/work/lean_entity_spawn_20261006')
context = {
 'random.c': 'src/blob/func_8008B2E4.c',
 'wheel_effect.c': 'src/blob/groups/frontier_func_8008E408/func_8008E408.c',
 'normalize.c': 'src/blob/groups/frontier_vector_normalize/group.c',
 'allocator.c': 'src/blob/func_8008E26C.c',
 'pool.c': 'src/blob/func_8008E3C0.c',
 'texture.c': 'src/blob/func_8008D870.c',
 'show.c': 'src/blob/model_transform_setup.c',
 'hide.c': 'src/blob/model_data_load.c',
}
keep = ['entity_spawn_init', 'func_8008B2E4', 'func_8008E408',
        'func_8008E098', 'func_8008E0B8', 'func_8008E26C',
        'func_8008E3C0', 'func_8008D870', 'model_transform_setup',
        'model_data_load']
with tempfile.TemporaryDirectory(prefix='entity-spawn-') as tmp:
    group = Path(tmp)
    shutil.copyfile(packet / 'candidate.c', group / 'candidate.c')
    for name, source in context.items():
        (group / name).write_bytes(subprocess.check_output(
            ['git', 'show', base + ':' + source]))
    (group / 'group.json').write_text(json.dumps({
        'files': ['candidate.c'] + list(context),
        'keep': keep, 'members': ['entity_spawn_init'],
        'context': keep[1:], 'claims': [],
        'flags': '-g0 -O3 -mips2 -G 0 -non_shared'
    }))
    subprocess.run(['python3', 'tools/cloud/score.py', 'group', str(group)],
                   check=False)
PY
```

The unchanged accepted wheel-effect context carries its already-disclosed
historical frame shaping. It is not a new candidate device or a new claim.
Context scores are informational; this packet does not replace accepted
production sources or claim their preservation in another build recipe.

## Assumptions and next useful lead

- O32 layouts, big-endian halfword views of handles and native 32-bit arithmetic
  apply. Explicit structure gaps represent observed field offsets, not inferred
  hidden fields. Array bounds beyond the actual strides are not established.
- Expected caller domain is a valid car/model slot and effect type 0 through 7.
  Types 6 and 7 require a readable three-float offset. As in the native control
  flow, the saved prior vector is consumed only when the existing-side state
  permits repeated interpolation.
- External service behavior, float conversion edge cases, gameplay behavior and
  aliasing outside the recovered layouts have not been independently tested.
- Broad allocation/scheduling differences and a 208-byte frame deficit remain.
  The next lead is authentic inlined-source/frame ownership, not padding or
  random declaration sweeps. The unverified relocation sites are not masked or
  counted as matches.

Lean publication contains only this note and the candidate C. No additional
verification harness, receipts, full test run, CI wait, image integration or ROM
verification was performed. The independent checker owns acceptance and merging.
