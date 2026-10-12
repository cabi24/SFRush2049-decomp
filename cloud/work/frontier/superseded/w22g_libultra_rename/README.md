# Superseded compiler context: lane w22g libultra rename

Byte-identical copies (from the tree before lane w22g's rename, commit ba4a976e)
of the context files the two existing-TU storage activations pinned
(`cloud/work/integration_B24/context_hashes.json` for timer_services,
`cloud/work/boot_tail_extension/sdk_initialize_compiler_context.json` for
sdk_initialize) and that the rename changed. The activation-replay tests
(`tests/conveyor/test_existing_storage.py`, `tests/conveyor/test_initialize_storage.py`)
resolve a context path here when its live file no longer has the recorded hash,
keeping the historical receipts unchanged. The live owners were rebound by
`static_rename owned` (rom_owned_data.json -> cloud/work/frontier/w22g/rebind/).

`symbol_addrs.us.txt` and `src/rom/lib_8700.c` also serve `tests/conveyor/test_owned_text.py`,
which replays `cloud/work/integration_B22_followup/text_boundary_record.json` (function
osDpWait, now osSpTaskYield; the live row was rebound to cloud/work/frontier/w22g/rebind/).

`undefined_syms_auto.us.txt` (added by lane w24s): the copy from commit 1dde4ccb, byte-identical to
the hash sdk_initialize's activation recorded; w24s merged the scheduler's Hi/Lo timestamp symbols
(splat regenerates this file), so the live file no longer has that hash. The live owner was
rebound with `static_rename owned` (empty table) into cloud/work/frontier/w24s/rebind/.
