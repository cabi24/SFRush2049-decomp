# engine_sound_sync: complete source/listener update

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `engine_sound_sync`, `0x80098AE4`, 1,236 bytes / 309 words.

Observed canonical O3 group result: **286 / 309 words differ**, **6 nonzero
excess words**, no unresolved symbols, unverified relocation sites or comparison
errors. This is a broad **NONMATCH**. Initial complete source with only one
cleanup caller inlined that cleanup and produced 287 / 309 with 41 excess;
restoring the other two genuine archived callers reduced excess to 5, before
the submitted native-shaped loop spelling produced the stated 6. No match,
accepted coverage, complete behavioral proof or original-source identity is claimed.

## Useful source recovery

The complete routine locks the source queue, saves each next pointer before
callbacks, resolves each source's effect, cleans up eligible inactive effects,
then projects its normalized direction into each listener's basis. Distance
attenuation is clamped below zero, pan is reduced by attenuation, weighted pan
and depth are accumulated, and four scalar values are clamped/submitted before
unlocking the queue.

Two decompiler ABI errors are removed: cleanup consumes the actual effect
pointer, and `entity_transform_calc` takes a handle plus four floats, not three
array pointers. The native specialized registers are supplied by compiling its
genuine accepted source. The three live indexed arrays correspond to native
20-byte-spaced scratch spans; capacity five and a list-count bound are explicit
assumptions, not a general safety claim. The normalized direction, projected
vector and 3x3 basis have their actual consumed sizes.

`transpose_basis` and `world_vector` express the observed mathematical operations
in the forms of arcade `LIB/fmath.c`'s `TransposeUV` and `WorldVector` (donor
`845329d7b36f5a384c5625ed9a0aef584ab46139`). Their original N64 inline boundaries
and names are hypotheses. They improve the natural frame from 352 to 368 bytes;
retail is 376. No unused locals, invented ABI arguments or artificial padding
are supplied for the missing eight bytes. A consumed local count control reached
the native frame but caused 67 excess words, so it is not submitted.

The existing `audio_effect_setup` context stays at its archived 6 / 46 differing
words, without excess, and is not a new claim. Other context is informational.
No source from a synthetic-caller group is used.

## Flags and exact genuine context

`-g0 -O3 -mips2 -G 0 -non_shared`; the canonical group scorer also supplies
mandatory `-r4300_mul` to `as1`. Run with the project's IDO 5.3 setup:

```sh
python3 - <<'PY'
import json, shutil, subprocess, tempfile
from pathlib import Path
base='f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2'
packet=Path('cloud/work/lean_engine_sound_sync_20261006')
context={
 'cleanup.c':'cloud/work/ipa-groups/codex_effect_typed_b132/group.c',
 'transform.c':'src/blob/groups/codex_transform_b109/group.c',
 'state.c':'src/blob/entity_state_check.c',
 'normalize.c':'src/blob/func_80098A54.c',
}
keep=['engine_sound_sync','func_80091FBC','func_8009211C','client_sync',
      'entity_hierarchy_update','scheduler_recv','stat_lap_complete',
      'game_timer_resume','entity_state_check','func_80098A54','func_80098FB8']
with tempfile.TemporaryDirectory(prefix='engine-sound-') as tmp:
    group=Path(tmp)
    shutil.copyfile(packet/'candidate.c',group/'candidate.c')
    for name,source in context.items():
        (group/name).write_bytes(subprocess.check_output(['git','show',base+':'+source]))
    (group/'group.json').write_text(json.dumps({
        'files':['candidate.c']+list(context), 'keep':keep,
        'members':['engine_sound_sync','audio_effect_setup'],
        'context':['entity_transform_calc','func_80091BA8','func_80091B00']+
                  keep[1:]+['func_800988D8','entity_flag_check','audio_pitch_adjust'],
        'claims':[], 'flags':'-g0 -O3 -mips2 -G 0 -non_shared'
    }))
    subprocess.run(['python3','tools/cloud/score.py','group',str(group)],check=False)
PY
```

Context is referenced from the immutable base instead of duplicating headers and
already-published callers. The O2 header on the unchanged normalizer records its
prior standalone recipe; this entire group was actually compiled O3.

## Limits / next lead

Native O32 layouts, valid source/effect pointers, finite float inputs, a list of
at most five listeners, and consistent list counts are the bounded source domain.
There is no added check that changes the native traversal on invalid data.
External queue/services, arbitrary aliasing and gameplay behavior have not been
independently tested. Preserving all accepted neighbors in a production unit
remains the checker's responsibility.

The current gap is broad allocation/scheduling and an eight-byte non-save frame
deficit. Actual matrix-helper boundaries and the live ranges of the three scratch
array bases are the next concrete leads. This lean packet has only C and these
notes: no test harness, receipts, independent verification, full tests, CI wait,
image/ROM integration or merge was performed.
