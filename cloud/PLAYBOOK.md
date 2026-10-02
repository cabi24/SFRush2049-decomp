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


## Genuine readonly ownership and pointer-call closures (2026-10-01)

The PI device manager's real switch table occupies32 bytes of the existing data container. Split the composed source-built container around that slot and insert the owning TU's compiler section with fixed-address/size assertions. The baseline companion reads only the original bounded slot; promotion removes that companion atomically with the function pragma and rollback restores both. Preserve word/float/double parser sizing when adding incbin support. Build helpers must run from the builder's minimal synchronized package; validate reserved game-blob constants against Makefile/blob_rom without importing the full pipeline at build time.

A18 two mode wrappers close immediately when the real helper consumes its true hidden mode and uses signed byte fields, unsigned LCG arithmetic and actual float parameters. Claim only the wrappers; different helper bodies remain context. A19 three range/block callers also close immediately with actual32-bit pointers and unsigned saturating byte counters. Independently matching the existing range lookup in the same group proves its useful preservation contract, rather than assuming acceptance elsewhere suffices. Neither packet needs a synthetic wrapper or allocation sweep.

### Coordinator ninth milestone: complete VI ownership and ordinary leaf tail

The VI creator/worker need their complete actual SDK BSS layout together. Canonical strict0 with unlinked raw9/raw5 is valid relocation evidence, but accept only after full196-word original-address equality and startup-zeroing proof, then atomic source/header/NOLOAD ownership and source-built full-ROM acceptance. Pin complete source/context; reject single-function promotion for registered module owners. Correct FP pairs are u64 and thread size0x1B0. Ordinary proof staging drops local include paths/full module context, so the reviewed B19 recorder stores independently executed immutable strict evidence without weakening the scorer or falsely claiming unlinked raw0.

Six further tiny ordinary game leaves match faithful O2 full TUs immediately; exact float intrinsic prototype, signed-byte loads and true record field widths matter more than mutator sweeps. Prefix required literal flags line before the final independent rescore. OSTime low-word and task pointer aliases can be normalized only after exact SDK/live type/layout and all authoritative addresses agree; fresh target round-trip and unmasked full-link proof remain mandatory. Private linker fill that preserves original TU extent is not C coverage: osDpWait has32 logical body bytes and96 padding bytes.

### More ordinary SDK and real game leaves (2026-10-01)

C15 promotes four unchanged canonical sources after precise guarded target aliases, ordinary score0 locks, passthrough conversion/local context baseline and individual full-ROM gates. Keep the genuine SP-task PHYS_TO_K1 macro local before inserted slots; preserving the shared IO_READ avoids collateral semantics changes. Object alignment words never add coverage. A23/A24 add12 faithful ordinary leaves with real widths/strides/argument homes; A553C needs separate physical lines for actual-store scheduling, not synthetic register pressure.

Root review caught a proposed text-boundary helper importing full layout/lock packages at make time. The shared builder runs a minimal synchronized package, so standalone helpers and tests of their authority constants/body normalization must precede production adoption. This is the same dependency class caught during C13 acceptance; private ROM models do not validate actual shared-builder packaging.

### Actual heap-release closure and honest padding credit (2026-10-01)

A25 repairs the release routine's genuine address/tag signature and real successor alias behavior; two overwritten/dropped ABI lanes are not a reason to invent dummy logical formals. Real callers preserve unsaved s0/s1 when compiled together. All six new bodies and all five accepted real contexts independently strict MATCH in the same full module; source-built image and ROM prove placement. A genuine entry pointer view corrects stat's local home without dummy storage.

A32-byte SDK yield body can replace a128-byte assembly slot only when original96-byte zero tail remains. Preserve the explicit original256-byte TU endpoint with linker fill and all original address/size assertions; credit32Cbytes, never96fillbytes. Hash/count endpoint authority, full ROM/source/body validation, regeneration/removal/revert package coverage and standalone make-time dependencies are necessary. Current shared-builder baseline and standard promotion both ROM exact; no synthetic C padding function.

### Matching game source passes10 percent (2026-10-01)

A26 signed table-index wrap and A27 true pool unlink add300 actual matching bytes after independent strict, image and source-built full-ROM gates. Game C is64,796/647,072 bytes (10.0137%). A27 fixes the actual successor carrier and preserves its load order under aliasing, with no dummy runtime work. A26's different switch body remains unclaimed because local-table relocations are unverified; strict0 alone is insufficient. A28 authentic pool-init group remains an empty-claim nonmatch and its accepted helper earns no duplicate credit.

### Faithful signed-half absolute branch (2026-10-01)

A29's E5C9C retains true signed-half field+2000 and32-bit magnitude, including safe32768 for the minimum halfword. Writing its real nonnegative branch first reproduces retail condition/delay-slot scheduling with no added operation. All genuine float thresholds/fields remain unchanged. Its200bytes pass independent strict/image/full-ROM gates; expanded E7A98 heap context remains an honest empty-claim nonmatch despite eleven exact accepted contexts.

### Guarded counter closure and typed SDK targets (2026-10-01)

A30 preserves the real owner guard at offset16 and byte saturation at offset22. Genuine unchanged accepted context restores the retail O3 calling/register behavior; all eleven contexts independently MATCH. Only its184-byte claim is counted. SDK target aliases must be guarded by actual field widths, source declarations and original address relationships; refreshed relocatable targets produce raw equality without scorer masks. Zero scores for timer/initialization candidates remain uncredited until their complete source/storage transaction passes the original full-ROM gate.

### Existing C modules own initialized pointers and BSS jointly (2026-10-01)

Timer activation replaces the complete existing O1 TU and installs its actual pointer/data and BSS together, retaining five accepted body-lock dictionaries and its C SPLAT owner. Immutable complete-source/context/current-target/linked/startup proofs precede force-all-static original-ROM gates. Only new dll_init is credited. A standard coordinator callback checks source-built ROM, pytest and locks, and publishes the new member inside the rollback scope. Ignored ROM/assets remain synchronization dependencies and are filtered from Git. ROM-independent readonly guards still pin source, headers, original assembly and revert prerequisites. Timer-specific flags/counts must not silently authorize a different ownership module.

### SDK polynomial math uses original constants and genuine intrinsics (2026-10-01)

C22 reconstructs canonical SDK sinf/cosf with exact original named readonly arrays/scalars. Historical labels can be misleading: prove values/types from SDK initializers and original hashes rather than renaming symbols or creating replacement constants. Keep genuine ROUND/ABS macros and constant declarations local to the new TU so accepted shared compiler/header context stays pinned. Verify ordinary source and full actual recipe separately, then use normal pool locks and original-ROM promotion. A36's reciprocal square root uses the real IDO sqrtf intrinsic declaration; an inline assembly or dummy helper is unnecessary.

### Initialization ownership and genuine float ABI repairs (2026-10-01)

B26 introduces a separately reviewed exact owner profile, preserving timer's flags and complete source family. Distinguish actual tracked compile dependencies from ignored SDK target-normalization provenance: accepted source/context/current-target hashes and original ROM gates retain authority while readonly CI needs no reference checkout. Initializer credit is only680 C bytes; original32-byte initialized data and16-byte BSS belong to the same atomic transaction.

B27 repairs an actual scalar-float callee return contract rather than converting an assumed integer return. Prove formal types from original caller register lanes and callee homing; a truly unused third float observed in the actual call is legitimate, while an invented pressure formal remains forbidden. Typed table element scaling and natural product/value flow recover the other exact registered-head body.

### Intrinsic declarations and true vector dataflow yield ordinary matches (2026-10-01)

C24/B28 recover original hardware sqrt.s/abs.s via genuine IDO intrinsic declarations after proving SDK float signatures. Correcting implicit-int assumptions avoids invented wrapper work. Typed two/three-float arrays, a common return and actual component load order match the physical source behavior; prior distant vector seeds can become exact when this real missing contract is fixed. These three independent ordinary bodies add372 bytes with no context or storage credit.

### Genuine vector context preserves accepted helper identity (2026-10-01)

C24's motion caller uses its real one-pointer float-return length helper. An incidental caller register value is not evidence for an additional formal. Keep the already accepted helper definition identical, independently score both claim and context, and credit only the new caller. A real three-component vector local and actual pointer reuse can recover allocation without dummy stack pressure. Preserve original global access order where aliasing is observable. The companion ordinary vector leaf needs exact real operand grouping and signed product flow.

### Real cursor scope recovers byte-comparison source (2026-10-01)

A45 recovers the actual three-input byte-comparison loop: generic memory converted to an unsigned-byte cursor inside the existing nonzero-count guard, postincrement comparison and genuine predecrement mismatch difference. Natural cursor scope restores the original local register and schedule; IDO emits the original unroll/remainder structure without manually inventing a group or extra work. C25 proves the nearby reciprocal-length helper's one-pointer float-return ABI and original threshold; independent duplicate discovery earns only one credit.

### Tail recursion and actual unsigned types recover fresh ordinary bodies (2026-10-01)

A48's real signed-half traversal inputs are narrowed again at the recursive source call. IDO removes that tail call into the retail loop; a generic while rewrite omitted four actual argument-conversion instructions. One actual assignment-line boundary recovers halfword store scheduling. A49 matches immediately after describing only actual object fields and unsigned counter/mask globals with the genuine external remove signature. An old database zero is a selection hint, never acceptance proof. C26's render helper uses real (u16)(u32) float conversion and original named flag/global stores, producing IDO's complete original conversion sequence.

### Genuine pointer mappings and source control flow recover complete list/audio bodies (2026-10-01)

D1/D2 use complete real list/link structure rather than opaque field macros: indirect and direct modes assign the actual header/object pointers through explicit branches. Removal's genuine head store/read and reused reconnect pointer recover retail lanes; insertion initializes cursors only after its real null guard and explicitly selects both node and position headers. Preserve original reloads and scan behavior, including traversal after predecessor replacement, rather than inventing a break. These complete ordinary callees match without caller stand-ins. C28 explicitly advances the real voice pointer on disabled/retained nodes and preserves the actual next pointer before genuine removal calls; its unsigned volume conversion remains original compiler work. C27's scalar audio wrapper likewise needs actual u16/u8 widths and the original127 multiplier.

### Complete numeric/affine source recovers large ordinary bodies (2026-10-01)

C29's first canonical numeric formatter compile matches all 204 words using actual unsigned float conversions, numeric byte digits and return lengths. C30 specializes the real fixed-matrix packing algorithm to the actual three input rows and constant affine row; real parallel output offsets and explicit final row stores recover all 73 words without padding. Raw source checksum guards include whitespace: even token-preserving cleanup needs fresh strict proof and image publication to refresh its source lock. A genuine five-word IPA allocator residual is a lead while its real context remains nonmatching, and a failed source-line boundary control should be recorded once rather than swept.

### True resource layouts and original float store order (2026-10-01)

A57 preserves the real box predicate, including its asymmetric y test, and consumes only the actual shifted index return. A60 describes the complete player/reference/car/data chain, actual selector ranges and original nullable path; the result is the original full bit mask rather than a normalized boolean. C34's six real halfword counts partition contiguous resource sections with their actual strides. B40 uses genuine halfword masks and initializes the real returned input value before the divisor load. C35 preserves all six ordinary scalar trigonometric calls and nine actual matrix products/stores; original final-store order fixes scheduling without new arithmetic. Before selecting a head, check the canonical target section and accepted address intervals: an alias inside an already accepted body is not a new function.

### Indexed records preserve original unsigned conversion loops (2026-10-01)

C36's 72-byte viewport records contain a true bounds pointer and four float fields. Preserve actual unsigned-half to unsigned-word to float conversions and the original unsigned result conversion, including FCSR behavior. A genuine indexed record expression recovers the original induction register and tail schedule; replacing it with a pointer cursor leaves eleven real differences. The complete 808-byte routine matches without adding operations or changing the record/count globals.

### Genuine cursors and increment-before-store indexing (2026-10-01)

B41's encoded string copier keeps the actual destination return input intact through separate real input/output cursors. Preserve the 255 prefix, two-byte zero terminator and original ordinary byte-copy path. The state initializer's existing inner index increment precedes its actual matrix write; writing matrix[i][j-1] after increment reproduces retail pointer/store scheduling without added operations. The display-list getter preserves its original no-defined-result end opcode path. Recheck full archived packets immediately before reserving a target so new negative reports cannot be missed by an older shortlist.

### Forwarded arguments must retain the actual callee contract (2026-10-01)

B42's camera wrapper visibly forwards ten real incoming arguments; the genuine callee reads the first as a three-float vector pointer. Correcting an opaque word carrier to that proven pointer retains exact ordinary output and improves the reconstructed interface. Do not infer semantics for callee-unused forwarded carriers, fabricate extra arguments, or erase the original enable/index guards. Final typed source needs a fresh canonical replay even when an earlier carrier variant already matched.

### Ordinary O3 and real consumed expression sequencing (2026-10-01)

B43's complete list removal matches as an ordinary O3 source; the enclosing region default O2 is not an authoritative per-function recipe. Prove the exact compiler flags independently and pass them explicitly into publication. Its final two differences were equality operand ordering: sequence the genuine pointer advance before its consumed comparison, retaining every original operation, guard and field access. Correct signed HI/LO address arithmetic must precede source controls. A65/A67's single O3 controls are unchanged nonmatches, so this setting is a verified possibility rather than a blanket explanation.

### Actual multidimensional indexing recovers compiler unrolling (2026-10-01)

D6's complete initializer describes the observed12-by-3-by-5 signed-word table directly. IDO derives the original peeled and unrolled stores on the first native compile; the pointer-loop alternative leaves23 differences. Preserve the real call and subsequent name/age clears, without hand-expanded extra work. Shared data views remain local declarations rather than new owned storage. A stale automated candidate residual is not evidence that a complete native rewrite is blocked.

### Real captured values and consumed update order preserve larger routines (2026-10-02)

D8's actual initializer captures its one-time diagonal constant in a consumed local before writing three matrix elements; chained stores introduce an extra reload through real aliasing. Original vector/field store order completes the440-byte match. C44's624-byte nearest-point search needs original signed-half narrowing, genuine point-formal/counter reuse, branch-local coordinate loads and consumed next-point update order. Capture real loads and preserve data flow rather than adding pressure or dummy runtime work.

### True dimensional expressions and recursive source recover complete routines (2026-10-02)

A73's actual five-child rectangle builder matches at O3 when width/height differences are expressed at their real stores and the final call loop uses native child indexing. IDO hoists those consumed invariant differences naturally; an obsolete cursor declaration was removed and the resulting source independently reproved before publication. B51's forward/backward offset utilities preserve real tail-recursive argument updates and output pointer accesses. Native recursion reproduces the original8-byte frame and conversion work; one genuine sum operand-order correction closes its last instruction without extra work.

### Actual publication order and ordinary O3 preserve complete resource routines (2026-10-02)

B55's true halfword resource updater preserves signed selector ranges, actual record strides and the original asymmetric nullable paths; changing only the explicit recipe from O2 to O3 gives a complete match. A77's five-child parent allocator consumes the byte1 attribute before publishing its active flag, allowing IDO to recover the original load/store schedule without any new operation. An observed extra zero ABI carrier to a callee that ignores it remains through an old-style declaration rather than an invented unused formal. Exact SDK matches outside the registered static code map remain separate leads until real ownership and coverage denominators are supported.

### Genuine accumulator update order and reproducible coordinator replay (2026-10-02)

B59's full resource statistics loop matches when the actual float total update precedes its consumed halfword count increment. Preserve the original reloads, record widths and both real accumulators; no artificial pressure or operation is needed. tools/cloud/review_single.py binds exact first-line flags and canonical extent to fresh full comparison and source/manifest hashes. It rejects incomplete proof and replaces stale success output on failure. Object proof remains separate from independent source review and image/full-ROM publication gates. Read the live reservation map before creating a new native body to catch overlapping work whose source file has not yet appeared.

### Real shared offset captures recover entry scheduling (2026-10-02)

A79's complete dual-statistics loop has a genuine selector*64 byte offset consumed by both resource and global records. Capturing it per player before the original resource validation lets IDO place the original entry calculations and peel the true ten-counter update loop. Full600-byte equality follows without a pressure expression, added operation or stack padding. Retain signed live-counter loads, unsigned accumulators and the actual signed division before converting its consumed result.

### Genuine helper-only groups resolve original switch-table ownership (2026-10-02)

A80's real byte-hash helper compiles all45 code words, but two own .rodata references prevent a standalone bare MATCH. Remove the still-nonmatching caller from the proposed claim and use the existing genuine helper-only group pipeline. Fresh private compilation, full canonical image relocation and independent relocation of every original table entry prove both code and24 consumed table bytes exactly. Keep all gates and no unknown references; do not count alignment or table/data bytes as matched code. blob_group builder_script captures its workdir default at definition time, so changing BUILDER_TMP alone does not redirect its script. Private score.compile_group uses an isolated temporary directory and its resulting object can be fed into the unchanged canonical group resolver for independent proof.

A26's previously frozen0/37 switch lead also passes the genuine helper-only group route: all11 original jump-table entries are independently equal and every full body word verifies. The old C body stays untouched; new explicit O3 group metadata and fresh gates supply placement proof. B67's complete four-player input updater matches on its first native O3 compile; preserve the actual word/axes tables and both disabling guards rather than assuming the old mismatching group seed describes the source. An actual hardware sqrt.s requires the documented intrinsic declaration: A75's missing contract was corrected in a separate follow-up without rewriting its frozen original, but remaining nonzero results earn no credit.

### Separate consumed cursors and real checksum caller context (2026-10-02)

B75's complete list removal uses separate consumed cursors for its primary and pending domains. That recovers the original stack slot and closes two words without changing the actual list traversals, player handle clearing or helper calls. A88/A89's checksum callers preserve their actual resource records and the complete accepted hash helper. Compiling that real helper internally reproduces the original live registers across the calls; both callers match on their first native group compile. Independently prove all helper body words and original switch entries while leaving its existing source lock intact. No repeated helper credit or table/alignment byte credit is added.

### Native switch tables can occupy separate original image windows (2026-10-02)

A86/A87's full native callers and genuine hash helper pack two tables into one object section, while the cartridge places each table separately. Canonical blob_group first preserves the existing contiguous-section proof. Its narrow fallback derives original table windows from complete actual HI16/LO16 references, requires a complete aligned R_MIPS_32 entry partition into genuine covered text, and proves every disjoint original window byte. Unknown entries, conflicting or missing references, overlaps, mixed data and out-of-window targets refuse. Fresh independent D objects, all original entries and four real table-corruption refusal drills verify the change before image/full-ROM acceptance. Member-only reconstruction also verifies context table windows. See cloud/work/table_windows_A86.md and cloud/work/integration_38/ for the scoped tests and acceptance evidence.

B81's complete six-input record allocator matches ordinary O3 on its first native compile. Preserve its observed200-record limit, signed active halfword, unsigned flag/parameter fields, count/max updates and real seven-argument initializer. The compiled zero alignment word contributes no additional code coverage.


### Collaborate on one complete larger routine (2026-10-02)

func_800F56E0 (2,072 bytes /518 instructions) matched through three complementary agents: complete C reconstruction, independent control-flow/callee review, and record-layout/unsigned-conversion review. Confirm the ordinary ABI and full extent before selecting the target. Direct indexed player-record fields let IDO create the original induction pointer and stack homes. Preserve genuine selector read/update order, live loop bounds, sequential float stores, and consumed backward-loop counters. These natural source choices recovered the160-byte frame and original scheduling without invented padding or operations.

The exported generated-symbol target object contained alias-dependent relocations; raw ELF comparison produced false residuals. The protected canonical retail scorer and fully resolved image are authoritative. Publish exact first-line flags, independently recompile the normalized source, then require image/ROM equality and lock checks. Final source: cloud/matches/func_800F56E0.c; evidence: cloud/work/large_func_800F56E0/compiler/. Accepted game coverage **13.02%**; static **46.76%**.
