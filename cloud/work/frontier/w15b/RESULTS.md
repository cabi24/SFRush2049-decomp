# w15b results (wave 15): physics_sym + func_800B5688 + func_800B59F0

**All five bodies of the pause-menu file tail are EQUAL in the whole-program unit with their real callers.**
There are no stand-ins, no implicit int and no false prototype. Deliverable: group `groups/physics_sym/`
(`group.c`, `group.json` with complete members and claims).

| Function | Bytes | State | Flags | Scorer output (exact) |
|---|---:|---|---|---|
| physics_sym | 1,252 | EQUAL in unit (kept) | -O3 | `EQUAL physics_sym: 313 words (kept, c_group.c)` |
| func_800B5688 | 196 | EQUAL in unit (internal, real caller physics_sym) | -O3 | `EQUAL func_800B5688: 49 words (internal, c_group.c)` |
| func_800B59F0 | 1,372 | EQUAL in unit (internal, real caller); code identical, own rodata checked by hand (see below) | -O3 | `EQUAL func_800B59F0: 343 words (internal, c_group.c)` |
| func_800B59E8 | 8 | EQUAL (locked stub given its body: inlined getter) | -O3 | `EQUAL func_800B59E8: 2 words (internal, c_group.c)` |
| func_800B55F4 | 8 | EQUAL (locked stub given its body: inlined helper) | -O3 | `EQUAL func_800B55F4: 2 words (internal, c_group.c)` |

Command (Pi, repo root; `tools/g3.sh` wraps it):
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w15b --jobs 2 score physics_sym func_800B5688 func_800B59F0 \
  func_800B59E8 func_800B55F4 --internal func_800B5688 --internal func_800B59F0 --internal func_800B59E8 \
  --internal func_800B55F4 --with cloud/work/frontier/w15b/groups/physics_sym/group.c --neighbours
  EQUAL physics_sym: 313 words (kept, c_group.c)
  EQUAL func_800B5688: 49 words (internal, c_group.c)
  EQUAL func_800B59F0: 343 words (internal, c_group.c)
  EQUAL func_800B59E8: 2 words (internal, c_group.c)
  EQUAL func_800B55F4: 2 words (internal, c_group.c)
  locked bodies that differ in this unit: 0
blob_unit score: 5/5 equal; object build/blob_unit/w15b/unit.o (6.9s)
```
Standalone group (builder scratch w15b, `score.py group cand/grp_physics_sym`):
```
func_800B55F4:  MATCH
func_800B5688:  MATCH
func_800B59E8:  MATCH
func_800B59F0:  NOT VERIFIED (12 section-relative relocations unverified: ... own .rodata: this function's references
  disagree on the section's image address (0x801226A8 for +0x0/+0x10, 0x80123DAC for +0x18/+0x1c/+0x20/+0x24):
  the literals are not laid out as in retail)
physics_sym:    MATCH
```
func_800B59F0's own literals: in retail, the strings and the floats are not contiguous, so the scorer cannot
verify them as one section. This is the same situation as w6d's pause_quit. I checked them by hand against the image
(`score.own_data().read`): 0x80123DC4..D0 = 0x3fc90fdb 0x41490fdb 0x40490fdb 0x40c90fdb = 1.5707964f, 12.566371f,
3.1415927f, 6.2831855f (the source spellings), and 0x801226A8 = "BUTTON_SELECT", 0x801226B8 = "BUTTON".

## Integration notes
- New group `physics_sym`. It supersedes no locked group. keep = [physics_sym], which is reached through a pointer
  table (no jal callers). The other four members are internal.
- **Locked stubs given bodies:** `func_800B55F4` (src/blob/func_800B55F4.c) and `func_800B59E8`
  (src/blob/func_800B59E8.c). Both are members. The locked single stubs also define these names, so the unit
  needs either the singles retired in favour of the group, or `prefer_definition` entries pointing at the group's
  group.c, plus `force_internal` if the derivation does not already make them internal. Their image bytes
  (`jr ra; nop`) do not change.
- func_800B5688 and func_800B59F0 are internal (unsaved callee-saved registers). They are not kept.
- `provisional.json`: the w12h provisional func_800B59F0 entry (implicit int, waiting on physics_sym) is
  resolved by this group. The new form needs no implicit int.
- physics_sym's other callees stay asm in the unit (particle_collision, sound_loop_set, func_800B4FB0). They do
  not block it.
- Own rodata: see above (hand-checked; the scorer's NOT VERIFIED comes from the non-contiguous layout).

## Disclosed shaping (also in group.c's header)
1. Unused locals that are the exact frame residual:
   - physics_sym `v`, `c` and `pad`. Removing any one, or any two, gives frame 144 and 2-14 words (k1-k7).
   - func_800B59F0 `cur` and `d`. Removing them gives frame 104.
   - func_800B59F0 declares `i, name, y` first. This is set by its homes (old 84, tex 70, sel_y 76); 30-variant layout sweep `physics_sym/fr/`.
2. Helper identities (hypotheses, with evidence):
   - **func_800B59E8 = `s32 f(s32 i) { s32 v; v = D_8011AD40[i]; return v; }`**, used by physics_sym's selection
     loop and by func_800B59F0's visibility test. Its inlined parameter and return temps are the "invisible
     holders" that w12h was looking for: they block v0/a0 in physics_sym's loop, and v0 in func_800B59F0, which
     gives `vis` v1 as in retail. That was the implicit-int residual, and it is now closed without implicit int.
   - Its 2-word stub sits exactly where a definition just before func_800B59F0 leaves it.
   - The form with a local `v` is required. With `return D_8011AD40[i];`, physics_sym is 52 words off (r4/r7).
   - **func_800B55F4 = `void f(void) { sound_handles_clear(0); }`**, defined before func_800B5688.
     - Traced: retail puts `sw zero,D_8011AC94` in the delay slot of `jal sound_handles_clear`.
     - An as1 listing-edit oracle (asm.sh) gives 0 rows only when the store's `.loc` line is greater than the line
       of the call's argument set-up while the store stays before the call in the listing.
     - A helper defined earlier in the file produces exactly that, because inlined code keeps the helper's
       (lower) lines. Its stub is at 0x800B55F4.
     - h2 (the helper also does `D_8011AD68 = 0;`) is equally EQUAL. The bytes cannot tell the two apart.
3. physics_sym's reset branch reads back the just-zeroed globals (`D_80146108[19] = D_8011AD50;` ...,
   `update_viewport(D_8011AD50, D_8011AD4C)`) (from w13e's w_e). The controller-present byte is `volatile`.
4. Old-style declarations with the correct return type, from w10e: `func_800B24EC()`, `model_data_load()`,
   `model_transform_setup()`. They are not false prototypes.

## How it was closed (trace log)
- **Baseline.** I re-scored the old drafts. w13e had already reached EQUAL for physics_sym (`v/w_e.c`, `v/z9.c`)
  before it was interrupted. That baseline used the static `item_visible` / `menu_exit` helpers and implicit int in B59F0:
  `w_e.c: EQUAL physics_sym: 313 words (kept, c_w_e.c) | words 0 ops 0 norm 0 | frame 152/152`.
- **g0, adding func_800B5688's body.** It was inlined away (4 words), and physics_sym was 66 words off.
  - Cause: `menu_exit` holds the only call of func_800B5688, so umerge inlines it (an internal callee with one
    call site).
  - `--block` does not work on internal names.
  - Fix (g1): write the exit tail out at both sites. physics_sym stays EQUAL.
- **g1, the stub tail.** The `item_visible` static then left a 2-word deleted stub after func_800B59F0, which is
  not in the image ("stub tail ... NOT as in the image").
  - With the helper removed (g2/g3), physics_sym is 57/102 words off.
  - `frontier stubs` lists a locked caller-less stub at **0x800B59E8**, right before func_800B59F0.
  - Defining the getter there (g4) makes everything EQUAL, including the stub.
- **func_800B5688's 4 as1 words** (w14d's residual), traced with as1t.sh and asm.sh:
  - (a) The `la s0` / `la s2` order: retail's order comes out when both are on one line.
    - The natural source for that is an indexed `for (i = 0; i < 12; i++)` over D_8011A994[i] (h3).
    - Loop strength reduction puts the pointer and the end pointer on the `for` line, and no pre-check appears
      because 0 < 12 is known.
  - (b) The delay slot: see helper func_800B55F4 above (h1/h2).
- **func_800B59F0 without implicit int.**
  - Using the getter in B59F0 too fixes the register (r1). It costs +8 frame and +16 homes.
  - The layout sweep found f10: drop `cur[4]` to one word and declare `i, name, y` first. This gives EQUAL.

## Other forms (for the owner)
- Implicit-int form: also EQUAL (h3.c: the getter only in physics_sym, func_8008D870 undeclared). It is no
  longer needed.
- Best form with no getter in B59F0 and with the prototype (h3p.c): func_800B59F0 is 2 words off
  (`+0x2b4 image 82e30000 unit 82e20000`, `+0x2c0 image 1460000b unit 1440000b`, vis v0 vs v1). The other four
  are EQUAL.

## What generalises
1. **Locked caller-less stubs next to an unmatched function are its inlined helpers. They are also where retail's
   "invisible register holders" come from.** An inlined call's parameter and return temps block argument and
   return registers in the caller. Before hunting colour levers for a "registers one up" residual, look for a stub
   in the same file region and try a getter or helper defined there.
2. **as1 delay-slot / same-line ties can come from inlined code's line numbers.** as1 breaks ties on the lower
   `.loc` line. Code inlined from a helper defined earlier keeps its lower lines. A store that retail puts in a
   jal delay slot "after" the argument set-up therefore suggests the call came from an earlier-defined helper.
   Prove it by editing the `.loc` in the listing (asm.sh) first.
3. **An internal callee with one call site is always inlined by umerge.** A static wrapper (menu_exit) around a
   call to an internal function makes that function disappear. Retail's two jal sites mean that the call was
   written at both sites.
4. **Indexed for loops over a global array** (`for (i = 0; i < N; i++) A[i].f`) give the strength-reduced
   pointer loop with no pre-check, and both `la` on one line. Prefer this to the hand-written pointer do-while
   (w14d) when as1 orders the base and end `addiu`.

## Files
- `groups/physics_sym/{group.c,group.json}`: the deliverable.
- `physics_sym/`: the variants.
  - g0-g10: helper, stub and loop steps.
  - h1-h3, h3p: helper and loop forms.
  - k1-k7: unused-local tests.
  - q1-q4, r1-r7, fr/f01-f30 (+ `fr/results.txt`): B59F0 without implicit int.
  - f10a.c is the source of group.c.
- `tools/g3.sh` (five-name unit score) and `tools/ps.sh` (physics_sym only).
- Builder scratch: `~/rush2049/scratch/frontier/w15b` (toolkit installed with --reuse wtk). Snapshots st_g4,
  st_g7, st_g8.

Environment note: about 23:00 the shared tree's unit_overrides.json was mid-integration (coordinator). For a few
minutes blob_unit exited 2 with `prefer_definition entity_hierarchy_update: src/blob/groups/frontier_mode_select/mode.c
does not define it (stale override?)`. I waited until the manifest built again and changed nothing.
I made a mistake with the first rsync to the builder: it created a stray `blob/` directory in my own scratch.
I removed that directory (only in my own scratch) and re-synced.
No permission denials. Nothing committed, spliced, or edited outside `cloud/work/frontier/w15b/`.
