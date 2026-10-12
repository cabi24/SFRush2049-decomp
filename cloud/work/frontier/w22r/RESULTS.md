# w22r: recomp Phase 2 (function replacement + frame oracle) — tooling lane, worktree branch w22r

Status: done (2026-10-12). Not a matching lane: no src/, asm/, lock or coverage change.
Host: rocky, `~/rush2049/recomp/w22r` (copy of the Phase 1 tree + this branch's tools/recomp; nothing
ROM-derived committed). Design and commands: `tools/recomp/README.md` "Phase 2"; plan section 10.

## Results (oracle over 1800 frames = 60 s of game time, frame-clocked BOOT_LOG race script)

| Set (`tools/recomp/replacements.json`) | Verdict (first line of `oracle.py run`) | Replaced calls |
|---|---|---|
| `matched10` | `EQUIVALENT over 1800 frames (judged: all frames, 2 baseline run(s))` | render_helper 21344, euler_to_matrix 24192, entity_name_copy 22395, steering_apply 23768, model_transform_setup 76682, vector_normalize_length 98, pool_linked_list_init 3, func_8008A644 3907, native func_8009E820 138946, native func_8008B32C 21915 |
| `bug` (`bug/render_helper_flipped.c`: `value > 0.0f` -> `<`) | `DIFFERENT at frame 3: display lists (dl 1239f50a81542ab8 vs stock e52404c3fbc7d2ca, 4 vs 4 lists); 4 rdram page(s)` | 21344 |
| `draft_engine_sound_sync` (w21h best.c, 8 rows off) | `DIFFERENT at frame 706: 7 rdram page(s)` | 3599 |
| `draft_func_800D7634` (w21h best.c, 15 words off) | `NOT EXERCISED over 1800 frames` | 0 |
| (exploration) all 678 matched `src/blob` files that built in the first pass | `DIFFERENT at frame 1: 1 rdram page(s)`: only func_800E7710's local static (below) | 329 of 678 called |

Bug detail (word diff at frame 3, stacks and unjudged pages skipped): `D_80149B88 00000000 -> 00000010`
(the flag render_helper sets), `D_8012E608`, `D_8012E678`, and the display list at 0x80127330 gains
commands (`ee000000 ffff0000 e2001d00 ...`): exactly the flipped branch's effects.

engine_sound_sync draft verdict: **not behaviourally equivalent.** Exercised from the first race frame
(image B loads at frame 700); first judged difference frame 706, words
`D_8011024C 0000000c -> 00000002`, `D_80110288 00000014 -> 0000000a`, `gRenderSync+0xC 0x57 -> 0x45`, and the
links of a free list at 0x80142C64.. (`gFinishTimeByPlace+0x64` per the symbol table; stock 0x80142e38,
0x80142e50, ... in 0x18 steps; draft all 0x80142dd8 / 0x8002e8e8): the draft allocates/links audio
effects or voices differently. Next step for w22e: a Phase 3 per-call capture (arguments and written
words per call) would name the first differing call; meanwhile compare its list/count handling with the
retail asm around the effect allocation (func_80091BA8) and the out[] clamp helper.

func_800D7634: the 60 s script never opens the audio-options screen; a script that does is needed
(menu layout unknown without pixels; RT64 work on the other host will help).

The 678-at-once run (exploration, not committed; manifest generated on rocky): the only judged
difference in 1800 frames is D_80150F18 (the SI message buffer) holding 0x808139C0 instead of
0x80150F58: func_800E7710's function-local static (retail .bss 0x80150F58) is a private copy in the
replacement image and its address escapes as the SI event message. Placement, not behaviour. With
func_800E7710 left out (677) the first difference is frame 705, `D_801161D0 1 -> 0`; a bisect was
started on rocky (`runs/bisect.txt`); see "Open" below.

## Design (what was built)

- `tools/recomp/replacements.json`: entries `{function, population, source, mode, state, note}` and sets
  (`matched10`, `bug`, `draft_engine_sound_sync`, `draft_func_800D7634`).
- `tools/recomp/replace.py`: mode `ido` (default) = IDO 5.3/7.1 with the file's flags -> `ld.lld`
  per object at 0x80800000+ (undefined symbols by inventory or address-derived name; defined globals
  bound to the game's address; IDO intrinsic declarations without relocations ignored) -> ELF32 reader
  -> `.repl` section functions `repl__<id>__<name>`. Mode `native` = generated o32 wrapper
  `native__<id>(rdram, ctx)` + the unchanged source compiled for the host with the function renamed.
- `gen_config.py --replace-set/--replace/--ido53/--ido71/--ld`: appends `.repl` to the synthetic ROM
  and the symbol file, adds an entry hook per replaced function (`rush_repl_count`, call, `return`),
  writes `rush2049_repl.h`, records `replacements` in manifest.json. Without replacements the output is
  unchanged except the (empty) header.
- Host (`tools/recomp/runtime`): `oracle.cpp` (frame hook in `__scExec` via patches.json, display-list
  walk, page hashes with thread stacks masked, `--oracle-log/--frames/--dump-at`, replacement image
  load and call counts), `lockstep.cpp` (`--clock frames`: VI/AI from the idle thread, synchronous
  SP/DP, framebuffer model; virtual osGetCount/osGetTime), all through `-Wl,--wrap`; CMake builds the
  native wrappers; `check_contract.py --extra-src` checks them.
- `tools/recomp/oracle.py`: `compare` / `run` / `worddiff`.
- Tests (ROM-free): `tests/recomp/test_replace.py` (o32 layout, wrapper text, prototype parsing, native
  fit, manifest, flags, symbol resolution, ELF reader on a synthetic ELF, gen_config integration),
  `tests/recomp/test_oracle.py` (log parsing, page/DL/event diffs, baseline-unstable pages, horizon,
  stack-only, NOT EXERCISED, word diff; end-to-end skips without `RUSH2049_RECOMP_HOST`).

## Limits (stated with every verdict)

- One input script; NOT EXERCISED is no verdict.
- Thread stacks are masked (dead frames differ whenever code differs); three pages of ultramodern host
  state (OSThread contexts at 0x80034000-0x80035FFF, 0x80038000) are never judged.
- Function-local statics of a replaced body are private copies (equivalent unless the address escapes or
  an unreplaced function of the same unit shares them).
- Native mode: leaf bodies, aligned 32-bit memory through pointer arguments only (word-swapped rdram,
  8-byte host pointers, no game globals/callees). Pointers held in rdram structs are not attempted.

## Integration notes

Merge branch w22r as is (tools/recomp/**, tests/recomp/**, plan section 10, WAVE_BRIEF "Behavioural
check", this file, `bug/render_helper_flipped.c`). CLAIMS.md: no line to add for the drafts (neither is
"correct, not matched"); the note format for a future EQUIVALENT is in WAVE_BRIEF.

## Open

- 677-at-once divergence at frame 705 (`D_801161D0`): bisect result in `~/rush2049/recomp/w22r/runs/bisect.txt`
  if it finished; a matched body that diverges behaviourally would point at an unmatched-context
  assumption (unit flags, a shared static) rather than its code, since the IDO output is retail bytes.

## Permission denials

None.
