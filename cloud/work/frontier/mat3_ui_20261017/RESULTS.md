# MAT3-informed UI lane, checkpoint 1

Base: `58425072b49e95f0fbe5c4c239988fb4885f7c5c` (current master, 2026-10-17).

## New result

`func_800DA0BC`: complete 184-byte / 46-word **canonical MATCH** in the real `frontier_optionscr_buttons` group. Previously provisional w14ob body required a stand-in; this packet removes that stand-in and compiles it with its now-locked genuine caller `func_800DB1E0`. Native direct-call census contains only that real parent, two call sites. Parent and teardown preserve native private-ABI behavior through real whole-program context.

Existing members `func_800DB1E0` (1,400 B) and `func_800DA2C0` (2,316 B) are preserved, not new credit. `buttons.c` is byte-identical to the accepted source. The old group name, both members and their claims remain, adding teardown. No locked source or unit override was edited. Retained source-line shaping in teardown is disclosed.

## Proof

- Fresh canonical compiler baselines: sound_handles_clear MATCH; resource_slot_clear all three members MATCH.
- Canonical group, without `--claims`: DB1E0 MATCH, DA0BC MATCH; DA2C0 has zero differing/excess words and its inherited one-base own-rodata NOT VERIFIED diagnostic. No uncertainty waiver.
- `blob_group.verify_own_data` with authenticated opaque runs independently verifies every member, including DA2C0's independent read-only windows and two BSS placements. BSS has address evidence only, not bytes stored in the image.
- Original complete compiled ELF GNU-linked at teardown's native address: **184/184 bytes exact**; STT_FUNC size 184, zero extra words, resolved symbols.
- Wrong sentinel source control rejected at 6/46 words. Corrupted split-owned literal rejected.
- Normal and optimized-Python replay pass. Receipt pins source, selected target bodies, addresses and caller census, not shared tools or mutable locks.

Reproduce from repository root with stock IDO 5.3 and MIPS binutils in the environment:

    python3 cloud/work/frontier/mat3_ui_20261017/verify.py
    python3 -O cloud/work/frontier/mat3_ui_20261017/verify.py
    python3 tools/cloud/score.py group cloud/work/frontier/mat3_ui_20261017/groups/frontier_optionscr_buttons

Last command exits 1 solely on the inherited DA2C0 one-base refusal; the packet's read-only independent-regions proof resolves it. This is not an image/splice/ROM build. New verified source: **184 B game**; new accepted-image/ROM bytes: **0**. Independent review and coordinator integration remain required. Publication is draft-only; the independent checker owns acceptance and merging.

A focused test replays the proof, including both negative controls. Missing IDO follows the shared toolchain skip policy; REQUIRE_TOOLCHAIN=1 makes an absent tool a failure.
