# w15i: making the mode-select cluster installable through the group path

Lane w15i (wave 15). Packaging only: w13a's member sources are unchanged.

**Result:** `score.py group` gives all 10 claims MATCH, and 8 of the 10
context bodies MATCH. A blob_group-style relocation dry run against the image
gives 0 differing words for all 10 members. blob_unit with the member files
gives 10/10 EQUAL and 0 locked bodies differing. A simulated post-install unit
(fake repo, lock and overrides) gives 948/948 locked bodies EQUAL, **with no
new unit_overrides entries**.

Group: `cloud/work/frontier/w15i/groups/frontier_mode_select/`

| file | origin |
|---|---|
| d5e64.c, mode.c, msh.c, ded78.c | byte-identical to `cloud/work/frontier/w13a/groups/frontier_mode_select/` (claimed members) |
| func_8008B2E4.c | byte-identical to `src/blob/func_8008B2E4.c` (locked single) |
| player_conditional_check.c | byte-identical to `src/blob/player_conditional_check.c` (locked single) |
| codex_transform_b109.c | `src/blob/groups/codex_transform_b109/group.c`, but **its 5-line `entity_hierarchy_update` definition is replaced by its prototype** `void entity_hierarchy_update(s32 h,f32 value);`. Nothing else changes. This is the same strip blob_unit applies to a non-canonical definition. |

An alternative that keeps the context byte-identical and changes one member
file instead is in `groups_alt/frontier_mode_select/`. It is also fully
verified; see "Alternative" below.

## Why the w13a group failed standalone

The w13a group compiled only the 4 member files. Everything else was external,
so umerge could not inline the locked helpers and the internal IPA convention
of `entity_transform_calc` was missing:

| member | w13a standalone | cause (proved by the drop tests below) |
|---|---|---|
| mode_select_input 15/38, func_800DFBA0 13/298 ($f28/$f30), func_800E05F0 | differ | `entity_transform_calc` is **internal** in the unit (codex_transform_b109 does not keep it). Retail passes its float arguments in $f22/$f24/$f26/$f28 (IPA), from the inlined `entity_hierarchy_update`. Without b109's source it is an ordinary external call. |
| mode_select_handler, func_800E0050 | differ | umerge in the unit inlines the locked kept single `func_8008B2E4` (6x into mode_select_handler, 2x into E0050) |
| func_800E0050 | differ | umerge also inlines the locked kept single `player_conditional_check` |

From the unit (`blob_unit` umerge log, tag w15i) for the w13a component:
mode_select_handler inlines func_800DEF60 and func_8008B2E4; mode_select_input
inlines entity_hierarchy_update; func_800E0050 inlines entity_hierarchy_update,
player_conditional_check and func_8008B2E4; func_800E05F0 inlines
entity_hierarchy_update and func_800E0048. The group reproduces exactly this
inlining (`umerge -v` on the builder).

## Why one entity_hierarchy_update definition had to go

mode.c defines `__inline void entity_hierarchy_update` and b109's group.c
defines it too. When both are in the group (v1, and v3 with reversed file
order), uld warns `entity_hierarchy_update: multiply defined`, keeps one, and
merges the symbol information. The kept out-of-line body then homes a0..a3
(`sw a2,80(sp)`, `sw a3,84(sp)`, so it acts as if it had 4 parameters). That
grows func_800E05F0's frame from 224 to 232 (13/332 words differ), and
entity_hierarchy_update is 29/34. So exactly one definition must be present.
The options I tried:

| variant | what | members |
|---|---|---|
| v1/v3 | both definitions (either order) | E05F0 13/332 |
| v4/v5 | mode.c gets a **plain** prototype, b109 copy unchanged | umerge does not inline it: mode_select_input 28/38, E0050 323/360, E05F0 287/332, DFBA0 13/298 |
| v6 | mode.c `static __inline` | members match, but the deleted static leaves an unnamed 2-word `jr ra; nop` after mode_select_handler. That gives "1 extra words (nonzero beyond target length)", which the splice refuses |
| **v2 = primary** | b109 copy's definition replaced by its prototype; mode.c unchanged | **10/10 MATCH** |
| **vM = alternative** | b109 copy byte-identical; mode.c's definition replaced by `__inline void entity_hierarchy_update(s32 h,f32 value);` | **10/10 MATCH** |

The `__inline` prototype is what makes umerge inline the other file's
definition. That is also why the real unit matches with either canonical
definition (see the post-install simulation).

## Exact `score.py group` output (builder, scratch copy, primary group)

`IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a.../ido python3 tools/cloud/score.py group cand/final`
(exit 0). The `--claims` run also exits 0. Saved verbatim in
`score_group_final.txt` / `score_group_final_claims.txt`:

```
Members:
func_800D5E64:
  MATCH
func_800E05F0:
  MATCH
    own .rodata verified at 0x8012438C..0x80124394
best_times_display:
  MATCH
mode_select_handler:
  MATCH
    own .rodata verified at 0x80124324..0x80124370
func_800DED78:
  MATCH
    own .rodata verified at 0x80124320..0x80124324
func_800DEF60:
  MATCH
mode_select_input:
  MATCH
func_800DFBA0:
  MATCH
    own .rodata verified at 0x80124370..0x80124378
func_800E0050:
  MATCH
    own .rodata verified at 0x80124378..0x8012438C
func_800E0048:
  MATCH

Context (informational; excluded from exit status):
entity_transform_calc:
  MATCH
func_80091BA8:
  MATCH
func_80091B00:
    +0x01c  want a06f0003 sb t7,3(v1)                   got 2418ffff li t8,-1
    ... (11 rows: sb/li/sh store order in each unrolled copy)
  11/42 words differ
client_sync:
  MATCH
entity_hierarchy_update:
  MATCH
scheduler_recv:
  MATCH
stat_lap_complete:
  MATCH
game_timer_resume:
  MATCH
func_8008B2E4:
  MATCH
player_conditional_check:
  MISMATCH (1 extra words (nonzero beyond target length))
```

### The two context bodies that do not print MATCH

- **func_80091B00 (11/42)**: this is b109's own copy of the slot allocator.
  It scores the same 11/42 in the locked group codex_transform_b109 itself
  (I scored that group on the builder). The lock and the unit use
  frontier_slot18_alloc's volatile version. That version stays canonical after
  install (func_80091B00's lock entry is group frontier_slot18_alloc). Its
  schedule does not shape the members: they MATCH both with b109's copy and
  with frontier_slot18_alloc's func_80091B00.c added first (v10). It is not
  spliced and not verified here.
- **player_conditional_check (MISMATCH, 0 differing words)**: all 17 words are
  equal. The 1 "extra word" is the deleted static `rand` from func_8008B2E4.c.
  It is inlined, and its unnamed `jr ra; nop` is emitted before
  func_8008B2E4, so it counts toward the preceding function's extent. It is
  context, so the splice never checks or outputs it.
  `blob_group compile` (include_context) will print
  `context not compared: player_conditional_check: 1 extra words` and then
  compare the members only.
- All other context bodies are compared and MATCH. No context body is
  unverified for a missing target.

## Context list and why each entry is needed

| context | from | role | needed? (drop test, builder) |
|---|---|---|---|
| entity_transform_calc | codex_transform_b109.c | **internal** (not kept): its IPA float-register convention shapes mode_select_input, func_800DFBA0 and func_800E05F0, through the inlined entity_hierarchy_update | yes. v9 (no b109 file): mode_select_input 15/38, DFBA0 13/298, E0050 322/360, E05F0 156/332 |
| func_80091BA8, func_80091B00 | same file | internal callees of entity_transform_calc/scheduler_recv (internal in the unit too) | come with the file |
| client_sync, entity_hierarchy_update, scheduler_recv, stat_lap_complete, game_timer_resume | same file (entity_hierarchy_update's body comes from mode.c) | kept, as in b109's keep list and in the unit | come with the file. Kept so that the only internal procedures are the ones the unit makes internal |
| func_8008B2E4 | func_8008B2E4.c | kept locked single that umerge inlines into mode_select_handler and func_800E0050 | yes. v7: mode_select_handler 626/744, E0050 357/360, E05F0 300/332 |
| player_conditional_check | player_conditional_check.c | kept locked single that umerge inlines into func_800E0050 | yes. v8: E0050 294/360 |

keep = the w13a keep (func_800D5E64, func_800E05F0), b109's keep list
(client_sync, entity_hierarchy_update, scheduler_recv, stat_lap_complete,
game_timer_resume), func_8008B2E4 and player_conditional_check. There are no
stand-ins. The non-kept set (8 internal members + entity_transform_calc,
func_80091BA8, func_80091B00) is exactly what the unit already makes internal.
Installing therefore makes no new function internal in the unit.

## Image dry run (Pi, read-only)

I fetched the builder object to the scratchpad. I then ran
`blob_group.group_bodies("frontier_mode_select", blob_layout.load(), obj_dir=<scratch>, root=<scratch copy of the group>)`,
which uses the same member_slices/relocate/extra-word checks as splice, and
`word_diffs` against the image:

```
member func_800D5E64 146 words, 0 differ
member func_800E05F0 332 words, 0 differ
member best_times_display 56 words, 0 differ
member mode_select_handler 744 words, 0 differ
member func_800DED78 122 words, 0 differ
member func_800DEF60 2 words, 0 differ
member mode_select_input 38 words, 0 differ
member func_800DFBA0 298 words, 0 differ
member func_800E0050 360 words, 0 differ
member func_800E0048 2 words, 0 differ
context not compared: player_conditional_check: 1 extra words (nonzero beyond target length)
```

I did not run the image gate or the ROM build. I did not splice anything.

## blob_unit (Pi)

Pre-install, the command as asked, with the group's 4 member files
(`unit_members.log`):

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w15i score func_800D5E64 best_times_display mode_select_handler func_800DED78 func_800DEF60 func_800DFBA0 mode_select_input func_800E0048 func_800E0050 func_800E05F0 \
  --with cloud/work/frontier/w15i/groups/frontier_mode_select/d5e64.c --with .../mode.c --with .../msh.c --with .../ded78.c \
  --internal best_times_display --internal mode_select_input --internal func_800DFBA0 --internal mode_select_handler \
  --internal func_800DED78 --internal func_800E0050 --internal func_800DEF60 --internal func_800E0048 --neighbours
  EQUAL func_800D5E64: 146 words (kept, c_d5e64.c)
  EQUAL best_times_display: 56 words (internal, c_d5e64.c)
  EQUAL mode_select_handler: 744 words (internal, c_msh.c)
  EQUAL func_800DED78: 122 words (internal, c_ded78.c)
  EQUAL func_800DEF60: 2 words (internal, c_msh.c)
  EQUAL func_800DFBA0: 298 words (internal, c_mode.c)
  EQUAL mode_select_input: 38 words (internal, c_mode.c)
  EQUAL func_800E0048: 2 words (internal, c_mode.c)
  EQUAL func_800E0050: 360 words (internal, c_mode.c)
  EQUAL func_800E05F0: 332 words (kept, c_mode.c)
  locked bodies that differ in this unit: 0
blob_unit score: 10/10 equal
```

I also ran it with all 7 files as `--with` (tag w15i7, `unit_all7.log`):
10/10 EQUAL, but 1 locked body differs: func_80091B00. That is an artifact
of `--with`. A candidate file wins every name it defines outright, so b109's
copy of func_80091B00 displaced frontier_slot18_alloc's. After install, the
lock owner's definition stays canonical; the simulation below shows this.

### Post-install simulation (no repo files touched)

I built a fake repo in the scratchpad: a copy of src/blob plus
`groups/frontier_mode_select` (group.json without claims), a copy of the lock
with func_800DEF60/func_800E0048 removed (as `--revert-single` does) and all
10 members entered with `"group": "frontier_mode_select"`, and a copy of
unit_overrides.json. I monkeypatched `blob_unit.read_inputs` and
`load_overrides` to use them, then ran a full `Run(...).build()` and compared
every locked body:

| overrides added | result |
|---|---|
| force_internal func_800DEF60 + func_800E0048, prefer_definition entity_hierarchy_update -> group mode.c (tag w15ipost) | 948/948 locked bodies EQUAL |
| prefer_definition only (tag w15ipost2) | 948/948 EQUAL |
| **none** (tag w15ipost3) | **948/948 EQUAL**. entity_hierarchy_update canonical = b109 group.c; mode.c's definition is staged as an `__inline` prototype, and umerge still inlines it into all three callers |

In every case the derived roles are: func_800D5E64 and func_800E05F0 kept;
the other 8 internal (as non-kept group members, so the force_internal entries
are redundant after install); entity_transform_calc, func_80091BA8 and
func_80091B00 internal; client_sync, entity_hierarchy_update, func_8008B2E4
and player_conditional_check kept; no problems, no unused overrides.

## unit_overrides.json entries for the install

**None are required** (simulation above). If the coordinator still wants to
record w13a's hand decisions, these exact entries were also simulated
(948/948):

```json
"force_internal": [
  {"name": "func_800DEF60", "reason": "w13a/w15i: deleted check_forces_on_car helper; inlined into mode_select_handler, only the retail 2-word stub remains (also derived internal as a non-kept member of frontier_mode_select)"},
  {"name": "func_800E0048", "reason": "w13a/w15i: deleted camera wrapper (identity hypothesis); inlined into func_800E05F0, only the retail 2-word stub remains (also derived internal as a non-kept member of frontier_mode_select)"}
],
"prefer_definition": [
  {"name": "entity_hierarchy_update", "file": "src/blob/groups/frontier_mode_select/mode.c", "reason": "retail inlines it into mode_select_input, func_800E0050 and func_800E05F0; mode.c's __inline definition gives the locked 34-word body (not required: the staged __inline prototype makes umerge inline b109's definition too, w15i simulation)"}
]
```

(Append these to the existing lists. Do not replace the lists.)

## Install command

Not run by me. From the repo root on the Pi:

```
PYTHONPATH=. python3 cloud/work/frontier/tools/install_group.py \
  cloud/work/frontier/w15i/groups/frontier_mode_select frontier_mode_select \
  --revert-single func_800DEF60 --revert-single func_800E0048 \
  --provenance "frontier wave 13 (w13a) bodies, wave 15 (w15i) group packaging; cloud/work/frontier/w13a/RESULTS.md, cloud/work/frontier/w15i/RESULTS.md"
```

Then run, as its docstring says: `blob_group check`, `blob_splice check`,
`blob_unit check`, `blob_rom rom`. No group is superseded. func_800DEF60 and
func_800E0048 are today locked -O2 2-word stub singles; after the install they
become (identical 2-word) members of this group.

## Alternative: byte-identical context (`groups_alt/frontier_mode_select/`)

In this variant, codex_transform_b109.c is byte-identical to the locked
group.c, and mode.c's definition becomes
`__inline void entity_hierarchy_update(s32 h,f32 value);`. That is a
member-file change, and it is exactly the text blob_unit stages for mode.c
when b109's definition is canonical. Results:

- `score.py group`: 10/10 MATCH, the same context results
  (`score_group_alt.txt`).
- blob_unit `--tag w15iM` with its 4 member files: 10/10 EQUAL, 0 locked
  differ (`unit_alt_members.log`).

Use it if byte-identical context is preferred over byte-identical members. It
also needs no overrides.

## Disclosures

- The primary group's codex_transform_b109.c is **not** byte-identical to its
  locked source: one definition is stripped to a prototype, for the reason
  above.
- func_80091B00 is a context body that does not match (as in b109 itself).
  player_conditional_check matches word for word, but score.py reports it as
  MISMATCH because of the rand stub.
- Builder runs used `~/rush2049/scratch/frontier/w15i` only, at nice, one
  compile at a time. The unit tags used were w15i, w15i7, w15iM, w15ipost,
  w15ipost2 and w15ipost3 (`build/blob_unit/<tag>`).
