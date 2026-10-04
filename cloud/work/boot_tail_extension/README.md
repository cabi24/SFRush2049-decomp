# Boot-tail static extension: re-issued sdk_initialize context

The static code range now runs to ROM `0x283D0` (vram `0x800277D0`) instead of
`0x10000`, so splat's `data` bin (`assets/us/data.bin`) starts at `0x283D0`.
That change regenerates `undefined_syms_auto.us.txt` and adds the S12 census
function symbols to `symbol_addrs.us.txt`. Both files are pinned by the active
`sdk_initialize` storage owner's compiler-context record. This directory
re-issues that record from a replay of the processes that produced its evidence.
It also rebases the reviewed storage records to the new `data.bin` offsets.

The originals are unchanged and remain the historical record:
`cloud/work/integration_B26/root_compiler_context.json` (sha256 `8198dd79…`),
`cloud/work/integration_B26/root_storage_record.json`,
`cloud/work/integration_B25/storage_record.json` and
`cloud/work/static_C19/strict_verification.json`.

| File | What it is |
|---|---|
| `reissue_sdk_context.py` | The replay and generation script, with one subcommand per step |
| `linked_targets.json` | C19 `verify_targets.py` replay. Both member targets are byte-identical to the reviewed strict proof's `target_sha256`. |
| `strict_rescore.json` | C19 `score_module.py` replay on watchman2 with toolkit `796ae99a…`. Both members score strict 0 / raw 0 against those targets. |
| `sdk_initialize_compiler_context.json` | The re-issued context record. It has the same key set as the original, hashed live; only `symbol_addrs.us.txt` and `undefined_syms_auto.us.txt` changed. |
| `provenance.json` | What changed and why, plus the superseded hash and the replay results |
| `sdk_initialize_storage_record.json` | B26 root record with `data_slot` rebased and `context_proof` pointing at the re-issued record (used by `tests/conveyor/test_initialize_storage.py`) |
| `timer_services_storage_record.json` | B25 record with `data_slot` rebased (used by `tests/conveyor/test_existing_storage.py`) |

Rebase rule: `offset_new = offset_old - 0x183D0` and container vram `0x8000F400` becomes
`0x800277D0`. Absolute slot addresses, sizes and slot hashes are unchanged. The
same rule is applied to `rom_owned_data.json` and to the two `PROFILES` in
`tools/conveyor/pipeline/owned_existing_storage.py`: timer `0x1cff0` becomes `0x4c20`
and initializer `0x1cf60` becomes `0x4b90`.

## Object-hash note

The reviewed proof records `module_sha256 a73bb742…`, an object compiled in another
tree. IDO's `.mdebug` embeds the compiling tree's include paths, so that file hash
is not reproducible elsewhere. The replay checks the strict scores, raw word diffs,
object offsets, word counts, flags, protocol and source hash against the reviewed
proof. It records `.text` and `.data` section hashes instead of the file hash. The
`.data` hash equals the owned data slot's original-byte sha256.

## Reproduce

Run from the repository root. First apply the splat re-split and the
`symbol_addrs.us.txt` edits from this commit, and sync the builder per
docs/BUILDING.md, including `rsync -a --delete asm/ watchman2:~/rush2049/repo/asm/`.

```bash
# Pi: replay target derivation (needs mips-linux-gnu binutils + baserom)
PYTHONPATH=. python3 cloud/work/boot_tail_extension/reissue_sdk_context.py targets \
    --out /tmp/boot-tail-sdk-context
# builder: strict rescore with the pinned toolkit IDO and scorer
ssh watchman2 'mkdir -p ~/rush2049/tmp/boot-tail-sdk-context'
rsync -a /tmp/boot-tail-sdk-context/*.target.o watchman2:~/rush2049/tmp/boot-tail-sdk-context/
ssh watchman2 'cd ~/rush2049/repo && PYTHONPATH=. python3 \
    cloud/work/boot_tail_extension/reissue_sdk_context.py score \
    --out ~/rush2049/tmp/boot-tail-sdk-context'
rsync -a watchman2:~/rush2049/repo/cloud/work/boot_tail_extension/strict_rescore.json \
    cloud/work/boot_tail_extension/
# Pi: context record + provenance, rebased records, registry pointer
PYTHONPATH=. python3 cloud/work/boot_tail_extension/reissue_sdk_context.py context
PYTHONPATH=. python3 cloud/work/boot_tail_extension/reissue_sdk_context.py records
PYTHONPATH=. python3 cloud/work/boot_tail_extension/reissue_sdk_context.py apply
python3 -m tools.conveyor.pipeline.owned_storage generate
python3 -m tools.conveyor.pipeline.owned_data linker rush2049.us.ld
```

A reproduction should leave every file in this directory and `rom_owned_data.json`
byte-identical. The one exception is the `module_sha256` field in `strict_rescore.json`,
which depends on the builder path; see the object-hash note above. Each step asserts its
own gate: the targets must equal the reviewed ones, the strict scores must equal the
reviewed ones, and only the two symbol files may change in the context record.
