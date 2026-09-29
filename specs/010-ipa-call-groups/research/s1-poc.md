# S1 spike: reproduce one real IPA call group (2026-09-29)

**Result: PASS at the instruction level.** Three functions compiled together
at IDO 5.3 `-O3` are word-identical to the ROM with only relocation fields
masked (R_MIPS_26 targets, HI16/LO16 immediates):

| Function | ROM | Insns | Role | Standalone -O2 |
|---|---|---|---|---|
| `func_80097694` | 0x80097694 | 65 | static leaf, temps squeezed to `$t6`-`$t9` | score 150 (register choice) |
| `resource_slot_clear` | 0x800C9334 | 18 | static IPA callee, parameter in `$t0` | not decompilable (m2c "unset $t0") |
| `resource_slots_clear_multiple` | 0x800C937C | 12 | caller, `li $t0,N` in the jal delay slots | cannot match |

Source: [s1-poc/group.c](s1-poc/group.c). Compile:
`cc -c -g0 -O3 -mips2 -G 0 -non_shared group.c` (toolkit 796ae99a, IDO 5.3).
Verification: the `.text` words of each slice were compared with
`build/game_code.bin` at the function's address, masking only the fields named
by the object's relocations.

## What made it work

1. **One translation unit with the helpers `static`.** `func_80097694` lives
   0x31CA0 bytes away from the other two in the ROM, yet it had to be static in
   the same TU as its callers for the joint allocation to happen. A two-file
   whole-program `-O3` compile (`cc -O3 -c a.c b.c`, uld-linked) kept the
   global callee on the O32 ABI, put the parameter in `$s0`, and did not
   restrict `func_80097694`'s temporaries.
2. **A second caller of the leaf.** With `resource_slot_clear` as its only caller,
   `-O3` inlined `func_80097694`. The stand-in `audio_user` in group.c keeps it out
   of line. In the game its other callers live in the audio code around 0x80097xxx.
3. **Joint register allocation is real.** The leaf avoided every register its
   caller keeps live across the call (`$t0`), so the leaf's temporaries cycle
   `$t6`-`$t9`. That is the pattern that also blocked `input_aux_handler` at -O2.

## Open questions this raises (feed S2-S4)

- **Unity build?** The leaf and its callers sit in different parts of the image
  but had to share a TU. That suggests the game's game-code was built as a few
  large TUs (`#include` of many .c files) or the linker localized globals
  (`uld -kp <file>` exists and takes an argument; undocumented here).
- **Layout.** `-O3` did not emit functions in source order. The output was
  leaf, `audio_user`, `resource_slot_clear`, `other_user`,
  `resource_slots_clear_multiple`. The ROM has `resource_slot_clear` immediately
  followed by `resource_slots_clear_multiple`. Reproducing layout needs the real
  member set and the emission-order rule.
- **Splicing.** `blob_splice.link_function` links a whole object's `.text` at
  one address. Group members live at different addresses and call each other
  through local relocations, so a group splice has to place each slice at its
  own image address (Phase 4). Until then this result is word-level evidence,
  not cartridge coverage. No image-gate or ROM-SHA-1 claim is made.
- **Stand-in callers.** `audio_user`/`other_user` are fabricated. They only
  had to keep the leaf out of line and not disturb the allocation. S2 should
  test how sensitive member code is to what the stand-ins do.
