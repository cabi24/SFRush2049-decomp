# w15j: own .rodata in more than one retail region (tooling)

**Result.** The group splice now accepts a member whose own `.rodata` windows sit in more than one
retail region, but only when it has complete evidence. The dry run of the w15b `physics_sym` group
on a scratch copy now passes the own-data gate. func_800B59F0's six windows are each verified at
their retail addresses: strings at 0x801226A8..0x801226C0 and floats at 0x80123DC4..0x80123DD4.
All five member bodies equal the image (0 word diffs). Nothing was installed. Nothing was
committed. `tools/cloud/owndata.py` and `tools/cloud/score.py` are unchanged.

## Design

### Where the change lives
The change is in `tools/conveyor/pipeline/blob_group.py` only, not in owndata.py:

- **Hash pins.** Two verification receipts pin owndata.py's exact SHA-256 (f689c3a7...):
  - `cloud/work/frontier/dot_runtime_a_flags_20261006/verification.json`
  - `cloud/work/runtime_b_reset_20261006/verification.json`

  Their tests (`tests/cloud/test_runtime_a_flags.py::test_receipt_is_bound_to_current_sources_and_target`
  and `tests/conveyor/test_runtime_b_reset_match.py::test_saved_input_identity`) fail on any edit to
  owndata.py. My first version extended `owndata.verify` with an opt-in `regions=` argument. It worked
  (same dry-run result), but it broke exactly those two tests, so I reverted it. Its diff is kept,
  NOT applied, as `owndata_alternative_not_applied.diff`. That version would be the cleaner home if
  the owner prefers to rebind those receipts.
- **Other gates unchanged.** owndata's one-base rule still holds for score.py, the single-function
  splice (`blob_splice.own_data_placements`) and blob_group's first-choice proofs.

### Order of proofs (unchanged except the last step)
1. Whole-section single base (`_local_data_bases.whole_section`).
2. Native jump-table windows (`_jump_table_windows`).
3. Per reference (`per_reference` → `_member_own_data`). Each member now goes through
   `verify_own_data`:
   - It runs `owndata.verify` exactly as before.
   - Only when owndata's failures are all the one-base refusal (`own <read-only section>: this
     function's references disagree on the section's image address`), with nothing unverified and
     with `data_runs` supplied, it tries `_independent_regions`.
   - If that fallback is refused, its reason is appended to the failures, so the result is still a
     refusal.

### What `_independent_regions` requires
It re-derives everything from the object and the retail words. It does not trust owndata's
intermediate state. Every condition below must hold:

- **(a) Every window verified.** owndata reported nothing unverified, and its only failures are the
  one-base refusal. So every window the function opens verified by content at its retail address.
  Any other failure (for example a third window that differs) refuses the fallback: "not only the
  one-base refusal".
- **(b) No unverified literal rides along.** Each window is compared over its whole extent
  `[offset, stop)`, where `stop` is the next object that any function in the object references.
  Trailing zeros are included, so no object byte a reference can reach is skipped.
  - The one exception is the window that ends the section. Its trailing zeros past the compared
    length are IDO's section padding, which owndata also leaves uncompared. They are allowed only
    if there are fewer than 16 of them (`SECTION_PADDING`).
  - Every access must fit in its window: 8 bytes for ldc1/sdc1/ld/sd, otherwise 4.
  - The section must carry no relocated words. Jump tables keep the existing one-base and table
    proofs.
- **(c) No overlaps.**
  - Each window must lie wholly inside one non-function run of the image (`data_runs`, taken from
    the layout's opaque entries by `opaque_runs(document)`), so it cannot sit on function code.
  - No two windows of the function may share image bytes.
  - Across members, the existing `_reference_windows` still refuses two object windows over the same
    image bytes and one object at two addresses. It now sees the whole extents.
  - The lock records no data placements for other locked functions, and these bytes are never
    emitted (the image keeps retail's data run). Every address comes from retail's own HI16/LO16
    words, not from us. Overlap with another function's literal would therefore be a retail fact
    that nothing can check here.

### Plumbing and fail-closed behaviour
- **Plumbing.** `relocate(..., data_runs=None)` passes the runs through to `_member_own_data`.
  `group_bodies` supplies `opaque_runs(document)`, which covers `splice` and the
  `blob_splice.spliced_bodies` rebuilds.
- **Fail closed.**
  - Without `data_runs`, behaviour is exactly as before.
  - Placements become `_ReferenceWindows` entries, as for any per-reference placement.
  - `Result.bases()` still raises for such a result, so the single-function link can never place it.

## Diff summary (`gate.diff`)
- `tools/conveyor/pipeline/blob_group.py` (+180 lines):
  - adds `ONE_BASE_REFUSAL`, `SECTION_PADDING`, `_independent_regions`, `verify_own_data` and `opaque_runs`;
  - `_member_own_data` and `relocate` take `data_runs`;
  - `group_bodies` passes it;
  - updates the docstrings.
- `tests/conveyor/test_owndata.py`, section "own .rodata in several retail regions" (8 tests, pure
  Python ELF, run on the Pi):
  - accepted in two regions (and still refused by owndata alone and without data runs; `bases()`
    still raises);
  - whole-extent compare catches a differing zero word;
  - refused when a window is unverified, and when another window fails;
  - refused when windows overlap;
  - refused outside the data runs (in code);
  - refused when an access is wider than its window;
  - refused when the section padding is 16 bytes or more;
  - refused for jump tables.
- `tests/conveyor/test_blob_group_own_data.py` (4 tests, gas, run on the Pi): one member through
  `relocate`:
  - accepted with data runs, and refused without them with the original message;
  - a wrong literal is refused;
  - a window inside function code is refused;
  - overlap: adjacent and equal literals still pass through the one-base path, swapped and disjoint
    ones are accepted, the same retail word for two objects is refused.

## Tests and gates (Pi, current tree, with the change)
```
$ python3 -m pytest tests/conveyor tests/cloud -m "not node_required" -p no:cacheprovider -o addopts="" -q
2096 passed, 905 skipped, 9 deselected in 325.98s (0:05:25)
$ python3 -m tools.conveyor.pipeline.blob_unit --tag w15j --remote-dir rush2049/scratch/frontier/w15j --jobs 2 check
blob_unit: 955 locked bodies, 955 equal to the image in one 872-file unit, 0 differ [926 plain; stub tail 28, own bss 2; 318 own .rodata/.data references in 98 bodies checked against image bytes] (914 kept, 156 internal; IDO 3.2s, total 5.5s)
$ python3 -m tools.conveyor.pipeline.blob_group check
group lock: 0 problems
$ python3 -m tools.conveyor.pipeline.blob_splice check
blob lock: 955 entries, 0 problems
```
(`-o addopts=""` only restores the summary line; the plain command prints dots only, with no F.)

## Dry run (scratch only)
The w15b group dir was copied to my session scratch. It was compiled with `blob_group.builder_script`
in builder scratch `~/rush2049/scratch/frontier/w15j` and the object was fetched to scratch. Then I
ran `blob_group.group_bodies` with `obj_dir`/`root` pointing at scratch. That is exactly the gate
`splice()` runs, without the image build or the lock write. Script: `dryrun.py`. Output: `dryrun.txt`.
```
source_sha 28a9bea6157034351fa8367a554e2dbcec3051f91627a02866af2212ab863c6d
members relocated: ['func_800B55F4', 'func_800B5688', 'func_800B59E8', 'func_800B59F0', 'physics_sym']
image mismatches: none
word diffs: {'func_800B55F4': 0, 'func_800B5688': 0, 'func_800B59E8': 0, 'func_800B59F0': 0, 'physics_sym': 0}
owndata only ok= False failures= ["own .rodata: this function's references disagree on the section's image address (0x801226A8 for +0x0/+0x10, 0x80123DAC for +0x18/+0x1c/+0x20/+0x24): the literals are not laid out as in retail"] unverified= []
data_runs ok= True failures= [] unverified= []
  placements= {'.rodata': [('0x0', '0x10', '0x801226a8', 'rodata'), ('0x10', '0x18', '0x801226b8', 'rodata'), ('0x18', '0x1c', '0x80123dc4', 'rodata'), ('0x1c', '0x20', '0x80123dc8', 'rodata'), ('0x20', '0x24', '0x80123dcc', 'rodata'), ('0x24', '0x28', '0x80123dd0', 'rodata')]}
  notes= ['own .rodata verified at 0x801226A8..0x801226C0', 'own .rodata verified at 0x80123DC4..0x80123DD4']
# control: the same gate with the fallback disabled (opaque_runs -> None) reproduces the install refusal
REFUSED: func_800B59F0: own .rodata: this function's references disagree on the section's image address (0x801226A8 for +0x0/+0x10, 0x80123DAC for +0x18/+0x1c/+0x20/+0x24): the literals are not laid out as in retail [per-reference placement tried because the whole-section placement was refused: .rodata: multiple placements need complete jump table evidence]
```
- **func_800B59F0's object `.rodata`** is 0x30 bytes:
  - "BUTTON_SELECT\0\0\0" at +0x0 (window 0x10);
  - "BUTTON\0\0" at +0x10 (0x8);
  - four floats at +0x18..+0x24;
  - 8 bytes of section padding.

  Every window is compared whole. Only those 8 padding bytes are uncompared (fewer than 16).
- **Not run in the dry run:** the full image gate (`blob_splice.build_with`). It regenerates asm/
  and build/ in the repo, which this lane must not touch. All five relocated bodies equal the
  extracted image bytes, so the gate is expected to pass. The coordinator's install runs it.

## Install command (coordinator, real repo)
Both helper stubs are locked singles (`src/blob/func_800B55F4.c` and `src/blob/func_800B59E8.c`,
verified 2026-09-29). They must be reverted for the group to take them, or `splice` refuses with
"already spliced outside this group".
```
PYTHONPATH=. python3 cloud/work/frontier/tools/install_group.py \
    cloud/work/frontier/w15b/groups/physics_sym physics_sym \
    --revert-single func_800B55F4 --revert-single func_800B59E8 \
    --provenance "frontier wave 15 (w15b), 2026-10-10; own .rodata in two retail regions verified by blob_group._independent_regions (w15j)"
python3 -m tools.conveyor.pipeline.blob_group check
python3 -m tools.conveyor.pipeline.blob_splice check
python3 -m tools.conveyor.pipeline.blob_unit check     # see the w15b integration notes: internal names / prefer_definition
python3 -m tools.conveyor.pipeline.blob_rom rom
```
The group.json already has a `provenance` field. Passing `--provenance` overrides it, so drop that
flag to keep w15b's text.

## Notes
- **score.py does not need to change for the install.** It still reports func_800B59F0 as
  NOT VERIFIED (owndata alone). Install goes through blob_group only. If the owner wants score.py to
  agree, the clean route is the owndata alternative diff plus rebinding the two receipts (or
  re-running their verify scripts). That decision is the owner's.
- **pause_quit (w6d)** looked like the same shape. It should pass through the same fallback if its
  own `.rodata` has no jump table. I did not test it.
