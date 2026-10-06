# Selector call-site correction and bounded dependency census

**Read-only research. No C candidate, compiler variants, match or coverage gain.**
Native basis: `cd22879d40b3de443cfde047b86e75e159b6cec6`.

## Correction that changes the next assignment

The [old D-lane blocker](../../s20261004/D/notes.md) says `func_800D9058`
needs a discarded `slot_state_setup` return copied into s0. Its sole current
protected call, **0x800D9098**, has **no v0 capture** before the following unlock.
D9058 is **0x800D9058–0x800D91A0, 328 bytes / 82 words**. Do not manufacture
that copy or a result keeper. It remains a private-ABI helper of the large
D91A0 caller; this correction does not make it a standalone matching target.

The same no-capture pattern occurs at six lock/selector/unlock call sites:

| Function | Selector call |
|---|---|
| particle_render | 0x800B8670 |
| func_800D9058 | 0x800D9098 |
| game_results_render | 0x800FF434 |
| func_80106B3C | 0x80106C48 |
| func_8010EA08 | 0x8010EB00 |
| func_8010F218 | 0x8010F954 |

The audit checks each exact address is a selector JAL with adjacent acquire
and release **call labels**, and no simple return capture before/through the
release call's delay slot. It is a syntactic observation, not CFG liveness,
queue-argument identity or proof of original source boundaries. Captures stop
being attributed to the selector if v0 is redefined first.

## Fresh census

Manifest-verified registered game bodies contain **170 direct selector JALs
in 65 functions**. There are 167 adjacent acquire/selector/release call-label
sequences. Of these, 161 capture v0: 123 into s0, 7 into s1, 7 into s4, 2 into
s5, 1 into s7, and 21 stack-word stores. Six do not capture it. Three other
selector sites in world_velocity_integrate/world_trigger_check operate within
wider sequences and are not classified as adjacent wrappers.

Only `credits_scroll` is currently accepted among those 65 callers. The other
64 occupy **74,428 native bytes**. This is dependency surface, not an unlock
or coverage forecast. Counts exclude computed calls, uncovered regions,
static-image callers and overlays; historical counts used other views.

Eight unaccepted callers have no other **unaccepted direct game-body callee**:

| Function | Bytes | Remaining prerequisite |
|---|---:|---|
| object_create | 112 | C104 private-register/context nonmatch |
| func_800D9058 | 328 | Genuine D91A0 parent/private ABI |
| world_trigger_check | 216 | Wider locked-state/private helper context |
| dust_cloud_effect | 600 | A141 frame/allocation residual |
| skid_mark_render | 1,396 | Source audit and runtime-image B calls |
| checkpoint_hit | 580 | B124 frame/allocation residual |
| func_80107EDC | 632 | Old dynamic-difficulty source/context audit |
| func_80109F54 | 1,504 | Registered-head seed, not reviewed full source |

Total **5,368 bytes**. Indirect callbacks, external/static/overlay calls,
storage, literals and caller mismatches remain outside that graph filter.
None of these eight is labeled ready to match. The receipt lists every caller,
call address, selected body hash and additional unaccepted direct game callee.

## Known source boundary; unresolved contract

The [clean selector source](../../dot_menu_options_root_20261005/slot_state_setup.c)
returns the old signed slot byte. It stores the new low byte, but compares the
**full input** with -1 and 0. Resource lookups use the current signed global
byte plus 38/22, reloaded after callbacks. Bank refresh compares the old signed
byte against the full input. Native input arrives in s2; s0/s1/s3 are unsaved
callee scratch, and bank refresh takes its force value privately in t0.

The actual `object_create` wrapper, **0x800B42F0–0x800B4360**, locks, calls the
selector, preserves its return in an actual stack slot, unlocks and returns
it. All ten direct wrapper calls are in playgame_state_change. The wrapper
therefore independently supports preserving the returned value; it does not
prove every adjacent triple was inlined from the same declaration.

The [E-lane notes](../../s20261004/E/notes.md) and
[helper source](../../s20261004/E/src/helper.h) already explain discarded
copies and 24-byte per-instance homes with nested gfx/font wrappers. That is
prior work, not a new mechanism discovered here. The accepted credits_scroll
group uses that hypothesis with unclaimed selector context and an explicitly
frame-inferred text buffer. It is not an original-capacity donor for new callers.

[C104](../../game_C104/REPORT.md) has a complete natural wrapper at 1/28 differing
words: selection is loaded into s1 instead of native s2. Its truthful downstream
context remains nonmatching. The accepted slot_sound group supplies matching
bank refresh, but explicitly contains a non-original dead-switch/empty-read
96288 inline blocker. This audit neither imports, expands nor endorses it as
original source. The unresolved prerequisite is authentic private-register/
callee-summary composition and hook/compiler visibility, not a missing return
value that should be recreated with fake consumers.

Pinned arcade [font.c](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/LIB/font.c)
and [font.h](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/LIB/font.h)
use FONTINFO/SFONTINFO pointer APIs. The reviewed files and a bounded C/H
set/select-font-name search did not supply this N64 two-resource selector.
The reconstructed names gfx_lock/font_set/gfx_unlock are not authenticated
arcade names. This is a negative search result, not proof of absent ancestry;
no external donor checkout is required for the native/source census replay.

## Reproduction and limits

    python3 cloud/work/frontier/dot_slot_contract_audit_20261006/audit.py
    python3 -m pytest -q tests/cloud/test_slot_contract_audit.py

No IDO or GNU toolchain is needed. The unmodified project reader authenticates
current target/symbol manifests on every read. Selected native bodies and
source inputs are strictly hash-bound, and current relevant acceptance states
must reproduce the saved bounds. Historical base metadata is provenance, not
a substitute for current integrity checks. Unrelated files may evolve without
changing the selected evidence. A relevant source/body/status change fails
closed and requires a fresh review before updating the receipt with --write.

Tests reject wrong site addresses, a real captured site falsely labeled no-copy,
wrong saved-site metadata, an actual changed source file, selected native/helper
mutations, changed acceptance status, and warm-cache manifest corruption.
No native words, raw assembly, objects or donor source are published. No host-C,
native behavioral execution, GNU relocation, image, compression, ROM, gameplay
or whole-project test pass is claimed: this packet is a bounded read-only audit.
Stop return-local/volatile/keeper sweeps. Reopen matching only for a specific
genuine source or callee-summary hypothesis, in the correct copy/no-copy class.

Validated locally: **22 packet tests plus 732 existing scorer, integrity, guard
and submission tests (754 total)** passed. The ordinary commit lock check
preserved all **402 static source locks**. Initial sparse-checkout missing-file
failures were resolved by materializing the existing required inputs; no test,
source or gate was weakened. This is selected-suite validation, not a full suite.
