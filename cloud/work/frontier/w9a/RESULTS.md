# w9a results (wave 9, 2026-10-06)

Assignment: `steering_sensitivity` (0x800AD128, 928 bytes, component 3). Component 3 is
{steering_sensitivity, vector_diff_process, traction_control}; the other two were already locked, so the
component is complete with this match.

| Function | State | Flags | Scorer output |
|---|---|---|---|
| `steering_sensitivity` | **strict MATCH** (real -O3 group, no stand-ins; own .rodata verified) | `-g0 -O3 -mips2 -G 0 -non_shared` | `steering_sensitivity:` / `  MATCH` / `    own .rodata verified at 0x80123BFC..0x80123C00` (exit 0) |

Exact command (builder, own scratch copy synced to the current tree):

```
ssh watchman2 'cd ~/rush2049/scratch/frontier/w9a && IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py group cand/steerfinal --claims'
Members:
steering_sensitivity:
  MATCH
    own .rodata verified at 0x80123BFC..0x80123C00

Context (informational; excluded from exit status):
vector_diff_process:
  MATCH
traction_control:
  MATCH
func_800A61B0:
  MATCH
math_utility:
  MATCH
exit=0
```

Whole-program shadow check (Pi, repo root):

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w9a score steering_sensitivity --with <group.c> --neighbours
  EQUAL steering_sensitivity: 232 words (kept, c__u.c)
  locked bodies that differ in this unit: 0
blob_unit score: 1/1 equal
```

## Deliverable

`groups/steering_sensitivity/` (`group.json` with `"claims": ["steering_sensitivity"]`, `group.c`).
The group has the same files and keep list as the locked `src/blob/groups/frontier_traction_control`; only
steering_sensitivity's body is replaced (that group carries it as unmatched context). Integration options:
revert `frontier_traction_control` and splice this group, or replace the steering body in that group and
add steering_sensitivity to its members. The body alone is in `steering_sensitivity/best.c` (header comment in
`steering_sensitivity/hdr.c`). Not a `cloud/matches/` single: it sets vector_diff_process's register
parameters (t0/a1/a2/a3), so it only matches in the group.

## Semantics / recovered layout

N64-only path code (no arcade ancestor found). Places a world position in the cross section of path
segment `idx` (0x84-byte `PathRec` at `D_80152034`, count `*D_801526F0`, next record wraps to 0): copies the
segment basis to outMatrix, transforms the position into it, clamps the forward coordinate to
[0, skew1 (+0x60)], lerps +0x58 ("len0", the flat half-width) and +0x54 ("halfWidth") to the next record,
height = halfWidth - width - 2.5. Upside-down basis (`outMatrix[1][1] < 0`) is turned over with
`func_800AD090(0, -1)` and x/y mirrored (+5.0). Inside the flat part sets `outPosition[1]` and returns;
otherwise the point is in a rounded corner: distance from the corner centre, `outPosition[1] = height -
dist`, and below `threshold` the basis is rotated onto the surface with up to four sine/cosine rotations.
`0.01f` is its own rodata word at 0x80123BFC (0x3C23D70A).

## How it closed (and the order of the levers)

Start: prior best was 106/232 (`cloud/work/dot_steering_medium`), whose notes said "do not start a sweep".
Fresh natural body: 229/232 positional (frame 96 vs 104), structurally right. Then, each step guided by the
aligned diff and then the traced uopt (`tr/uopt` copied from w3a, `ctr.sh` / `force9.sh` here):

1. `y = rel[1];` assigned before the upside-down test (y lives in callee-saved $f22 across the call; the
   else path re-reads `rel[1]`): frame 104 with $f22 saved.
2. Ten scalar locals (frame slot count) - r1 at sp+96, dist 84, width 80, x 76, z 68 match the retail homes.
3. `z = 0;` (int literal) - retail materialises two separate zero registers (compare vs store).
4. One variable for `skew1` and the fraction (`frac = r0->skew1; ... frac = z / frac;`) - retail keeps both in
   $f0; a separate `t` took $f14.
5. Traced allocator: height and y were swapped ($f20/$f22) by priority, x and width tied on priority
   (2.6 each) with the tie broken by web number (first appearance). `force9.sh` with height=c30, y=c31 showed
   the rest was ordering. Separate statements `height -= width; height -= 2.5f;` and
   `x = rel[0]; x += r0->skew0;` placed after the lerps gave every web the retail colour and the retail
   schedule (final subtract in the `bc1f` delay slot). 24 of 360 statement orders of that block match.

About 680 compiles in total, almost all of them in five generated permutation batches (0.5 s each).

## Generalisable

- **Tie-breaking by web number**: two webs with identical priority are coloured in order of first appearance
  in the source. Split the assignment (`x = a; x += b;`) to move a web's first appearance without moving where
  its arithmetic is emitted.
- **`v -= a; v -= b;` vs `v = v - a - b`** changes both the priority (more references) and whether the chain is
  computed in the home register (retail `sub.s $f20,$f20,$f18` then `sub.s $f20,$f20,$f10`).
- A "clamp limit then divide by it" pair living in the same FP register means the C used **one variable**
  for both.
- The traced uopt from w3a works unchanged from another scratch directory: copy `uopt`, snapshot
  `unit/<tag>/stage/{merged,st}` after a `blob_unit --tag <you> score`, find the proc ordinal by diffing two
  traces (steering_sensitivity was proc 89 in this unit), then `CDX_PROC=89 CDX_DETAIL_WEB=all`. FP colours:
  24=$f0, 25=$f2, 26=$f12, 27=$f14, 28=$f16, 29=$f18, 30=$f20, 31=$f22.

## Unblocked by this match (not attempted; component 9, not assigned to me)

`camera_trigger_check` (1,232 B, unit with func_800C3AD0), `entity_update` (1,564 B), `input_deadzone_apply`
(3,580 B, unit with input_process_controller), and partly `camera_victory` (also needs camera_play_script).
Their only remaining unit blocker was steering_sensitivity.

## Tools in this directory

`run.sh BODY` (group score), `fd.sh BODY` (aligned diff in the group), `batch.sh BODY...` (2-core batch),
`ctr.sh BODY LABEL [PROC]` (unit score + traced colouring), `force9.sh LABEL PROC SPEC` (forced colouring).
`pre.c`/`post.c` are the frontier_traction_control group file split around the steering body.
