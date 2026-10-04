# Renderer readiness: blocked input packet, no execution assignment

Base: `53d0fba0c5f44e91fe921f722fee7ff63a2dd0fe`. Read-only audit of tracked source and manifest-checked native text; no compilation, private input access, ROM extraction, production edits, or matching claim. No newly testable allocation hypothesis passed preflight.

## Decision and corrected prerequisite

D09's remaining-helper wording is stale. PR9's merged `cloud/work/ipa-groups/codex_gfx_rectangle_b119/group.c` already contains complete source for mode `func_80086A50`, graphics initializer `sound_init`, set-mask `func_800878E0`, palette `func_8008A148`, clear-mask `func_8008705C`, and rectangle `func_8008A46C`. The palette's banked branch already contains corrected `0x0703C000`; the separate full-palette branch intentionally uses `0x073FC000`. Its frozen receipt records rectangle 71/118, palette 101/145, clear-mask 11/45, set-mask 19/74, initializer 70/100; mode is 0/387 with two unverified references. All are historical receipts, not fresh results. PR48/49 add reviewed source/behavioral evidence but do not make the same closure novel. The later round12 closure's parent-reported 75/118 result does not supersede 71/118 as the best historical rectangle receipt.

The actual remaining blockers are (1) authentic original compilation-unit/export-root and caller allocation evidence, (2) complete audited caller source where that context is genuinely required, and (3) exact-candidate local-table verification. Solving (3) alone does not explain t0/t1 versus a2/a3 allocation.

## Exact identities already in repository

`native_identity.json` records scoped function hashes, authoritative extents, and family call addresses, derived from four SHA256SUMS-verified text files. Source definition leads, classifications and hashes are in `caller_definition_inventory.json`.

- `func_80086A50`: 0x80086A50, 1,548 bytes / 387 words; accepted, read-only context, zero repeat credit.
- `func_8008705C`: 0x8008705C, 180 / 45; complete B119 actual one-mask body. Earlier 45/45 group uses stand-ins and is not genuine closure proof.
- `func_800878E0`: 0x800878E0, 296 / 74; complete B119 and PR49 bodies.
- `func_8008A148`: 0x8008A148, 580 / 145; complete B119 four-argument palette/color helper.
- `func_8008A46C`: 0x8008A46C, 472 / 118; complete B119 and PR48 five-argument rectangle.
- `sound_init`: 0x800A4934, 400 / 100, complete B119 and PR49 graphics initialization; historical sound name is misleading.

## One mode table, two reference sites

The two unverified entries `.rodata+0x0 at +0xc` and `.rodata+0x0 at +0x14` are the HI16/LO16 pair at 0x80086A5C / 0x80086A64. They jointly address ONE five-case switch table at `0x80123870`, logical interval `[0x80123870,0x80123884)`, 20 bytes. They are not two independent table placements. The table address and case count are verified from manifest-checked public text; original table contents/order/hash are not present in this packet and were not read from a private image.

`blob_matched.lock.json` already accepts the mode function via `src/blob/groups/func_80086A50/group.json`, source hash `13d34aee0632430906b999b8636ce0256c689fd3f2da6699b58586307a20dbf8`, toolkit `796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5`, image_gate 2026-09-30. `cloud/work/module_campaign_20261002/acceptance/fresh_game_rebuild.json` records its full-body SHA256 `945eedfdd9b852633b98c2b855a03f76b7b611382e2f36076d3aa35a771fd60f`, identical to current canonical text. Latest `cloud/work/pr_integration_20261003/gates.json` records 666 complete bodies, prior 664 locks unchanged, 647072-byte image equality, compressed-stream equality and ROM SHA1 `3f99351d7bb61656614bdb2aa1a90cfe55d1922c`.

This is genuine existing acceptance evidence, but not an exported source-specific table receipt for a changed research closure. The production group explicitly keeps `caller_a` / `caller_b` from stand-in `callers.c`; its accepted bytes do not establish the original renderer TU or the correct keep list for new claims. Do not edit that production group.

Production `tools/conveyor/pipeline/blob_group.py:relocate` / `_local_data_bases` require original image verification, recover paired reference bases, relocate table targets into covered text, and compare bytes. Its normal one-section proof is the applicable starting path. Multi-window support is not evidence that this mode table has two placements. The cloud scorer's allow-unverified mode cannot replace this proof.

## Genuine caller audit, not filename absence

The tracked C definition scan finds:

- `Input_ProcessGameplayPad`, 0x800A04C4, 2720 bytes: only `work/game/physics/Input_ProcessGameplayPad/base.c` empty stub. Native calls rectangle at +0x520 (0x800A09E4), palette at +0x744/+0x794/+0x7cc/+0x814, object renderer at +0x8e4/+0x9c4, and multiple flag helpers. The rectangle call passes color in incoming argument slot +16. Full body must replace the stub before it can supply source context.
- `audio_doppler_calc`, 0x800B6788, 1124 bytes: only `work/game/audio/audio_doppler_calc/base.c` empty stub. Actual native family calls are set-mask +0x48, clear-mask +0x54, palette +0x118, object renderer +0x168/+0x378. Its name is not evidence of an audio-only contract.
- `object_render`, 0x80087A08, 10048 bytes: `src/game/game.c:21540` is a two-argument illustrative object/matrix routine, inconsistent with native eleven-argument textured-rectangle ABI; reject as actual context. `cloud/work/bigfish/object_render/base.c` is a prefix with char pad[552], artificial pad_use, and missing tail; reject. `cloud/work/bigfish/object_render/seed.c` and `cloud/work/bigfish/seeds/object_render.c` are the same complete-shaped m2c source, not absent source, but contain guessed two-argument mode/set-mask prototypes and unreviewed register-shaped locals. They need a full semantic/ABI audit, not adoption as authentic TU. Existing bigfish report freezes broad nonmatch and 632-byte frame. Native calls set-mask at +0x210 and mode at +0x22c; full live ranges, eleven arguments, three real __ashldi3 calls, and accepted func_80087804 calls must be retained.

The native allocation facts already documented by PR48/49 remain the reason to request context: rectangle coordinates survive mode in t2–t5; set/clear-mask preserve mask/address webs across mode; initializer preserves height-global address across mode. Extra formals, artificial keepers, padding or partial object-render source would only simulate the desired allocator pool.

## Minimal maintainer export request (safe text only)

1. **Original context manifest:** named complete source files and hashes for the actual renderer TU/link unit, ordered compile units, original extern/export/uld `-kp` roots, flags, compiler/toolkit identity, and provenance for each choice. If original boundaries are unknown, say so. Do not substitute the accepted stand-in group as historical original-TU proof. No need to export all unrelated game source.
2. **Caller source where warranted:** full audited definitions (or an explicit reconstruction assignment, not an execution packet) for the three callers above; real helper prototypes/types and actual live-across-call evidence at the listed callsites. Establish why each belongs in this compiler closure before enlarging it. Export full existing maintainer source if available; do not ask cloud workers to invent a TU from callgraph reachability alone.
3. **Mode table proof for the exact candidate:** hash-bound JSON/text record with candidate source/object/.text/.rodata hashes, original image identity, logical table address 0x80123870 and 20-byte extent, both paired reference offsets, relocated five entry destinations expressed as named function-relative offsets, table hash, original-versus-relocated equality result, alignment handling, and zero unresolved/unverified/error counts. Original table bytes, native instruction dumps, ROM and binary objects stay private. Include both the accepted baseline proof and changed-closure proof if their object layouts differ. A base address alone is insufficient; copied accepted-body hash does not prove the changed candidate's data.
4. **One novel causal hypothesis:** explain the missing authentic context's concrete effect on the observed register webs; name the one source/context change to test and the expected native geometry. Merely removing table warnings, adding existing B119 bodies, changing keep roots without provenance, or repeating O2/O3 variants is not novel.

Safe text packaging recipe for already-authorized exported source: `git rev-parse HEAD`; `git show <pinned-commit>:<confirmed-source-path> > <export-source.c>`; `sha256sum <export-source.c>`. Copy only reviewed source and structured proof metadata, preserving licenses and exact hashes. Maintainer uses existing private verification workflow to produce item 3, with `blob_group.relocate` on the exact object and original image; no new CLI, arbitrary table-symbol substitution, source mutation, splice, or promotion is prescribed here. Public context inspection needs no private data. No automatic private path reads or guessed private filenames are part of this request.

## Ready / accepted gates and stopping rule

READY requires all of: complete audited source for claimed bodies and genuinely needed context; authoritative target hashes/extents; real ABI/export roots; source-bound table proof; no fabricated code; a baseline receipt tied to exact source; and one new testable hypothesis. Current packet fails context, candidate-table and novelty gates, so it is BLOCKED, not execution-ready.

Once inputs clear preflight, claim only complete matching members, preserve accepted mode context without repeat credit, compare every complete body including stack operands and nonzero excess, and require zero unresolved/unverified relocations/errors. Genuine context members are reported separately from claims. Any affected previously accepted body must remain exact. Actual acceptance remains maintainer-only full linked-image, original compression and ROM hash gates. An unchanged baseline or no causal explanation stops the experiment; do not reset prior sweeps under a new packet name.
