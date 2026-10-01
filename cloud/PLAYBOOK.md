# Cloud matching playbook (one page)

What worked in cloud rounds 1-3 (single functions and IPA groups). Setup and rules are in
[CloudHandoff.md](../CloudHandoff.md); group state and open blockers are in
[CloudHandoffV2.md](../CloudHandoffV2.md). Everything here was verified by strict scoring.

## The six rules

1. **Score strictly on the real bytes, every time.** `python3 tools/cloud/score.py fn|group ...`
   (it adds `-r4300_mul` itself). A compile plus strict compare takes about 0.3 s, so search
   instead of reasoning about one variant. While working, also compare *aligned by instruction*
   (`cloud/work/bigfish/near.py`): a positional score is misleading, because one extra or missing
   word shifts every word after it. "0 differing" without true strict MATCH is only a lead.
2. **Decide ABI or IPA first.** If the function reads non-ABI registers on entry (`t0`-`t3`,
   `s*`, `f16`+) or keeps values in caller-save registers across calls, no source change matches
   it alone. Build a small closed group with its callers/callees (`cloud/work/tools/closure.py`,
   `callers.py`). `func_800EA3F4` only matched as an `-O3` group. Functions with ABI-only callees
   are the best targets.
3. **Write natural source, not m2c-shaped source.** Typed struct arrays, plain `for` loops and
   real element types on externs fixed about half the single functions. Drop m2c's `spNN` spill
   locals and goto-loops unless the target needs them.
4. **Search, don't ponder.** When a function is structurally right and only register names or
   instruction order differ, script a mutation search (declaration order, temporaries, casts,
   loop forms) scored by aligned words. Keep the multi-line source layout: line layout changes
   IDO's scheduling.
5. **Scout before committing to a big function.** Check ABI vs IPA, callee count, globals, and
   how close a hand-written first pass gets. A truncated prefix cannot be scored strictly
   (frame and registers depend on the whole function), so a go/no-go on a 2,500-word function
   costs the whole function.
6. **Know when to stop.** About 50 variants with no movement on the same difference means the
   cause is in IDO/`as1`, not the source. Write it up in the group's `STATUS.md` and move on.

## Quirks that worked repeatedly

- **Frame size counts named locals.** IDO reserves a stack slot for every named scalar local,
  even one held in a register. Use expression style; an unused local or `volatile s32 pad[n]`
  reproduces an exact frame. Locals are laid out in declaration order, first at the highest address.
- **`do/while` vs `for`** compile differently (peeling, unrolling, `bnezl`); swapping fixed several functions.
  A known trip count invites unrolling: use a goto loop or an `s16` counter to stop it, or an `s32`
  counter to get the peel-2/unroll-4 shape.
- **Shared exits:** write every early exit as `result = N;` with one final `return result;`.
- **Parameter types:** an `s16` parameter adds `sll/sra` and a spill; use `s32`. A K&R callee
  prototype (`s32 f();`) removes `lh` narrowing of arguments.
- **One `lui at` for several stores:** `as1` merges them only when the symbol is *defined* in the
  same unit, not `extern` (and only for one array, in loop form).
- **`(u32)` laundering:** passing a pointer through a `(u32)` cast stops uopt hoisting loads above
  stores (`c = (PCar *)(u32)&player_array[i];`).
- **`volatile`** fields/globals stop reuse where the target does not reuse (`D_8002EB94` in
  `func_800EA3F4`).
- **Literal types:** int literals (`0`, `1 / len`) and float literals (`0.0f`) form different
  constant webs; a `Nf` where the target uses an int constant, or the reverse, rotates registers.
- **Compound forms:** `x *= 2` vs `x <<= 1` swap `sll`/`srl` order; scale-then-add
  (`v *= 1 - b; v += w * b;`) and `(u32) slot * 0x18` (no reuse of a hoisted `li 24`) matched FP/index code.
- **Stand-ins:** a missing callee can be stood in for by a function that clobbers the right
  registers; keep roots in `keep`, give each IPA function two call sites or `-O3` inlines it, and a
  dead `if (0) { switch ... }` in a callee stops inlining without changing words. A stand-in can
  never be spliced.
- **Debug listing:** `cc -K` leaves `a.s`, the listing before `as1` scheduling, which separates
  ugen's register choices from `as1`'s hoisting.

## Honesty notes

- Some matches depend on compile-affecting quirks (`volatile`, unused locals, literal types). They
  match the bytes but may not be the original source; say so in the PR.
- Claim only functions that score strict MATCH: put them in `group.json` `"claims"`. Keep scorer
  scripts in `cloud/work/tools/`, not in group dirs. Hand back matched singles in `cloud/matches/`.

## See also

- [Snowboard Kids decomp: DECOMPILATION_LEARNINGS.md](https://github.com/cdlewis/snowboardkids-decomp/blob/main/DECOMPILATION_LEARNINGS.md)
  Generic IDO 5.3 notes (register allocation measured with an instrumented `uopt`, stack-frame
  arithmetic, loop unrolling rules, struct/global access). Measured at `-O2 -mips1`; check a rule
  on our flags first. No licence declared: link to it, do not copy it into this repo.
- **Vendored workbench (CC0):** `third_party/n64-decomp-workbench/` — run `diagnose` on a near-miss first; it names
  the residual and the lever (`python3 tools/workbench.py guide` for the field guide and IDO 5.3 laws). The temp-ring
  levers (14-16) explain the `t6`-`t9` register-rotation wall.

## Workbench pilot findings (2026-10-01; logs in `cloud/work/workbench_pilot_*.md`)

Eighteen near-misses were worked with `diagnose` in the loop: 7 matched, 4 matched only as whole-program groups, 7 stay
2-44 words off. Four of the 7 (`func_800D0B14`, `func_800EAFDC`, `func_800966D8`, `func_8008A644`) had already eaten
50-1,000 blind variants from earlier agents. What worked, in the order to try it:

- **Read the lanes first.** `identical N/N` on the pool lane and a differing temp lane means ugen's temp ring; the other way
  round means uopt's coloured webs. The verdict moving structure -> allocation -> schedule is a reliable progress meter.
  At a true `MATCH` diagnose can still report 1-4 words (trailing padding nop): only `tools/cloud/score.py` is the gate.
- **Colouring order (float and integer webs).** The first-coloured web gets the lowest register (`f2` before `f12`, `v0`
  before `v1`). A code-free dead read such as `f32 t = 0.0f; if (G) {}` or `if (param + 1) {}` reorders a whole pool lane
  without a stack slot (`func_800EAFDC`, `func_800D0B14`, `func_800966D8`). An *uninitialised* dead read instead adds an 8-byte frame.
- **Temp-ring pop.** A redundant mask on a narrow store or shift (`& 0xFFFF`) supplies the missing pop (levers 15/16,
  `func_800966D8`, `func_8008A644`). The ring is four wide (`t9` wraps to `t6`) when the allocator reserved registers; in a
  function with no named variables it is wider (`f4 f6 f8 f10 f16 f18`), so levers 14-16 shift phase but cannot change width.
- **Line placement is a scheduling barrier on plain `cc` too** (no `acpp` needed): put the last stores on the same physical
  line as the statement before (lever 33: `func_800D4D84`, a callee loop's `for` header and body on two lines).
  Token-identical newline sweeps alone rarely close a gap (all 1,024 layouts of `func_8008A704` gave 2 or 5 words).
- **Frame first.** Drop m2c's `pad`/`sp1C`-style locals before chasing registers (`func_800CCE5C` 15 -> 2 words); a named
  local's type (`u16` vs `s32`) can change which copy feeds a mask.
- **Check the seed, not just the diff.** `func_8010E828` was a missing first call argument; `func_8008B000`'s seed had the
  wrong element size. Re-derive from the asm when the verdict stays `structure-mismatch`.
- **The `t6`-`t9` wall is a whole-program effect.** A function compiled as a non-exported `-O3` group member with two call
  sites reproduces the ring from plain C (`input_aux_handler`, `func_800C7200`, `func_8008ABE4`, `func_800B7438`); these
  cannot match alone, and a group with stand-in callers is not spliceable: they need the real callers in the group.
- m2c noise that matters: `0x400 & 0xFFFFFFFFFFFFFFFF` adds a `beqzl` and an extra `andi`; deleting a pass-through local
  moves a neighbouring spill slot. A dead read of an uninitialised local can match yet is not the original source: say so.


## Codex acceptance findings (2026-10-01)

- `func_800B7438` and `func_800B73E4` reproduce with real `input_init_flag_get` context: no stand-ins needed. Keep the externally used wrapper and real callee in `keep`; define `D_801551E8` for the shared-lui scheduling quirk.
- Standalone known unit-defined globals need absolute image assignments, rather than GNU ld `PROVIDE`, which leaves the object's own definition in newly allocated storage. This explained every differing word in `func_800BB7F4` and `render_post_process`; both now pass the image and ROM gates without changing their proven O2 source.
- New head `func_8010C7F4`: natural typed-vector source fixes seed signed load, stat-word dereference and float argument handling. Unused local reproduces the 72-byte frame and home slots. `func_80105DA8`: signed-width indexing and removing a pass-through local fixes the final reversed branch operands. Both strict-score/image/ROM accepted.
- Static seeds require stack-sensitive verification: `osViGetFramebuffer` and `osContStartReadData2` were historical masked zeros; reversing declaration order fixes the saved return-value slot. An object match remains a lead until promoted and full-ROM verified.

- Six conservative switch-head proofs are now registered; cloud head audits must treat their old tail labels as interiors of registered extents. `func_80104704` remains refused because its branch enters the existing `highscore_entry_anim` head; permissive cloud discovery is not registration authority.

## Static cartridge acceptance (2026-10-01)

Eleven static object matches (1,008 slot bytes) promoted behind full-ROM gates; static coverage is 34/230, 2,456/61,440 bytes. Shared-TU declarations matter even when the full seed matched: `inflate_flush_window` needed a nonvolatile view of `gDisplayListSize` because the shared header's volatile declaration changed stores and introduced an extra instruction. Its failed first promotion rolled back cleanly; the corrected source independently reverified and then passed the ROM gate. Do not change existing locked allocator declarations to make a new body fit.

Static conversions move asm to `asm/us/nonmatchings/rom/`; both target extraction and seed indexes must retain those inputs. Establish the passthrough baseline after explicitly syncing the new sources/assembly/linker/config to the builder: the game blob workflow alone only syncs its compressed blob and Makefile.

## Callback symbol and line-join follow-up (2026-10-01)

save_load_data's final ori/addiu mismatch was a hard-coded callback address: naming real drone_ai_update produces strict MATCH and exact image/ROM bytes. The other three one-word packet members need the real sound_update_channel IPA calling convention; stand-in closures cannot be claimed.

The cloud catalog now has token-preserving line_join scheduling mutations, guarded against comments, preprocessing continuations and line-dependent macros. It reproduces the existing D4D84 pilot but yields 0/8 both before and after on the fixed bounded held-out protocol. See cloud/work/line_join_codex.md; no new coverage or generalization gain is claimed.

## Real closure acceptance and static expansion (2026-10-01)

Real sound closure now accepts mode_byte_set, mode_byte2_set and the two byte getters: unreachable switch in the real empty debug callee preserves IPA metadata without adding runtime effects. Real input_aux_handler needs only its actual game_loop caller to reproduce allocation. Task completion needs a real already-locked neighboring comparator for module alignment; invented padding helpers are unaccepted. All claimed bodies pass image and full-ROM gates; unmatched context is preserved.

func_800C7200 matches with both actual callers after real seed repair, nonvoid exhaustion fall-through and same-line initializers. This is an explicit C undefined-return quirk, not proof of original source; exact fixed-compiler retail bytes are the acceptance evidence. 8ABE4 still needs its final six-word web repair.

random_seed_init uses a code-free argument read to exchange colored webs. D0C0 instead simplifies its natural loop and removes pass-through locals, matching without dummy reads. Static acceptance expanded to 50/230, 4,308/61,440 bytes. Canonical float ABI aliases and converted-slot indexing repair four stale target objects through existing round-trip gates; no scorer masks were weakened. Shared declarations retain existing signed bzero_alt body and correct DP wrapper's 64-bit size argument.


## Static GU errata and real helper follow-up (2026-10-01)

Explicit `-Wab,-r4300_mul` in isolated scoring is decisive for guMtxF2L, guOrthoF and guPerspectiveF. Makefile ROM builds already supply it, so reconcile shared flag evidence only after re-verifying every accepted body in those segments, not by changing optimization pins arbitrarily. Five shared accepted bodies remain strict/raw0; full ROM passes after all three helper promotions. Existing gOrthoScale holds the perspective radians constant; reference the actual retail symbol instead of assuming new local .rodata will link correctly.

Real sound extra callers and control-settings helpers add376+268 bytes, with actual audited callee/caller context and claimed image/ROM equality. The latter needs real control_settings's ten call sites to reproduce unsaved s0/s1. Unclaimed context still has external IPA and rodata gaps; do not treat a strict helper match as proof of its entire caller.

Current game errata resweep gives zero immediate matches among79 attempted seeds (56 strict nonmatches,23 compile/score failures), plus12 already locked andone seed failure. Six42-word matrix seeds improve to5 differing words after real stack-home/expression repair but remain unaccepted. Prefer missing vector-output/prototype repairs over another uncontrolled formatting batch. Static coverage now66/230,8,532/61,440 bytes; game521/1,216,60,604/647,072 bytes, reported separately.


## PFS, controller and actual symbol signatures (2026-10-01)

Current retail labels often differ from SDK names: derive prototypes from actual arguments and canonical bodies before shared-header integration. C7–C10 prove messaging, controllers, PFS identities, motor, directory and PI helpers. C8 preserves accepted pointer-valued s32 __osInsertTimer with explicit casts; changing its return declaration would conflict with its locked body. PIF status aliases are accepted only when authoritative addresses prove __osSiDmaRetry == __osSiDmaBuffer + 0x3c; strict scoring remains unchanged.

__OSDir retains size32 but requires data_sum@0xA, unsigned ext_name@0xC and unsigned game_name@0x10. The old header offsets were wrong. Independent current-header body verification plus53 unchanged existing C-TU text comparisons establish compatibility; full forced-ROM rebuild remains final authority. Raw comparison streams stay in ignored build/private scratch; checked-in evidence records identities/counts/equality.

Static preparation must lock candidate flags BEFORE deriving/writing opt_overrides.mk. If conversion predates a new O1 lock, regenerate the fragment with pipeline.layout.write_opt_overrides(), sync it, and prove the passthrough ROM baseline. C10's event setter initially failed because its generated override lacked the newly locked O1 pin; rollback restored the exact ROM. No candidate-body change is warranted for a compiler-flag mismatch. Shared-TU pins continue to block O2-only osCreatePiManager and O1-only osSendMesg; do not override accepted neighboring flags.

Game A5 closes a real inlined audio getter; B10 adds only D9CC/DF90. E828 was already accepted and is archived as re-verification without extra credit. B13 adds two tiny actual bodies with unused real argument homes documented. Real A6/A7/A8 context groups remain honest nonmatches with empty claims; additional context matches do not earn duplicate coverage.


## Proven shared-TU flag reconciliation and real entity helpers (2026-10-01)

A shared pin can be reconciled when every already accepted body in that segment independently remains strict/raw0 under the new exact flags, its normalized lock hash is unchanged, and the full-ROM baseline passes. C11 verifies existing dll_get_priority and osPfsReAllocate at O1 on A/C/D; both eight-word bodies and original provenance remain intact. This enables dll_insert/dll_update and historical StopThread osPfsChecker_full without arbitrary override. Update evidence first, regenerate opt_overrides.mk, explicitly sync and force the baseline build, then promote. Three other PFS checker/VI initialization functions also pass ROM gates.

The timer-head extern now uses canonical OSTimer*, retaining the old __OSTimerNode overlay definition. The only accepted caller osSetTimer already casts to OSTimer*;63 existing promoted C-TU text streams remain exact before/after. Natural canonical timer types reproduce O1 code where casting the split-word overlay in each new expression does not.

A10 authentic entity/message module closes results_screen_update, leaderboard_update and camera_clip_planes (476 bytes). Keep the actual allocator out of line, remove the historical synthetic wrapper, and retain the actual empty BF01C's pointer formal with the documented code-free if(c) guard. Camera's real lower clamp stores zero directly; its upper branch carries the clamped result. Allocator/receiver remain18/42 and27/40 differing words, honestly unclaimed. No stand-in context or extra claimed coverage.


## PFS/EPi acceptance and storage ownership boundary (2026-10-01)

C12 five canonical PFS/EPi bodies add4376 ROM-verified bytes. Historical osPfsReadWriteFile is SDK allocator with seven arguments, while osPfsGetFileSize is SDK read/write body. Independent current-header new/accepted caller proofs plus forced full-ROM baseline confirm the corrected declaration. Keep PI macros local to lib_e9a0 and before its promotion slots: placing them after a future inserted body caused a compile failure and clean rollback; the same candidate passes after moving the macros.

Actual steering/traction callers reproduce vector_diff_process's hidden origin register t0 without a dummy formal. Claim only its80 bytes; both reconstructed callers remain different. Real physics closure repairs improve source semantics but not yet the allocation; empty claims remain honest.

New source-built local tables/BSS are not proven by an isolated raw-zero object. The PI device-manager switch table is inside monolithic data at retail0x8002D860; replace that actual slot, preserving the rest, rather than appending duplicate rodata. Full VI BSS layout derives every real object through the local retrace counter at0x80036700, but a standalone localcounter .bss at0 does not establish binding. Restore genuine complete ownership and typed SDK thread layout behind the unchanged full-ROM gate; do not invent individual symbol assignments.


## Seven true output pointers close a small IPA helper (2026-10-01)

A16 func_800B6748 is a16-word initializer with seven genuinely consumed output pointers: RGBA, float, two signed halfwords and three signed bytes. Recover every actual caller binding, rather than accepting the decompiler's four-formal prototype. Three real callers and their corrected byte-stream/float-bit operations give strict MATCH with no invented guard, unused formal or broad ancestor expansion. Caller contexts remain unclaimed nonmatches. Independently rescore the frozen source, image-splice only the helper, and require the full source-built ROM gate.
