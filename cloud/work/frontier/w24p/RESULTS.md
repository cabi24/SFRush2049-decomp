# w24p: integration of Astra's open PRs (#163, #311–#334) (2026-10-12)

Branch `w24p` from `wave24` = origin/master `b37643ee`. Not pushed.

## Outcome

| PR | What | Verdict | Bytes | Gate |
|---|---|---|---:|---|
| #334 | B `func_8038D3A4` opponent damage, one-member group + owned 0.2f literal | LAND | 244 | `group spliced ['func_8038D3A4']; image gate byte-identical` |
| #333 | A `func_8039BB80` in superseding `dot_runtime_a_text_helpers_20261006` (all 11 locked members + BB80, 12 claims) | LAND | 712 | `group spliced [... 12 ...]; image gate byte-identical` |
| #332 | A `func_803A4340` menu callback (single) | LAND | 428 | `spliced 1, refused 0, locked 55; image gate byte-identical` |
| #331 | A `func_803990D0` menu-state root, superseding `dot_runtime_a_menu_state_20261006` (3 locked + root, 4 claims) | LAND | 692 | `group spliced ['func_80398B40', 'func_80398C3C', 'func_80398E70', 'func_803990D0']; image gate byte-identical` |
| #330 | A `803A6B50` research | DECLINE (nothing new) | 0 | head `f2e3fb52` = what w22p imported; packet/test byte-identical on master |
| #325 | recomp Phase 2 wrapper generator | ASTRA-ACTION | 0 | no commits since w22p's comment; accessors on master unchanged |
| #323 | CLAIMS reservations | DECLINE | 0 | every row superseded by the hand-reconciled rows in CLAIMS.md |
| #322 | game `func_800D816C` (699/699 in group) + caller `func_800D91A0` 166/965 | IMPORT-ONLY | 0 | `blob_unit score`: FAIL 691/699 kept; 570/699 internal with caller (`d816c/*.log`) |
| #317 | game D816C two-word residual | DECLINE (superseded by #322) | 0 | — |
| #314 | static voice-unblock candidate | DECLINE (already landed via TU-spelled copy) | 0 | — |
| #313 | research handoff doc (#305–#308 archive) | DECLINE | 0 | — |
| #312 | cloud-packet skill doc | DECLINE | 0 | — |
| #311 | overlay-A two research waves | DECLINE (superseded) | 0 | — |
| #163 | overnight portability fixes | DECLINE (superseded) | 0 | — |

Overlay A 53/193 (23,364 B) → **56/193 (25,196 B, 12.98%)**; overlay B 10/49 (3,364 B) → **11/49 (3,608 B, 8.22%)**.
Total +2,076 B (A +1,832, B +244). Static and game unchanged (600/668 130,500 B; 985/1216 325,760 B).

Gates after all splices (`ovl_rom.log`): `ovl_layout build` (A 56 bodies, B 11, both byte-identical),
`ovl_splice check` ("all 67 runtime-image entries intact"), `ovl_rom rom`: both streams `== ROM`, "ROM build ... SHA-1
EXACT", "ROM matches!", `MAKE=0 TEST=0`.

## Notes per PR

- #331/#333: same-name supersessions; no migration needed (the locked groups' members are subsets of the new ones).
  #333 claims all 12 members itself, `--claims all` not needed. B908/B988 formal order changed (bodies identical).
- #334: the packet dir itself is the group (`group.json` names it); installed as `src/ovl_b/groups/dot_runtime_b_damage_20261012`.
- #322: `renamed_game_source()` leaves all seven files unchanged. `blob_unit --tag w24p score func_800D816C --with
  target.c --neighbours`: 691/699 differ (kept: standard prologue; retail D816C has none, umerge inlines D8154).
  With `--with caller.c --internal func_800D816C --block func_800D8154`: D816C 570/699, D91A0 740/965, and the
  unit inlines locked `func_800DA0B4` into D91A0 (1 locked body differs). D816C is an IPA-internal callee of D91A0 and can only
  land together with a matching D91A0 (and the packet's internal/keep split vs. the unit's kept singles
  DA0B4/D8078/D8154/render_replay_ui). Packet + test imported as research.
- #317: its 2/699 D816C is closed by #322's 0/699 in the same group; nothing else to keep.
- #311: every near-miss it lists landed since (#315, #316, #318, #321, #324); 803A7180 research is on master in
  `dot_runtime_a_renderer_frame_20261011`/`..._renderer_residual_20261011`; the four packets already on master came via those PRs.
- #163: 448 files from 2026-10-06; its portability edits target packets later re-imported/reworked; the game
  groups (audio_priority_find, func_800B66B0, func_800A3724) are IPA-internal group-only matches (agentA/w11h/w14d);
  voice_unblock landed via #314's route; flag-line edits to locked cloud/matches singles are not needed.

## Tests

- rocky, `REQUIRE_TOOLCHAIN=1` on the five new packet tests (#331–#334, #322): 5 passed.
- Pi without IDO: the same five skip cleanly ("IDO missing").
- Pi `make test-fast JOBS=4`: conveyor 1541 passed, 795 skipped; cloud 770 passed, 155 skipped; exit 0.
- rocky `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16`: conveyor 2285 passed, 51 skipped; cloud 924 passed, 1 skipped;
  exit 0.

## Setup notes

Fresh worktree: `cp baserom.us.z64`, copy `build/game_code.bin` + `build/blob/obj`, `ovl_splice rebuild` A/B,
`blob_layout derive`, `blob_rom produce` (stream byte-identical; it rewrote `asm/us/blob/{SHA256SUMS,symbols.json}` and
`src/blob/blob.ld`, reverted, not committed). `OVL_BUILDER_DIR=/tmp/ovlsplice-w24p`. Rocky clone `~/rush2049/scratch/w24p`.
