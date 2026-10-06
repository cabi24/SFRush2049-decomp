# w12h results (wave 12): research on the hardest near-misses

Assignment: func_8008E408 (22/386) and func_800B59F0 (2/343, plus its real caller physics_sym).
Flags everywhere: `-g0 -O3 -mips2 -G 0 -non_shared`. All scores are in the whole-program unit (tag w12h).

| Function | Bytes | State | Flags | Scorer output (exact) |
|---|---:|---|---|---|
| func_8008E408 | 1,544 | **EQUAL in the unit** (kept; no stand-ins). Delivered as `cloud/matches/func_8008E408.c`. Shaping is disclosed (see below). | -O3 | `EQUAL func_8008E408: 386 words (kept, c_func_8008E408.c)` / `locked bodies that differ in this unit: 0` |
| func_800B59F0 | 1,372 | **provisional**: EQUAL with the stand-in callers and with the real caller physics_sym (draft). No false prototype. Not spliceable until physics_sym matches. | -O3 | `EQUAL func_800B59F0: 343 words (internal, c_best.c)` / `locked bodies that differ in this unit: 0` |
| physics_sym | 1,252 | scout draft: 4 rows with 8 forced colours | -O3 | `FAIL physics_sym: 208 of 313 words differ; compiled body is 315 words, target 313` (forced: `want 313 words, got 313; differing rows 4 (words); frame 152/152`) |

Commands (Pi, repo root):
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w12h --jobs 2 score func_8008E408 \
    --with cloud/matches/func_8008E408.c --neighbours
  EQUAL func_8008E408: 386 words (kept, c_func_8008E408.c)
  locked bodies that differ in this unit: 0
blob_unit score: 1/1 equal; object build/blob_unit/w12h/unit.o (8.4s)

python3 -m tools.conveyor.pipeline.blob_unit --tag w12h --jobs 2 score func_800B59F0 --internal func_800B59F0 \
    --keep zz_caller --keep zz_caller2 --block func_800B59F0 \
    --with cloud/work/frontier/w12h/func_800B59F0/best.c --neighbours
  EQUAL func_800B59F0: 343 words (internal, c_best.c)
  locked bodies that differ in this unit: 0
blob_unit score: 1/1 equal; object build/blob_unit/w12h/unit.o (5.8s)

python3 -m tools.conveyor.pipeline.blob_unit --tag w12h --jobs 2 score func_800B59F0 physics_sym \
    --internal func_800B59F0 --with cloud/work/frontier/w12h/physics_sym/best.c --neighbours
  EQUAL func_800B59F0: 343 words (internal, c_best.c)
  FAIL physics_sym: 208 of 313 words differ; compiled body is 315 words, target 313
       umerge inlined into it: menu_exit
  locked bodies that differ in this unit: 0
blob_unit score: 1/2 equal; object build/blob_unit/w12h/unit.o (5.8s)
```
`score.py fn` on its own does not match func_8008E408 (+0x24 onward): the function needs the whole-program
unit, because it inlines the locked func_8008B2E4 and needs the unit's IPA. The unit result above is the
evidence. The splice path checks func_8008E408's own literals (0.0333333f, 70, 0.75, 0.25, 90, 0.85, 0.15,
0.5, 1.25 and Random's 1.0f/32768) against the image. They are unchanged from w9c's best.c.

## func_8008E408: the +4 was the merged frame (closed)

I checked w11b's finding first, and it explains the whole residual. umerge gives each inlined callee's area an
8-aligned start, in call order. Retail's merged frame is 204, which is 4 mod 8. That is only possible when the
last inlined call has an area of an odd number of words. Before this change, the two inlined func_8008B2E4 calls
were the last ones, so the frame was always a multiple of 8. The measurement table is in
`func_8008E408/notes.md`. Summary:
- 3 or 4 of the bottom pads removed (cfe 0xa4/0xa0) plus one trailing inlined callee with a single 4-byte local
  gives merged 0xcc = 204 and EQUAL. An empty trailing helper gives 0xc8 (64 rows). Without the pads removed it
  gives 0xd4 (42 rows).
- The natural form: the final `o->w52 = func_8008E26C(D_8014295A[o->h86], o->m, -1, 0x40000)` sits in a
  `static void attach(Obj *o) { s32 h; h = func_8008E26C(...); o->w52 = h; }`. The s32-returning versions with a
  local also match. `return func_8008E26C(...)` without a local does not (64 rows).
- No extra coloured web and no zero-code live value is needed. w10h's "web X" hypothesis is not needed.

Disclosed shaping (also in the file header):
1. The helper `attach` is a shaping helper whose real identity is unknown. It is static with one call site and
   leaves no stub (`frontier show`: no stub_neighbours).
2. 18 unused locals (pB1, pC1..pC7, pD1, pD2, pE1..pE3, pF0..pF4; w9c had 22). They are the exact frame residual:
   they reproduce every retail local offset. Under the wave-12 rule they are acceptable only as disclosed frame
   residual. **The owner should review this before splicing.**
3. The w9c shaping that was already there is unchanged (dead `snap` read, `if (type > 3)` compiled-out check,
   `u32 type`).

Integration: a plain single, `cloud/matches/func_8008E408.c`. It supersedes no group and needs no unit_overrides.
It also unblocks entity_spawn_init (func_8008E408 was its sole blocker).

## func_800B59F0: v0→v1 without a false prototype (provisional)

The colouring trace (`tools/trace/ctrace.sh`) shows that vis (w82, save 20) costs 0 in every caller-saved
register, so it takes v0. In w10e's quirk it gets v1 because its forbidden mask includes v0 (0x40020000):
the second func_8008D870 call is declared to return a value, so v0 is defined in vis's block.
The same effect without a false prototype: **no declaration of func_8008D870 in scope** (a C89 implicit `int`
declaration, the usual result of a header that is not included). Nothing is assigned from the call.
- `v/k1.c` (= best.c): no prototype, `(s16)` casts on the handle arguments. Retail loads the low half
  (lh 6/262), which the prototype's s16 parameter used to supply. Result: EQUAL.
- `v/k2.c`: explicit old-style `int func_8008D870();`. Also EQUAL, which confirms that the mechanism is the int
  return type. This one is a false declaration, so it is not proposed.
- `v/k3.c`: `void func_8008D870();` (no parameters). Still 2 words off.
- Inlined getter for D_8011AD40[i] (`g1..g4`; the frame is compensated with smaller filler): still v0, 8-12 words
  off. Reusing `name` for vis: 41 words off.

**Caveat for the owner.** This is honest-looking source: real code often called functions with no header in
scope. But the int return is the same mechanism as the rejected quirk. Whether "no prototype" counts as acceptable
is the owner's call. The match also stays provisional until physics_sym matches. Nothing to splice now.

## physics_sym (real caller of func_800B59F0): scout, 4 rows with forced colours

A first draft written from the disassembly (`physics_sym/best.c`; no prior source existed).
physics_sym is reached through a pointer table, so it has no `jal` callers. It is the pause-menu input handler:
- btn/rep/hold pad words: D_8015694C/D_80149784/D_80156944 when `D_801174B4 & 0x7C03FFFE`, else
  `D_80156998/D_80143A00/D_80156978[D_8015698C]`.
- particle_collision when `D_8011AD30 == 0`; a viDeadlinePassed latch on D_8011AD54/58/5C.
- D_8011AD58 sub-mode (resource_type_select / audio_doppler toggling D_8011AD5C).
- Otherwise: `btn & 5` → D_8011AD3C = 2; `hold & 0x3000` → view offset D_8011AD4C/D_8011AD50, clamped to
  ±32, mirrored in bytes D_80146108[20]/[19], then update_viewport; `btn & 0xC00` → step the selection
  D_8011AD44 over the visible entries D_8011AD40[]; `btn & 2` with selection 3 → reset all.
- Then func_800B59F0(); the controller-present byte `D_80156CF0[ctrl].b0` (volatile) toggles D_8011AD6C and
  sound_loop_set.
- The menu_exit tail (`func_800B5688`, then D_801174B8 = 4/16 or func_800B4FB0(1)) is one inlined static
  helper with two call sites.

What closed structure: volatile on the controller record (retail reloads b0), a named loop value
`v = D_8011AD40[D_8011AD44]` (coloured in retail), frame 152 with homes btn 136 / rep 132 / hold 128 /
dz 120 / ds 116, and the menu_exit helper.

Residual (forced colours):
`force.sh psb physics_sym "p1:w61=c2,p1:w103=c4,p1:w166=c5,p1:w172=c6,p1:w173=c7,p1:w91=c8,p1:w155=c7,p1:w57=c6"`
→ `differing rows 4`. The forced webs are: the selection value sel → v1, v → a1, &D_8011AD44 → a2,
&D_8011AD40 → a3, constant 3 → t0, ds → t1, constant 1 → t0, dz → a3.
- Retail's loop webs all sit one register "up": something blocks v0 and a0 throughout the loop. Making only
  audio_distance_atten implicit-int (`physics_sym/s1_implicit_atten.c`) gives sel = v1 (the same mechanism as
  func_800B59F0). Retail's other calls are consistent with void prototypes. The blocker for a0 is still unknown.
- The last 4 rows are an as1 choice: retail puts `sw zero,D_80161398` in the update_viewport delay slot, ours puts
  `move a1,zero` there. Changing the store's position, its line number (edited listing via asm.sh) or putting it
  inside the argument list does not reproduce retail.
- Not working: reusing hold/rep/btn for the loop value, inlined helpers for the switch/loop (h1-h3), implicit
  declarations for every callee (s2-s4: it puts v0 in the wrong places after audio_doppler).
- physics_sym's own unmatched callees (particle_collision, sound_loop_set, func_800B5688, func_800B4FB0) stay
  asm in the unit. The prologue already matches with the saves of s0-s8 and f20-f30, so they do not block a
  physics_sym match in the unit.
About 40 physics_sym variants in total. Next step: find the web that holds a0 across the selection loop. One way:
force `&D_8011AD44` to a2 alone (55 rows) and diff the forbidden masks.

## What generalises

1. **A +4 spill-temp shift with an odd-word frame start means an inlined callee with an odd-word area last in
   call order.** A helper whose result goes into a named s32 local is a plausible source. An empty helper or a
   `return f()` helper does not reserve the word. Measure `Udef Mmt` (`tools/mu.sh BODY NAME CONSTS`) before
   looking for an extra coloured web.
2. **An implicit declaration (no prototype) of a void callee makes v0 busy at the call.** A web that starts after
   the call in the same block then moves from v0 to v1. This is the honest counterpart of the "discarded
   s32 result" quirk. Only the int return matters: `int f();` and no declaration give the same code, `void f();`
   does not. The handle arguments then need explicit casts where the prototype narrowed them.
3. A function that is only reached through a table can still be matched in the unit with its asm callees left
   as asm. Its prologue saves came out right here.

## Files
- `cloud/matches/func_8008E408.c` (deliverable). `func_8008E408/{best.c,notes.md,v/}`: the variant sweep
  (d*/e*/l* = pad and helper sweep, a1-a7 = helper forms).
- `func_800B59F0/{best.c,v/}`: k1 = best, k2/k3 controls, g1-g4 getters, n1. The physics_sym drafts p*/q*/r*/s*/t*/u*/w*/h* are also in `v/`.
- `physics_sym/{best.c,s1_implicit_atten.c}`.
- `tools/mu.sh` (unit score + cfe/merged Udef), `tools/b5.sh` (B59F0 with stand-ins), `tools/ps.sh`
  (physics_sym + internal B59F0), and the w11b copies `cfedef.py`/`udump.py`. Builder scratch:
  `~/rush2049/scratch/frontier/w12h` (trace toolkit installed via `--reuse wtk`).

Not done: `tools/workbench.py diagnose`. Its CLI wants reference/candidate objects. The force oracle answered the
same question (colour-only, plus one as1 row). No permission denials. Nothing committed, spliced, or edited
outside `cloud/work/frontier/w12h/` and `cloud/matches/func_8008E408.c`.
