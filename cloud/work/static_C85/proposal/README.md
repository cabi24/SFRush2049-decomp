# C85 private production integration proposal

This folder contains a reviewed draft, not live tool/registry/build changes. The final matching source and full module remain frozen in the parent C85 folder. Root independently confirmed source strict0 against a privately refreshed original target, through the production build wrapper. The shared DB still needs its supported refresh before normal acceptance.

The draft copies the actual production owned_data helper and adds an explicit narrow logical-table extent. `alignment: 4` plus `logical_extent: true` is only accepted for an entire source-built readonly table, with one original R_MIPS_32 relocation per retained word and every destination inside source-built text. All ordinary registry rows retain default16 behavior and cause no object write. No new multi-owner, mutable storage, target normalization or scoring feature is added.

The helper verifies the object path matches the registry-derived owner_object, rejects symlinks, and uses the existing asm_processor MIPS ELF parser/writer. It rejects nonzero excluded tail bytes, incomplete/alignment geometry, excluded or malformed relocations and symbols, unproved table references, invalid symbol indices and overlapping slots. The only structural changes are removing12 proven alignment zeros, setting `.rodata` sh_addralign4, and changing its descriptive SECTION symbol size368→356. Every actual table word, instruction, relocation record and meaningful symbol is retained. The serializer adjusts genuine .mdebug file offsets. All validation precedes the atomic replacement.

`Makefile.integration.patch` adds the CLI object step after ordinary asm processing. Metadata filtering is cheap for unrelated ROM TUs; full original protected-source checks occur only for matching logical owners. Existing dependencies already rebuild ROM objects when the registry/helper changes. `fcvt.slot.proposed.json` is the one intended new row. Root must merge it with existing rows, preserving existing owners. `fcvt.table.proposed.s` is the genuine symbolic original table companion for passthrough baseline; conversion/promotion's existing companion lifecycle applies. It declares no native-body coverage.

The generated linker moves the actual owned section into original data-container position and asserts start8002D558, size356 and actual4-byte alignment. The discarded section tail cannot cover the following12-byte interval, which has8 nonzero bytes. No leading padding, masked entry, changed opcode or replacement container is introduced. The symbolic companion baseline and complete native final object both reproduce the entire original12,517,376-byte data container SHA256c348ea04768321ca2f864195ad6013a8fb15b59f0295ab74b8b16317cfa77154. The full native final emits8528 exact original text bytes/all9 members; baseline has8 native members/4580 native bytes and actual fcvt asm passthrough. Untrimmed sections fail exact linker size; a changed neighbor remains visible and fails final container identity.

Validation is36 new real MIPS ELF/helper tests plus39 unchanged existing ownership tests run against the draft in a private Python process; both pytest exits are0 and recorded. Tests exercise the production split/link/rewrite functions, real GNU assembly ELF records, serializer output and linker assertions. They also require atomic refusal for a different valid89-entry-table object passed with fcvt's TU and for a malformed relocation symbol index. The actual IDO module and passthrough baseline proofs add unmasked original-word checks outside the unit fixtures.

Reproduce from repository root:

```sh
PYTHONPATH=. pytest -q cloud/work/static_C85/proposal/test_owned_table_extent.py
python3 cloud/work/static_C85/proposal/run_existing_compatibility.py
python3 cloud/work/static_C85/proposal/verify_actual_module.py
python3 cloud/work/static_C85/proposal/build_passthrough_companion.py
python3 cloud/work/static_C85/proposal/verify_actual_module.py --passthrough
```

The source-built game/current full-ROM, normal lock and pytest gates remain root responsibilities after live review and integration. These private original-container proofs do not claim current cartridge coverage or replace those gates. Review/apply the narrow helper/Makefile edits after coordinator ownership is resolved, then supported scoped target refresh, baseline conversion, normal fcvt lock/promotion and forced current ROM acceptance.
