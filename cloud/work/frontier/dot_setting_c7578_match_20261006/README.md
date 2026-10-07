# C7578 setting writer: observed local match candidate

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `func_800C7578`, **148 bytes / 37 words**.

The canonical local group comparison reports **MATCH: 0/37 differing words**,
with no unresolved/unverified references, relocation errors or extra nonzero
words. This is an observed local match candidate, not accepted coverage or a
linked-image/ROM claim. Acceptance and integration belong to the independent
checker.

## Source change and real context

- Prior research `cloud/work/ipa-groups/codex_checksum_a17` recorded 21/37
  differing words. Its byte-offset setter, paired with the current accepted
  hash source and two genuine hash callers, scores 19/37 locally.
- The successful C represents the save block as its existing 1856-byte prefix
  followed by thirteen 16-byte setting records. Each setting holds a 4-byte
  checksum and twelve signed-byte fields. This is the actual 2064-byte save-block
  layout used by `src/blob/groups/menu_save_options/group.c`, including its
  thirteen-setting loop and 2064-byte allocation/reset size.
- The setter compares the selected signed field, writes only on change,
  recomputes that row's twelve-byte checksum, and marks the sixteen-byte row
  dirty. Unsigned byte index/field and signed byte value match the observed
  access widths. No new bounds test or memory access is introduced.
- `hash.c` is the unchanged accepted
  `src/blob/groups/codex_hash_a80/group.c` source. `other_callers.c` retains the
  real `object_data_allocate` and `menu_dialog_close` bodies from A17, with only
  the required scalar types and declarations. They are context, not new claims.
- The three real callers are kept external; the hash is internal in this local
  build. This visibility is an explicit hypothesis, not proof of the original
  whole-program partition. Other hash callers exist. No stand-in caller, false
  prototype, dummy formal, padding local, volatile qualifier, assembly or forced
  register is used. The prefix array describes existing record bytes and does
  not allocate extra storage.

## Reproduce

With IDO 5.3 available through `IDO_DIR`, from the repository root:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_setting_c7578_match_20261006
```

Exact source flags: `-g0 -O3 -mips2 -G 0 -non_shared`.
The canonical group pipeline supplies `as1 -r4300_mul` and `-Olimit 5000`.
The empty `claims` list intentionally avoids presenting this local result as
an accepted/promotion-ready group. The normal command above scores the member.
No broad tests, independent acceptance replay, image/ROM integration or CI
watching was performed before publication.
