# Wave 22, lane w22e: func_800D7634 + func_800D6914 group (MATCH); engine_sound_sync group (not matched)

Builder scratch watchman2:~/rush2049/scratch/frontier/w22e. Nothing committed, spliced or pushed. No permission
denials. Variants and traces are in `<fn>/v/` (ignored).

| Function | Bytes | State | Flags | Scorer output |
|---|---:|---|---|---|
| func_800D7634 | 1804 | **MATCH** (group audio_options_menu) | -g0 -O3 -mips2 -G 0 -non_shared | unit `EQUAL func_800D7634: 451 words (kept, c_func_800D7634.c)`; group `func_800D7634:\n  MATCH\n    own .rodata verified at 0x801241D4..0x801241E8` |
| func_800D6914 | 1260 | **MATCH, code identical; own .rodata in two retail regions** (install via blob_group) | same | unit `EQUAL func_800D6914: 315 words (internal, c_func_800D7634.c)`; group: owndata line below |
| func_800D690C | 8 | **MATCH** (locked stub given its body) | same | unit `EQUAL func_800D690C: 2 words (internal, c_func_800D7634.c)`; group `MATCH` |
| engine_sound_sync | 1236 | not matched, unchanged from w21h (8 norm rows, frame 376 exact) | same | unit `FAIL engine_sound_sync: 98 of 309 words differ` |
| audio_effect_setup | 184 | EQUAL beside engine_sound_sync only (provisional, unchanged) | same | unit `EQUAL audio_effect_setup: 46 words (internal, c_aes.c)` |

## Group 1: audio_options_menu -- MATCH

Deliverable `groups/audio_options_menu/` (claims func_800D690C, func_800D6914, func_800D7634). The source header of
`func_800D7634.c` discloses every device.

Whole-program unit (Pi, current tree):
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w22e score func_800D7634 func_800D6914 func_800D690C \
  --with cloud/work/frontier/w22e/groups/audio_options_menu/func_800D7634.c --internal func_800D6914 \
  --internal func_800D690C --internal post_msg_a --internal post_msg_b --neighbours
  EQUAL func_800D7634: 451 words (kept, c_func_800D7634.c)
  EQUAL func_800D6914: 315 words (internal, c_func_800D7634.c)
  EQUAL func_800D690C: 2 words (internal, c_func_800D7634.c)
  locked bodies that differ in this unit: 0
blob_unit score: 3/3 equal
```
Group (builder, `score.py group cand/groups/audio_options_menu`, without --claims; same with --claims, exit 1 only
because of the func_800D6914 owndata line):
```
func_800D690C:
  MATCH
func_800D6914:
  NOT VERIFIED (12 section-relative relocations unverified: .rodata+0x1c at +0x1c, .rodata+0x1c at +0x20, .rodata+0x20 at +0x24, .rodata+0x20 at +0x28, .rodata+0x24 at +0x2c, .rodata+0x24 at +0x30, .rodata+0x0 at +0x238, .rodata+0x0 at +0x23c, .rodata+0x10 at +0x20c, .rodata+0x10 at +0x220, .rodata+0x28 at +0x2fc, .rodata+0x28 at +0x300; own .rodata: this function's references disagree on the section's image address (0x8011F70C for +0x0/+0x10, 0x80124194 for +0x1c/+0x20/+0x24/+0x28): the literals are not laid out as in retail)
func_800D7634:
  MATCH
    own .rodata verified at 0x801241D4..0x801241E8
```
func_800D6914 has no word differences; the NOT VERIFIED line is the expected split-.rodata case: strings
"BUTTON_SELECT"/"BUTTON" at 0x8011F70C/0x8011F71C, floats at 0x801241B0..0x801241BC. I checked the float bits
against the image: 1.5707964f = 0x3FC90FDB, 12.566371f = 0x41490FDB, 3.1415927f = 0x40490FDB, 6.2831855f =
0x40C90FDB. The sibling physics_sym group is in the same position. All context functions score MATCH in the group
except func_800D6160 (unmatched context, informational).

What closed it (traced, then one lever each):
1. **func_800D7634, 15 words -> EQUAL: a block boundary after post_msg_b().** I ran ctrace on the D_80110640
   address web (p1 w212). It is one web over all four track sites, with `forbidden0=0x70018000` (v0, v1, a0, s1,
   s2). force.sh with `p1:w212=c7` showed that all four sites are this one web. The v0 interference is
   func_80091B00's precoloured return value from the inlined post_msg_b, which shares a uopt block with the
   track-row address web. A compiled-out `do {} while (0);` right after `post_msg_b();` (only there; the same
   device after post_msg_a changes nothing) ends that block, and the web takes v0 at every site, as retail does.
2. **func_800D6914, 46 words -> EQUAL: func_800D690C is the row-visibility helper, not a D_80110638 getter.**
   func_800D690C becomes `s32 func_800D690C(s32 i) { s32 v; v = 1; return v; }`, the same body w21h called
   row_ok. It serves both functions:
   - func_800D7634's row-wrap do-while uses it (`while (!func_800D690C(D_80110638))`).
   - func_800D6914 uses it in the sibling func_800B59F0's visibility idiom:
     `if ((vis = func_800D690C(i)) == 0) { model_data_load x2 } else { model_transform_setup x2; y -= 40; }`.
   func_800D6914 now compares `i == D_80110638` directly, as retail does. That one change brings retail's
   s7 = &D_801105B8 and s8 = 1, the BUTTON-block temp ring, the `vis` slot between sel_y and tex (tex at 62) and
   the 8-byte inline area (frame 144). The direct compare also lets the uopt constants decide pi/180 as retail
   does. No compiled-out compare is needed. w21h's `if (a == k || X.count == 1) {}` also worked here (33 words
   with `if (D_801105B8) {}`), but the helper made it unnecessary.
3. post_msg_a/post_msg_b stay two copies. One shared `post_msg` with two call sites is not inlined
   (355/451 words).

Disclosed shaping: the `do {} while (0);` above, and unused locals that are the exact frame residual:
- func_800D7634 `padB` (256 vs 248, 1 slot).
- func_800D6914 `m[3][3]` + `k` (144 vs 104, 10 slots).
- func_800D6914 `cur` and `d` (1 slot each; removing either moves the homes: 8 and 4 words). The locked sibling
  func_800B59F0 has the same `cur`/`d`.
Helper identity hypotheses: func_800D690C is a constant-1 row predicate. It is supported by the stub position,
retail's unfolded `li v0,1; beqzl v0` in func_800D7634, and the s7/s8 webs plus the frame in func_800D6914.
post_msg_a/b are copies of func_800C8918's post_type1.

### Integration notes (group 1)
- Install `groups/audio_options_menu/` as a new group with `install_group.py ... --revert-single func_800D690C`.
  func_800D690C is a locked single stub that is given a body.
- unit_overrides:
  - `prefer_definition` func_800D7634 -> `src/blob/groups/audio_options_menu/func_800D7634.c`, because
    credits_scroll_grp/gr3_e.c carries an m2c context copy, kept there.
  - `force_internal` func_800D6914 and func_800D690C. The unit ran them with `--internal`; both are internal in
    retail.
  - post_msg_a/post_msg_b are non-static and not on keep, so they need no override.
- Context files:
  - Byte-identical copies: `codex_vsync_a145.c` (= src/blob/groups/codex_vsync_a145/group.c: speed_set and the
    two speed wrappers), `sync_entry_register.c`, `func_80091B00.c` (frontier_slot18_alloc), `func_80091AF0.c`,
    `func_80091AF8.c` (codex_heap_release_a25).
  - `func_800D6160.c` is a verbatim **excerpt** of codex_mode_init_a18/group.c lines 4137-4156
    (func_800D6160, physics_init_mode0/1) with only the declarations they need. The whole a18 file cannot be
    used: it also defines speed_set and the wrappers, and the unit takes those from codex_vsync_a145. Including
    physics_init_mode1 matters: without its second call site umerge inlines func_800D6160 into
    physics_init_mode0, and func_800D7634 stops inlining physics_init_mode0 (24 words).
- No data-symbol changes. Claims: +1804 + 1260 + 8 B (the stub is already counted if locked).

## Group 2: engine_sound_sync + audio_effect_setup + func_80098ADC -- not matched (stop rule)

The source is unchanged from w21h (`w21h/engine_sound_sync/best.c` + `aes.c`). Same unit result:
`FAIL engine_sound_sync: 98 of 309 words differ`, us.sh `words 117 ops 8 norm 8 | frame 376/376`,
`EQUAL audio_effect_setup: 46 words`, `EQUAL func_80098ADC: 2 words`, 0 locked bodies differ.

New measurements (traced):
- ugt.sh: the FP temp ring here is only $f4/$f6/$f8/$f10 (LRU). Ours has no FP allocation before line 84
  (`out[0] = source->parameter`, first ALLOC_FP -> $f4). Retail's $f8 there needs two earlier FP allocations
  that leave no code, or 6, 10 and so on. No ALLOC_FP in our trace is freed without emitted code, so no source
  form in the current draft produces one.
- A non-inlined FP procedure placed before engine_sound_sync (z1, probe only) does not move the ring, so ugen
  resets the ring per procedure (agrees with w21h's 303/303).
- **asm.sh oracle:** renaming the ring registers in loop 1 of our ugen listing (f4<->f8, f6<->f10) and
  reassembling leaves the same 8 normalised rows. **The ring offset is not the whole residual.** The
  cross-product block differs in which basis values go to the FP spill temps 120..140 and in what order. Retail
  spills right[2] to 124, then fwd[2] reuses 124 and right[0] goes to 136. Ours spills fwd[2] to 136. This is a
  uopt spill/colour decision upstream of ugen and as1.
- A cross-product helper at the func_80098ADC stub (y1, with the clamp written inline) is worse: 40 norm rows,
  frame 400.
- spill.sh output is saved (`build/trace/w22e/`, spill run on snapshot e0) for the next lane.
Best next hypothesis: trace the FP spill decisions of loop 1 (spill.sh CONFL/HOME plus ctrace FP webs: which
basis-element expression webs are coloured and which are spilled) and compare with retail's spill-home pattern
above. The variable that decides it is probably one more or one fewer FP expression web in the cross product,
e.g. basis rows read through the listener pointer rather than the local array for one operand. Fix that
before revisiting the ring offset.

## What generalises
1. **Precoloured call results forbid a register for every web in the same uopt block.** An inlined helper
   ending in a call (func_80091B00 -> v0) makes v0 unavailable to an address web whose range starts in that
   block. A compiled-out block end after the helper call frees it. Check `forbidden0` in ctrace's p1dec, then
   force the web to an unused colour to see every site it covers.
2. **A caller-less stub can serve several callers in the same file.** func_800D690C was assigned to one
   caller (as a getter) when it is a shared predicate that both callers inline. Try the sibling file's helper
   idiom (here physics_sym's `if ((vis = helper(i)) == 0)`) before adding compiled-out compares.
3. **A context file that needs part of a locked group file** (the rest duplicates another group's members)
   can be a verbatim excerpt. Keep every caller of an internal context function: physics_init_mode1 holds
   func_800D6160 out of line.
4. The ugen FP ring is reset per procedure, and renaming it in the listing does not fix as1 schedules that
   come from different uopt spill choices. Check spill homes before chasing ring offsets.
