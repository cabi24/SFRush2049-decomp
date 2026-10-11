# w20p - boot-tail packet verifiers accept the registered owned slots (tooling lane, worktree)

Branch `w20p` (worktree `.claude/worktrees/w20p`, from `wave20` d426eced). Not pushed to origin.
IDO runs on rocky (`~/rush2049/scratch/w20p`, own clone of ciclone-coord, IDO 5.3 + 7.1, CI binutils/venv).

## Problem

w20c gave three boot-tail jump tables owned `.rodata` slots (rom_owned_data.json) and promoted
func_80021548 (lib_21210) and func_80023BDC (lib_22300); lib_1a660 carries its slot's passthrough
companion. Three research-packet verifiers assumed those TUs had no allocated data and failed on an IDO host:

| packet | failure on rocky before |
|---|---|
| voice_lists | `unexpected allocated data` (lib_1a660 current/combined: companion `.rodata`, 0x30 B) |
| macro_stream_contracts | `func_80023BDC: MATCH (2 section-relative relocations unverified: .rodata+0x0 at +0xa0, +0xa8)` |
| macro_wrapper_contracts | `allocated non-text sections changed` (lib_22300 now has `.rodata`); after that, the baseline passthrough's `jtbl_8002D950_main` was unresolved |

## Fix: `cloud/work/boot_tail_promotion/owned_slots.py` (shared by the three verifiers)

- `slots(tu)`: the non-storage `rom_slots` registered for `src/rom/<tu>.c` (registry read with
  `check_source=False`: no extracted assets on CI), keyed by owner section.
- `allocated_sections(obj, tu)`: every allocated section other than `.text`/`.reginfo` must be the owner
  section of a registered slot of that TU (no NOBITS), fit it (`owned_data.section_fits`: a logical-extent
  table may carry IDO's 16-byte padding), have only R_MIPS_32 relocations inside the slot extent, zero
  bytes past the extent, and, relocated (`.text` addends through their owning function, as score.relocate
  does), hash to the registry's original-byte SHA-256. Anything else raises.
- `relocate(obj, tu, words, start, end)`: score.relocate, then a HI16/LO16 pair the scorer left
  masked/unverified (compiled C: section symbol + addend) or unresolved (untouched passthrough asm: the
  slot's split label `jtbl_<ADDR>_main`, ADDR == the slot start) is applied at the slot address iff the
  addend lies inside a registered slot of this TU; a section-relative one also requires
  `allocated_sections` to prove the object's slot section. Only those exact detail strings are dropped
  from the scorer's lists; everything else stays and the caller fails closed.
- `accepted(result, obj, tu, fn)`: the scorer's strict verdict, or (no errors, no extra words) the full
  relocated words after `relocate` equal the target with nothing masked/unverified/unresolved.

Verifier changes (only where a ROM TU object is examined; standalone candidate objects keep the old
strict path with `tu=None`):
- voice_lists `compare_all(..., tu)`: allocated data outside the TU's slots still raises
  `unexpected allocated data`; strict match and the exact unmasked full-body proof go through
  `owned_slots`; report keeps `allocated_data_bytes == 0` (unregistered data) and adds
  `owned_slot_sections` per object and `owned_slot_references` per function.
- macro_stream_contracts `exact_bytes(..., tu)`, `allocated(..., tu)`/`untouched(..., tu)`: owned-slot
  sections are proven by owned_slots and left out of the baseline==current comparison; all other
  allocated sections must still be byte-identical.
- macro_wrapper_contracts `checked_bytes(..., tu)` and `allocated_sections(obj, tu)`: same.
- All three add owned_slots.py, rom_owned_data.json and owned_data.py to their hashed inputs.

Receipts rebound (handoff §5 test-fix class, verify.py hash only): voice_lists, macro_stream_contracts,
macro_wrapper_contracts `verification.json`, and contract_integration / remaining_contract_integration
where they pin those verify.py files. (voice_lists and macro_stream receipts were already stale: they
pinned a pre-4df89aad hash.)

## Negative controls (`negatives.py`, lib_22300 built with IDO 7.1 on rocky)

The real object: scorer `MATCH (2 section-relative relocations unverified)`, strict false;
owned_slots accepts, `.rodata` 32 B in slot func_80023BDC (20 B) at 0x8002D950, relocated slot SHA-256
`60d6380e...` = registry. Refused: the same object against lib_21210's slot (hash differs); slot moved by
4 B (references fall outside / mismatch); slot shrunk to 4 B (`32 B exceeds the owned slot`); wrong
registry hash.

## Tests

- rocky (IDO 5.3 + 7.1, CI binutils/venv, `REQUIRE_TOOLCHAIN=1`): the three packets' tests
  (`-k "voice_list or macro_stream or macro_wrapper"` over tests/cloud + tests/conveyor/test_voice_list_promotion.py):
  45 passed (before: 4 tests/cloud failures + 4 voice_lists errors).
- rocky `make test-fast JOBS=16` (REQUIRE_TOOLCHAIN=1): tests/conveyor 2197 passed, 51 skipped;
  tests/cloud 912 passed, 1 skipped, 27 subtests passed; rc=0. (A first plain `pytest -n 16` run had 3
  environment failures in the fresh clone: no `origin/master` ref (fixed by fetching it; both tests then
  pass) and the strlen smoke test's missing coordinator token; neither touches this change.)
- Pi `make test-fast JOBS=4` in the worktree: tests/conveyor 1445 passed, 803 skipped; tests/cloud 770 passed,
  143 skipped; rc=0 (no IDO on the Pi: toolchain tests skip).
