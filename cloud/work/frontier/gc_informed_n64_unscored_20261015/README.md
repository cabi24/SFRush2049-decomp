# GC-informed N64 candidates — unscored research handoff

**Fourteen open N64 targets, 21,388 native bytes of opportunity. Claims: `[]`.
New matched / installable / accepted-image / ROM-verified bytes: 0.** This draft
packages actual source experiments and donor-backed plans for coordinator review.
No IDO compiler or scorer ran in the collection/publication environment. Historical
residuals below are attributed observations, never scores for the new proposals.

Base: [`c226123281724be9459ffa5d8ea41850bede3187`](https://github.com/cabi24/SFRush2049-decomp/commit/c226123281724be9459ffa5d8ea41850bede3187),
freshly rechecked before publication. Current lock records contain none of these
fourteen targets. Population totals remain separate: **game blob 12,996 B**;
**runtime image B 8,392 B**. Do not confuse this with the separate GC/MAT3 work.

## Coordinator inventory

Confidence is very high for the routine identities from native control flow,
types, constants and callers. It does not imply identical historical source or
matching compiled code. Complete target body hashes, native paths, source hashes,
provenance and group recipes are in [packet.json](packet.json). The N64 native
target is the authority whenever GC differs.

| N64 target / address | Population / bytes | GC donor / original address / bytes | Available C and current evidence |
|---|---:|---|---|
| `func_80090308` / `80090308` | game / 1,128 | `StartNewBlast` / `80057998` / 816 | [Inherited natural draft](blast/effects.c). Historical **different device-based w26c source**: 202/282 words differ. Do not assign that score to this file. |
| `entity_physics_update` / `800908A0` | game / 712 | `AnimateBlast` / `800587C0` / 428 | [Inherited natural draft](blast/effects.c), historical 21/178; new group unscored. |
| `save_write_data` / `800AF06C` | game / 1,200 | `StartBlast` / `800585E4` / 476 | [New no-array experiment](blast/start_blast.c), [original baseline](blast/baseline_start_blast.c). Baseline 149/300 in its earlier unit; candidate score unknown. |
| `camera_look_at_point` / `800BF45C` | game / 796 | `_PlayMoveSound` / `8004B2C0` / 632 | [Existing upstream draft in real target-path group](https://github.com/cabi24/SFRush2049-decomp/blob/c226123281724be9459ffa5d8ea41850bede3187/src/blob/groups/camera_aspect_ratio/group.c); historical 38/199. No new C here. |
| `func_800B9740` / `800B9740` | game / 408 | `CalcResPathBounds` / `800331B4` / 368 | [Existing w14r draft](https://github.com/cabi24/SFRush2049-decomp/blob/c226123281724be9459ffa5d8ea41850bede3187/cloud/work/frontier/w14r/func_800B9740/best.c); historical strict 83/102. No new C here. |
| `audio_mixer_main` / `800BA7C4` | game / 732 | `SyncCheckpointsToPaths` / `80031B10` / 312 + inline `FindPathDistances` / `800325BC` / 228 | [New unsigned successor experiment](routes/unsigned_above.c), seven distinct C files / eight group configurations. Historical w6b 31/183; all new variants unscored. |
| `audio_effect_apply` / `800BAAA0` | game / 696 | `ReadPathData` / `8003220C` / 944 | [Existing w17h caller draft](https://github.com/cabi24/SFRush2049-decomp/blob/c226123281724be9459ffa5d8ea41850bede3187/cloud/work/frontier/w17h/groups/physics_response/ctx.c); no reliable fresh score. No new C here. |
| `entity_iterate` / `800C69C0` | game / 224 | `CameraCollision` / `80045D5C` / 200 | [New named-surface-pointer candidate](entity/candidate.c), [original unscored seed](entity/original-seed.c); neither compiled. |
| `camera_play_script` / `800C5644` | game / 3,520 | `CollideCarBodyAgainstSurface` / `800472A0` / 2,280 | [Existing w18d draft](https://github.com/cabi24/SFRush2049-decomp/blob/c226123281724be9459ffa5d8ea41850bede3187/cloud/work/frontier/w18d/camera_play_script/best.c); 27 aligned rows, but recorded strict unit **793/880**, own-rodata failure, candidate only 879 words. No new C here. |
| `input_deadzone_apply` / `800ADD58` | game / 3,580 | `ShotToWorldCollision` / `80045E24` / 3,016 | [Existing w12a draft](https://github.com/cabi24/SFRush2049-decomp/blob/c226123281724be9459ffa5d8ea41850bede3187/cloud/work/frontier/w12a/ida/best.c); historical 122/895. No new C here. |
| `func_8038AA14` / `8038AA14` | B / 120 | `_RemoveGattMFlash` / `8005C690` / 120 | Research only: native ABI / layout plan, no N64 implementation supplied. |
| `func_8038A95C` / `8038A95C` | B / 184 | `_RemoveCannMFlash` / `8005C708` / 184 | Research only: native private `s4` input, no N64 implementation supplied. |
| `func_8038C910` / `8038C910` | B / 276 | `StartWeapon` / `8005C7C0` / 244 | [New embedded-visual candidate](c910/candidate.c), [prior draft](c910/prior-candidate.c), [types](c910/weapons_types.h). Historical prior draft 13/69; new candidate unscored. **Astra owner handoff.** |
| `func_8038AA8C` / `8038AA8C` | B / 7,812 | `AnimateWeapon` / `8005C8B4` / 5,284 | Research only: native-led reconstruction plan, no N64 implementation supplied. |

That is **4 targets with new source experiments**, **2 with inherited packaged
drafts**, **5 with verified links to existing upstream drafts**, and **3 with
research only**. There are no fourteen completed implementations. Baseline source
comments are retained as historical evidence; any old “CLAIMED”/“MATCH” wording
about a provisional helper is subordinate to this packet's empty claims.

## Ownership and use

[`cloud/work/CLAIMS.md`](https://github.com/cabi24/SFRush2049-decomp/blob/c226123281724be9459ffa5d8ea41850bede3187/cloud/work/CLAIMS.md)
reserves C910 for **Astra**, with the prior [draft PR #364](https://github.com/cabi24/SFRush2049-decomp/pull/364).
The new C910 source is research for that claim owner. It does not take over the
claim or supersede the existing packet. Recheck ownership before any work or
coordinator scoring; authoritative claims are unchanged by this PR.

Read [RESEARCH.md](RESEARCH.md) for the evidence, type/ABI constraints and next
levers; [TEST_MATRIX.md](TEST_MATRIX.md) gives setup, controls, exact commands,
acceptance gates and rollback. `materialize.py` creates new scratch groups with
empty claims using the checked-in C and hash-checked genuine upstream context.
It never installs these sources. `verify.py` checks the handoff's integrity and
bounded source/layout controls without a compiler. The native-only C910 replay
requires a private full checkout and reads its protected targets directly.

The original source patches were applied in fresh private directories and their
results compared byte-for-byte with the candidate files. The seventeen-entry
blast ZIP was inspected and compared with its unpacked tree. The ZIP, private
claim copy, full recovered GC source/ELF, native listings, opaque image data and
compiled objects are excluded. Only scoped N64 C, research declarations and
replay code are published; source packet hashes record collection provenance.

## Environment and verification boundary

The selected environment is Windows. Earlier lane probes returned
`Wsl/E_ACCESSDENIED`; publication did not retry WSL, change security settings or
attempt another route to it. Ordinary Git transport failed DNS resolution, so
the authorized GitHub connector was used for repository reads and publication.
No supported executable IDO runtime, complete matching checkout or unit/image
builder prerequisites were available here. Official setup needs x86-64 Linux.

Fresh source/hash/patch/layout checks and native-fixture tests **are not**
compiler scoring, candidate/native equivalence, relocation verification,
compiled nonregression, image identity or ROM verification. All such coordinator
gates remain open. Keep this PR draft and leave acceptance and merging to the
independent checker.
