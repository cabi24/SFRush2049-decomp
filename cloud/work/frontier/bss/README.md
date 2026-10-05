# Own `.bss`: function-local statics in the game image

**Date:** 2026-10-05. **State:** implemented, tested, validated as a dry run. Nothing is spliced or committed.
**Files changed:** `tools/cloud/owndata.py`, `tools/conveyor/pipeline/blob_splice.py`,
`tools/conveyor/pipeline/blob_group.py`, `tools/conveyor/pipeline/blob_unit.py`, and tests in
`tests/conveyor/test_owndata.py`, `test_blob_group_own_data.py`, `test_blob_unit.py`.
**Scorer:** `tools/cloud/score.py` is **unchanged**. It already calls `owndata.verify` and accepts a
reference once owndata proves every masked word, so no `score_py.patch` is needed.

## Problem

No part of the build owned the game's zero-initialised data. When a function's source has a function-local
`static` (or a file-scope object that IDO puts in `.bss`), the compiler refers to it as `.bss+offset`
through a section symbol. The scorer reported those references as unverified, `blob_splice` placed nothing,
`blob_group` refused `.bss`, and `blob_unit` only noted them. Concrete case: `func_800F0674` (472 bytes,
arcade `hiscore.c` `fixword()`). Its code is identical to retail only with the arcade's
`static u8 *op1, *sp; static s16 cnt;`. With `extern` globals, uopt has to assume that `*sp++ = '!'`
aliases them, and 110 of 118 words differ.

## Design

The image holds only the compressed code and `.data`, so `.bss` has **no bytes in the ROM**. Owning it
means owning *addresses* (and extents), not contents.

**Range table:** `owndata.GAME_BSS` has one entry, `[0x801249F0, 0x8017A640)`. The evidence is the static
boot code at `0x80002350`, which calls `bzero(0x801249F0, 0x8017A640 - 0x801249F0)`
(`work/boot/game_init/README.md`; `bzero` is `0x80008590` in `asm/us/blob/symbols.json`). The linked
image ends at `0x801249F0`. The `0x8038A400+` addresses in type_model REPORT §5 belong to runtime image B's
load area, not to `.bss`, and are deliberately left out. Add a range only with evidence of the same kind.

**Verification rule** (`owndata._verify_uninitialised`, called from `verify` for `SHT_NOBITS` sections
named `.bss` or `.sbss`):

1. Every referenced object offset is **one object**, placed on its own, the same way as `.data` and
   unlike the per-function section base of `.rodata`. Retail need not keep the objects at the object
   file's distances. `func_800F0674`'s statics sit at `.bss+0/+4/+8` in the object and at
   `0x80156940/48/50` in retail: the translation unit's other statics are interleaved.
2. All retail HI16/LO16 pairs that refer to one object must encode **the same address**. If they do not,
   the result is a **failure**.
3. **Extent** of an object: up to the next offset that any function of the object references, or, for the
   last object, the widest load or store made to it (`ACCESS_WIDTH`; forming an address counts as 1 byte).
   The section tail is IDO's 16-byte padding and proves nothing.
4. The extent must lie inside one `GAME_BSS` range. If retail has initialised bytes at that address
   (inside the image), the result is a **failure**: the object cannot be zero-initialised. Any other address
   outside the ranges stays **unverified**, as before this rule existed.
5. Distinct objects of the function must not overlap in the image. If they do, the result is a **failure**.
6. A verified object adds a placement of class `bss` (`Result.bss()` gives `{section: {offset: address}}`),
   its sites, a note (`own .bss placed at 0x… (N bytes; zero-initialised: verified by address only,
   nothing to compare)`) and `Result.bss_sites`. `Result.bases()` skips `.bss`.

**What this proves and what it does not.** No bytes exist to compare. What is proved is this: once each
object is placed at the address that retail's own words encode, **every code word that refers to it equals
retail**. The non-immediate bits are compared word by word, as always, and the image gate then compares
the whole body. It does **not** prove the object's size, type or layout, or its zero initial value. It does
not prove that the source's static is the "same" variable as retail's: any object referenced at one
consistent, in-range, non-overlapping address passes. That is weaker than `.rodata`/`.data`, where the bytes
must also be equal, and it is the honest limit for storage that the ROM does not contain. An object's
extent is a lower bound (one byte for an address taken, only), so a large array reached only through a
formed address is not size-checked against its neighbours.

## Paths

- **Scorer** (`score.py`, unchanged): `MATCH` once every masked `.bss` word is a verified site. Wrong
  address, overlap and initialised-data cases are not accepted even with `--allow-unverified`. Out-of-range
  cases stay `MATCH (N … unverified)`, as before.
- **Single-function splice** (`blob_splice._own_data` / `link_function`): one input `.bss` section cannot
  be split, so the linker script places it `NOLOAD` (nothing emitted) at the first object's delta
  (`.ownbss0 0x80156940 (NOLOAD) : SUBALIGN(1) { *(.bss) }`). `_place_bss` then applies each verified
  reference's HI16/LO16 immediates for its object's address, as the linker would for a symbol there. Only
  the 16-bit immediates of the listed sites change. A HI16 shared by objects that need different upper
  halves is refused. Verification failures are a `BuildError` naming the object, the sites and the
  addresses. Unverified objects are not placed, and the image gate refuses the body as before. Objects
  without own references never reach this code (unchanged link script).
- **Group splice** (`blob_group`): `.bss`/`.sbss` (`NOBITS`) relocations are accepted and always go
  through the existing per-reference path (`per_reference` → `owndata.verify` per member →
  `_reference_windows`). That path already refuses one offset at two addresses and overlaps across members.
  Without `per_reference` the old refusal stands.
- **Whole-program unit** (`blob_unit`): `verify_own` checks each `.bss` reference's extent against
  `GAME_BSS`, and a unit-wide pass refuses distinct `.bss` objects that overlap in the image. This is the
  only **cross-function** check: two separately spliced singles that each define a `static` at the same
  retail address are flagged. If retail really has one file-scope object there, the sources belong in one
  translation unit (group) or should use an `extern`. The existing "one offset, one address" check is
  unchanged. `NOTE_BSS` now says "inside game .bss, no overlaps".

## Validation (2026-10-05, lock at 780 entries)

| Check | Result |
|---|---|
| `func_800F0674` from `w2g/func_800F0674/best.c`, IDO 5.3 `-O3` (builder scratch `~/rush2049/scratch/frontier/bss`), `score.py fn … --flags "-g0 -O3 …"` | **strict `MATCH`**, 3 `.bss` notes (0x80156940/48/50); also `MATCH` at `-O2` |
| Same object through `blob_splice.link_function` (dry run, `validate_f0674.py`) | **byte-identical** to the image (472 bytes); 18 sites, 9 references |
| Variant: one `sp` reference moved +4 | link `BuildError` (different addresses); scorer not accepted even with `--allow-unverified` |
| Variant: every `sp` reference moved to `0x80156942` | link `BuildError` (overlaps `op1`); scorer not accepted even with `--allow-unverified` |
| Variant: every `cnt` reference moved to `0x80180000` | not placed, body differs (image gate); scorer `MATCH (… unverified)`, not accepted |
| Variant: every `op1` reference moved to `0x80110000` (in the image) | link `BuildError` (initialised bytes); scorer not accepted even with `--allow-unverified` |
| `blob_unit score func_800F0674 --with best.c` | `EQUAL`, own zero-initialised data note |
| Every locked body relinked with the new code (`relink_locked.py`, cached objects, read-only) | 780/780 equal to the image |
| `blob_unit --tag bss check` | 780/780 equal, 0 differ, own bss 0 |
| `blob_splice check` / `blob_group check` | 0 problems / 0 problems |
| `pytest tests/conveyor tests/cloud`, Pi (no IDO) | 1511 passed, 683 skipped |
| Same, builder (`~/rush2049/scratch/ci` refreshed from this tree, civenv) | 2148 passed, 45 skipped, 1 failed: `integration/test_smoke_strlen.py`, which needs a coordinator token on the builder (`no --token given and ~/.conveyor/token not found`) and is unrelated |

New tests (stdlib plus mips binutils where marked, no IDO):
- `test_owndata.py`: per-object placement with retail strides different from the object's; disagreeing
  references; address outside the ranges (unverified), inside the image (failure), and at the range end,
  where the access width counts; overlapping objects (and adjacent ones, which are fine); an offset outside
  the section; `.bss` mixed with `.rodata`, each failing independently; scorer strict match and the four
  negative cases; partly verified function. Binutils tests: `link_function` places per object, emits
  nothing, refuses disagreement and overlap; an unplaced object leaves exactly its words differing;
  `.bss` with a literal in one link.
- `test_blob_group_own_data.py`: `.bss` outside the range refused; two members' statics placed per object
  in swapped order; overlap across members refused; one static at two addresses refused.
- `test_blob_unit.py`: in-range, overlap, out-of-range and range-end cases.

Reproduce:
```bash
# builder: compile best.c to f0674.o with IDO -O3 (-Wab,-r4300_mul), copy it here, then
PYTHONPATH=. python3 cloud/work/frontier/bss/validate_f0674.py f0674.o   # RESULT: PASS
PYTHONPATH=. python3 cloud/work/frontier/bss/relink_locked.py            # 0 differ
PYTHONPATH=. python3 cloud/work/frontier/bss/scan_bss.py --md            # table below
```

## Integrating `func_800F0674` (integrator)

The source is `cloud/work/frontier/w2g/func_800F0674/best.c` (line 1 gives the flags `-O3`). Its header
says "Not spliceable until something owns game .bss": update that sentence when landing it. Then follow the
normal procedure (`splice_singles.py func_800F0674=…/best.c`, `blob_group check`, `blob_splice check`,
`blob_unit check`, `blob_rom rom`).

## Reviewer checklist

- [ ] `GAME_BSS` is the boot `bzero` range and nothing else. Every added range cites code evidence.
- [ ] `.bss` is never emitted into the image: `NOLOAD` in the single link, and the group path only
      relocates. Image size and the ROM hash gate are unchanged by construction.
- [ ] A reference with retail words that disagree, or that overlaps another object, is a **failure** (never
      silently unverified). An out-of-range address with no retail bytes stays **unverified**, the old
      behaviour.
- [ ] `_place_bss` rewrites only 16-bit immediates at sites that owndata verified, inside the function's
      extent, and refuses conflicting shared HI16 words.
- [ ] `Result.bases()` ignores `.bss`, so `.rodata`/`.data` behaviour is unchanged (all 780 locked bodies
      relink identically).
- [ ] `score.py` is untouched. Its acceptance comes from `owndata.verify` through the existing call.
- [ ] The README states the limits: address-only proof, extents are lower bounds, and the only
      cross-function overlap check is `blob_unit`.
- [ ] Decide whether `tools/cloud/owndata.py` (now also the `.bss` policy) joins the protection hook (open
      decision in frontier plan §0).

## Other functions this may unblock

`scan_bss.py` uses `type_model/refs.json` (every absolute address each game function forms, loads or
stores). It lists unmatched functions that reference a `GAME_BSS` address **no other function
references**. Tier A means every `.bss` address the function touches is exclusive. That is the static-like
pattern, and only `func_800F0674` has it. Tier B mixes exclusive and shared addresses. An exclusive address
in tier B is often just one field of a shared struct that only this function touches at that offset (the
odd addresses are almost certainly fields), so treat tier B as a weak lead. The rule matters only where
retail's code depends on `static` (aliasing, as in `fixword`). Until then, `extern` remains the right
spelling. 126 unmatched functions (163,700 bytes) have at least one exclusive `.bss` address; tier B rows
with fewer than three are omitted. Another 133 `.bss` addresses are shared only by unmatched functions within
0x2000 bytes of each other: possible file-scope statics of one translation unit, which the group path now
accepts.

| Tier | Function | Address | Bytes | Exclusive .bss addresses | Shared .bss addresses |
|---|---|---|---:|---|---:|
| A | `func_800F0674` | 0x800F0674 | 472 | 0x80156940, 0x80156948, 0x80156950 (3) | 0 |
| B | `func_8010FBE0` | 0x8010FBE0 | 128 | 0x80155238, 0x80155240, 0x80155248, 0x80155288 … (5) | 1 |
| B | `func_800B0580` | 0x800B0580 | 152 | 0x80155224, 0x80155228, 0x8015522C, 0x80155B30 (4) | 3 |
| B | `func_800B2BDC` | 0x800B2BDC | 216 | 0x80138880, 0x80138886, 0x80138898, 0x8013889E … (17) | 3 |
| B | `func_800DED78` | 0x800DED78 | 488 | 0x8014080C, 0x80140810, 0x80140814 (3) | 3 |
| B | `func_800E7038` | 0x800E7038 | 252 | 0x80149AFA, 0x80149AFB, 0x80149AFC, 0x80149AFD … (7) | 3 |
| B | `func_80091B00` | 0x80091B00 | 168 | 0x80142DF0, 0x80142DF3, 0x80142E08, 0x80142E20 … (6) | 4 |
| B | `func_800E7710` | 0x800E7710 | 248 | 0x80150F18, 0x80150F58, 0x80156D00, 0x80156D10 … (5) | 4 |
| B | `func_800E8F10` | 0x800E8F10 | 804 | 0x80138668, 0x80138878, 0x801391E8, 0x801392B8 … (5) | 4 |
| B | `func_800A43FC` | 0x800A43FC | 212 | 0x80144336, 0x8014493E, 0x80144F46, 0x8014554E … (7) | 5 |
| B | `func_8010C2E4` | 0x8010C2E4 | 356 | 0x8014A7F2, 0x8014A7F4, 0x8014A7F6 (3) | 5 |
| B | `tire_sound_update` | 0x800B338C | 504 | 0x801428FE, 0x80142900, 0x80142902, 0x80142970 … (6) | 6 |
| B | `mode_select_handler` | 0x800DEF68 | 2976 | 0x80140868, 0x8014086C, 0x80140870, 0x80140874 … (5) | 6 |
| B | `func_800E7FA0` | 0x800E7FA0 | 1244 | 0x8014A254, 0x8014A6D0, 0x8014A6D4, 0x8014A72C … (13) | 6 |
| B | `stat_race_start` | 0x800FE080 | 1068 | 0x8015514A, 0x8015514C, 0x8015514E (3) | 6 |
| B | `entity_collision_detect` | 0x80090B68 | 820 | 0x80154FDA, 0x80154FDC, 0x80154FEC, 0x80154FFC … (10) | 7 |
| B | `audio_doppler_full` | 0x800B61FC | 952 | 0x80154368, 0x80154398, 0x801543A4 (3) | 7 |
| B | `func_800BB02C` | 0x800BB02C | 276 | 0x8013FEF0, 0x8013FEF1, 0x8013FEF2 (3) | 7 |
| B | `drone_target_update` | 0x800D7E88 | 496 | 0x8013C069, 0x8013C06B, 0x8013C06C, 0x8013C06D … (6) | 7 |
| B | `world_collision_response` | 0x800EDACC | 540 | 0x80156BCC, 0x80156BE8, 0x80156BEC, 0x80156BF0 … (20) | 7 |
| B | `physics_collision_test` | 0x800B9194 | 240 | 0x8013F1E4, 0x8013F1E8, 0x8013F1EC, 0x8013F38C (4) | 8 |
| B | `best_times_display` | 0x800D5BB0 | 224 | 0x80140820, 0x80140830, 0x80140834, 0x80140838 … (9) | 8 |
| B | `func_800F45F8` | 0x800F45F8 | 900 | 0x8015449D, 0x801544E9, 0x80154535 (3) | 8 |

Shown: rows with at most 8 shared `.bss` addresses, fewest first. Run `scan_bss.py --md` for the
full list (69 rows), which also includes `net_state_validate` (55 exclusive), `func_800E847C` (32) and
`skid_mark_render` (18).

## Open risks

- **Address-only proof.** See above. A source static that is really a different retail variable would
  still pass if its references match retail's words, which they do by construction when the code is
  identical. The image gate cannot tell either.
- **Extents are lower bounds.** Overlap checks miss an array reached only through a formed address. They
  can also falsely refuse when the object file pads between differently aligned objects and retail packs
  the gap with another object of the same function (not seen; the result would be a refusal, which is safe).
- **Cross-function ownership** exists only in `blob_unit check`. Splices do not record `.bss` ownership in
  the lock or in `blob.ld`. Run `blob_unit check` after every splice, as the procedure already requires.
- **Shared HI16 across objects** in the single link is handled by `_place_bss`. The group path keeps its
  existing LO16-without-new-HI16 behaviour (`hi_imm = 0`), which is correct for objects below 0x8000 in
  their section, and the image gate catches anything else.
