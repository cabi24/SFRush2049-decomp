# Group splice: own data placed per reference

2026-10-04. Changes `tools/conveyor/pipeline/blob_group.py` only (plus tests). Nothing was spliced, the lock was not written, nothing is committed. `tools/cloud/owndata.py` and `blob_splice.py` are unchanged.

## Result

| Check | Result |
|---|---|
| `frontier_skid_marks` (staged), 3 members | all equal to the image |
| `camera_scene_manager` (staged), 7 members | all equal to the image |
| 73 locked groups, 149 member bodies, recomputed from the existing objects | all equal to the image; bodies byte-identical before and after the change (same sha256 over all bodies) |
| Locked groups that reach the new code path | 0 of 73 |
| `blob_group check` | 0 problems |
| `blob_unit check` | 730 of 730 |
| Wrong literal, real compile (2 mutants) | refused, message names member, site, retail address, both values |
| Wrong jump-table entry, real object (1 mutant) | refused, names the entry |
| pytest `tests/conveyor` | 1107 passed, 601 skipped, 0 failed |

Full output: [validate.out](validate.out).

Per member (retail address, words, own data verified):

| Group | Member | Address | Words | Own data |
|---|---|---|---:|---|
| frontier_skid_marks | `cpak_init` | 0x800AFD5C | 265 | 1 reference, `.rodata` 0x80123C0C..10 (0.1f) |
| | `func_800AF8C0` | 0x800AF8C0 | 113 | none (0.8f is read through an extern) |
| | `save_validate` | 0x800AFB38 | 135 | none |
| camera_scene_manager | `func_800C2944` | 0x800C2944 | 167 | none |
| | `func_800C26C4` | 0x800C26C4 | 160 | none |
| | `func_800C2430` | 0x800C2430 | 165 | none |
| | `func_800C1B60` | 0x800C1B60 | 297 | 2 references, two jump tables, 0x80123E94..EE4 |
| | `func_800C2004` | 0x800C2004 | 130 | 1 reference, 0x80123EE4..E8 (0.15f) |
| | `func_800C220C` | 0x800C220C | 131 | 1 reference, 0x80123EE8..EC (0.15f) |
| | `func_800C3578` | 0x800C3578 | 37 | none |

The unchanged code refuses both objects with `.rodata: nonzero unmapped data after jump tables`.

## Design

`_local_data_bases` still tries the two whole-section proofs first, unchanged: one section base, then complete jump-table windows. They are now the body of an inner function. Only when they refuse a section does `relocate` place that section per reference:

1. `_member_own_data` runs `owndata.verify` once for every **output** member (the function's retail words, its image address, the whole image as retail data). Any failure or unverified reference in any own section refuses the group, with the member name in front of owndata's message.
2. `_reference_windows` turns the members' verified windows for the refused section into `{object offset: retail address}` (`_ReferenceWindows`). The relocation loop is unchanged; it asks the placement for the address of the offset each HI16/LO16 pair names, so each site gets exactly the address the retail words encode there.
3. A refusal reports both reasons: the per-reference failure, then the whole-section refusal in brackets. Existing tests that match the old messages still match.

Context functions and stand-ins are not verified in this path. Their references only bound the members' windows (owndata's rule: a window runs to the next offset any function references).

Refusals in the per-reference path:

- object bytes differ from the image at the retail address (literal, table entry, local static);
- reference that cannot be checked (HI16 outside the function, unpaired HI16, table entry with no image address, retail address outside the image);
- one LO16 whose retail HI16 words encode different addresses;
- one function's references into one read-only section that disagree on its base (owndata's per-function rule);
- one object offset placed at two image addresses by different members;
- two different object windows placed over the same image bytes;
- an offset a member references that owndata did not verify (a named symbol in an own section, for instance);
- a table entry that enters member text but lies outside every verified window (a member's table cut short by a context reference into its middle);
- the readelf/objcopy view this module relocates differs from what owndata's ELF reader sees (text bytes, `.rel.text`, the section's own relocations).

Still refused before either path, as before: `.bss` and other non-PROGBITS sections, LO16 with no HI16, any other relocation type against own data, missing image.

`compile` (CLI): if relocating with context fails, it now reports the members alone and prints `context not compared: <reason>`. Before, 28 of the 73 locked groups could not be reported by `compile` at all because a context compiles shorter than its extent; the staged `camera_scene_manager` has the same context problem. Exit status still depends only on the members.

## What a reviewer should check

- **One existing test changed meaning.** `test_context_table_is_verified_even_when_only_caller_is_output` is now `test_context_table_is_not_verified_when_only_caller_is_output`: a wrong table of a context function no longer refuses the member. That is the requested behaviour, and it is the only weakened assertion. The same test now also asserts that the table refuses as soon as its function is output.
- **`owndata._Object` is used directly** (underscore name) for the reader cross-check. If that is unwelcome, the cross-check is the only user.
- **The per-function base rule is owndata's and stays.** A function whose literals mix 4- and 8-byte alignment could sit at a different offset modulo 8 in a group object than in retail, and would then be refused although every reference is right. Neither staged group hits it. Relaxing it needs a change in `owndata.py`; I did not make one.
- **Window ends are heuristic.** A member's window ends at the next offset any function references. The table-entry rule above covers a table cut short; a multi-word literal object cut short by a foreign reference into its middle would have its tail unverified. IDO references objects at their start, so I do not expect this.
- **The member bodies are still compared with the image afterwards** (`image_mismatches` in `splice`). Per-reference placement cannot make a wrong relocation value pass; what it adds is the content check on the data the member reads.
- The staged `camera_scene_manager` replaces the locked group of the same name. Splicing it means moving the staged directory over `src/blob/groups/camera_scene_manager`; I did not do that.

## Files

- `staged.py` — compile a staged group on the builder and compare (objects in `build/grouprodata/obj/`, never `build/blob/obj/groups/`); `--mutate` for wrong-literal copies.
- `table_mutant.py` — corrupt one table entry in a copy of the staged camera object.
- `regress.py` — recompute all locked group bodies; `regress.before.json` / `regress.after.json` are the per-member body hashes with the old and new code (identical).
- `validate.out` — output of all of the above plus `blob_group check` and `blob_unit check`.
- Tests: `tests/conveyor/test_blob_group_own_data.py` (new, 21), `tests/conveyor/test_blob_group_table_windows.py` (one test rewritten).

Reproduce:

```bash
python3 cloud/work/frontier/grouprodata/staged.py frontier_skid_marks camera_scene_manager
python3 cloud/work/frontier/grouprodata/staged.py --mutate frontier_skid_marks group.c "lensq > 0.1f" "lensq > 0.2f"
python3 cloud/work/frontier/grouprodata/table_mutant.py
python3 cloud/work/frontier/grouprodata/regress.py
python3 -m pytest tests/conveyor/test_blob_group_own_data.py tests/conveyor/test_blob_group.py tests/conveyor/test_blob_group_table_windows.py -q
```
