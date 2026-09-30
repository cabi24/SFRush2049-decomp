# dynamic_difficulty  ->  really `func_80107EDC` (unregistered head)

**Result: not a strict MATCH. 157 of 158 words are emitted; 152/158 words match
exactly after alignment. The gaps are two dead `move s0,v0` (see below), one
scheduling swap at the tail, and nothing else.** Written by hand from the retail
words, tuned with `extscore.py` (this directory).

```
python3 cloud/work/ipa-groups/dynamic_difficulty/extscore.py cloud/work/ipa-groups/dynamic_difficulty --norm
# func_80107EDC: exact words after alignment 152/158; register-blind structure 0.937
```

## What this is (head defect, INDEX is wrong about the extent)

`dynamic_difficulty` (0x80108098, "47 insns") is **not a function start**. It is
the tail of a function that starts at **0x80107EDC** (prologue
`addiu sp,sp,-72`, epilogue ends at 0x80108154, 158 words) and lives in the
opaque `.incbin` run of `asm/us/blob/blob_80107edc.s`. The label is 0x80108098
inside its loop, at the `lw a2,824(t7)` of the first `music_tempo_adjust` call.
So the group member is named `func_80107EDC`, not `dynamic_difficulty`.

- No `jal` and no `lui/addiu` reaches it. One raw word `0x80107EDC` sits in a
  descriptor table at **0x80115184** (`0x80115184: 80107edc`, records of
  `0, -1,-1,-1, 0,0, <fn>, 0,0,0, ...`). It is a state-handler root, so it is in
  `keep` and uses the standard ABI.
- What it does: a race-screen handler. If not (`D_801174B4 & 8`) and
  (`& 0x400000`) and `D_8014A110` not 1/5 and `D_8016137C`, it sets
  `D_80118E20/24 = 1/3`, switches the audio slot (`slot_state_setup(11 or 12)`
  under `osRecvMesg/osJamMesg` on `D_801461D0`), then for each of `D_80151AD0`
  cars whose side differs from `D_80152734` and whose state is not 1 draws two
  strings with `dispatch_handler(0/1)` + `music_tempo_adjust`, positions from a
  table `D_80115DA8[count-1][i]` (8-byte `{x,y}` entries).

## Why the retail words are not in the repo tools

`score.py` reads targets from `asm/us/blob/*.s` `.text.<name>` sections. This
function is inside `.incbin "build/game_code.bin", 529548, 10440`, so
`score.py group` stops with `no target section .text.func_80107EDC`. The words
are still recoverable: `assets/us/data.bin` is committed and holds the raw
DEFLATE game blob (326,180 bytes at ROM 0xB0CB10 = data.bin offset 0xAFCB10)
that inflates to the 647,072-byte image at 0x80086A50. `extscore.py` inflates it
and compares with the same strict `score.compare` (relocation-resolved), for
members listed under `group.json` `"targets": {name: {addr, words}}`.
**Maintainers:** a `.text.func_80107EDC` section (or a `build/game_code.bin`
fallback in `score.targets()`) would let `score.py group` score this directly.

## Group layout

| file | function | role |
|---|---|---|
| `dd.c` | `func_80107EDC` | member (root, in `keep`) |
| `slot.c` | `slot_state_setup` | context: IPA callee, parameter in `$s2`, ~200 callers in the ROM |
| `sound.c` | `sound_update_channel` | context: IPA callee, parameter in `$t0` |
| `objset.c` | `object_byte9_set` | context, ABI (in `keep`) |
| `stub.c` | `func_80096288` | context stand-in for a 4-word empty check |

Every context body is a **reconstruction from the disassembly, not a match**:
`slot_state_setup` 58/58 words with only `$s0`/`$s1` swapped (18 words),
`object_byte9_set` 16/16 words with `$t1` vs `$t3` (5 words),
`sound_update_channel` 117 vs 122 words (its callee stub cannot be reproduced,
see below). They are there because the member needs `slot_state_setup` to take
its argument in `$s2` (`li s2,11/12` in the `jal` delay slot).

## Remaining differences in the member

1. **Two dead `move s0,v0` after the two `slot_state_setup` calls**
   (`0x80107FA8`, `0x80107FE4`). The result is never read, yet the retail code
   keeps the copy; IDO deletes the dead store in every form I tried
   (`slot =`, `arg0 =`, into `x`, `s32`/`s8` return, unprototyped call, `register`,
   use in dead `if (0)`/`switch`). Same dead move follows every
   `slot_state_setup` call in `catchup_logic` and `reconnect_attempt`, so it is
   systematic, not a source quirk of this function. Unexplained.
2. Tail store order: retail `lui at; li t9,3; sw; sw`, mine `li t9,3; lui at; sw; sw`.

## Tricks worth knowing (all verified here)

- **Inlining.** `umerge` inlines a callee when it has one call site, or when it is
  tiny (about one statement), regardless of `keep` and of the number of callers.
  Stand-in callers do **not** stop a tiny callee (`object_byte9_set`) from being
  inlined. A dead `if (0) { switch (v) { case 0: G = 1; break; ... } }` in the
  callee does (the switch makes it non-inlinable, and uopt deletes it), without
  changing the emitted words. Used in `objset.c` and `stub.c`. Source order and
  file split do not matter (`uld` merges everything).
- `umerge -noinline` exists (`--umerge=-noinline` in `extscore.py`) but is not
  what the ROM used; it is only a diagnostic.
- ABI (kept) functions get `sw a0,N(sp)` home stores; IPA-internal ones do not.

## Closure gaps

- `slot_state_setup` has ~200 callers across the whole game (0x800B4xxx-0x8010Exxx);
  its register choice (`$s0-$s3` unsaved, param `$s2`) is fixed by *all* of them.
  A group only needs its body, but its true context is the whole module.
- `sound_update_channel` (callee `func_80096288`, a 4-word `beqz a2,L; nop; L: jr ra; nop`
  empty check that -O3 cannot reproduce: an empty `if` is deleted and the calls
  vanish), `object_byte9_set`, `camera_shake_update`, `object_bytes_sum_global`,
  `object_manager_update`, `state_utility`, `dispatch_handler` form one audio/text
  cluster. Register naming in the members depends on the exact `$t`/`$a`
  usage of every function in it.
- **INDEX/symbol repair:** 0x80107EDC (158 words) needs registering; so do the other
  unregistered heads in this opaque run: 0x80108154 (frame 152), 0x801084D4 (224),
  0x80108AB0 (200), 0x80108DA8 (40), 0x80108F40 (168), 0x80109468, 0x80109A60,
  0x8010A53C, 0x8010B7FC, 0x8010B9C8, 0x8010BC84, 0x8010C02C. Labels such as
  `dynamic_difficulty`, `catchup_logic`, `skill_rating_update`, `matchmaking`,
  `session_*`, `reconnect_attempt` sit in the middle of these.
