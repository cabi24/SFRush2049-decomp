# Task enqueue callback: typed research, still a nonmatch

Base: `cf10b3392d7f00ae42d75c008b79fdc2541aab6b`. Scope is only
`func_8010FBE0`, `[0x8010FBE0, 0x8010FC60)`, 128 bytes / 32 words.
No accepted source, symbols, context, lock, target, scorer, or ROM build was
changed. No new coverage is claimed, and this source is not a splice candidate.

## Useful findings

The record at `D_80155238` has the repository's native `OSScTask` layout, rather
than the generic message/buffer labels in earlier packets:

| Offset | Field | Observed operation |
| --- | --- | --- |
| 0x00 | next | clear the linked-list pointer |
| 0x04 | state | untouched |
| 0x08 | flags | set to 2 |
| 0x0c | framebuffer | untouched |
| 0x10 | list | copy the input OSTask, exactly 64 bytes on the native ABI |
| 0x50 | msgQueue | set to the completion queue at `D_80152750` |
| 0x54 | msg | clear the completion message |

The function then jams the task record into `D_8002E960`, followed by integer
message token 670 into `D_8002E928`. Both calls use blocking mode 1 and ignore
their return values. The separate `sync_acquire_menu` waits on `D_80152750`.
The token is deliberately left numeric: the labels in the reconstructed
`src/libultra/os_scheduler.c` are not sufficient to name this event confidently.

Layout provenance: `include/m2c_types.h:117-129` and the scheduler's actual
field accesses in `src/libultra/os_scheduler.c`. The candidate replaces the
partial OSTask padding in that context with the known OSTask field layout
already present in `src/blob/sync_acquire_menu.c`. `abi_probe.c` checks every
record offset, 64-byte OSTask size, 88-byte scheduler-record size, and 32-bit
pointers under IDO. No anonymous padding or invented external field aliases
are used in this candidate.

The callback path also explains the absence of direct blob callers:

- The existing, integrity-checked blob data artifact has this function's pointer
  at `0x8011EAAC`, followed by the three neighboring callbacks.
- `car_shadow_render` passes the table at `0x8011EAAC` to `func_80020598` at
  its call site `0x800A5F2C`. Its historical name is not a semantic inference.
- The static native body of `func_80020598` copies that eight-word callback
  table to `D_80038000`.
- The static native body of `func_80011910` builds the OSTask at `D_800382F8`,
  writes back its audio command data, and invokes the first callback with that
  task pointer at `0x800119B8`.

These are cross-image registration/call observations, not a request to group
or alter the static functions. Sources: existing
`asm/us/nonmatchings/rom/lib_207b0/func_80020598.s`,
`asm/us/nonmatchings/rom/lib_11640/func_80011910.s`, protected blob target/data,
and the existing type-model reference map. No native bytes or assembly are
republished in this packet.

## Matching result and stop condition

`candidate.c` uses the natural typed aggregate with the observed store order.
At `-g0 -O3 -mips2 -G 0 -non_shared` it has **30/32 words different**. Every
relocation is resolved, but its ELF function size is **124 bytes**, with four
additional section-alignment bytes. That is not the 128-byte target extent.

The residual is global-address expression ownership, not a wrong memcpy size
or queue signature. The aggregate source carries one shared pointer register;
the native target uses direct global stores, sharing only an upper-address
materialization between the two completion fields. A workbench diagnostic was
run before the bounded search and classified the change as structural.

The bounded sweep tries all 24 orders of the four independent pre-copy field
assignments, then the same 24 with a volatile aggregate as diagnostic controls.
All ordinary cases remain 30/32 with a 124-byte function. Volatile controls range
from 26 to 30 differing words, still 124 bytes. The lower positional number is
not convergence on the ownership issue; no volatile version is adopted because
the native evidence does not establish volatility. `-O2` emits the same ordinary
body; an `-O1` diagnostic was further away and added argument spilling. A native
static-object control remained a nonmatch with unverified BSS relocations, and
was not retained as a candidate. Pointer-local, signed-flag, and field-volatile
controls did not improve the ordinary aggregate result.

Stop here rather than invent a callee, alias, fake parameter, forced register,
assembly fragment, dead local or padding. The next useful hypothesis needs
authentic translation-unit/global-ownership evidence for the task declaration
and the compiler's direct-store lowering. Repeating store-order variants has
no demonstrated value.

## Correction to prior extent reports

Fresh IDO `st_size` checks of earlier packets find:

| Earlier source | Function bytes | Section bytes | Word differences |
| --- | ---: | ---: | ---: |
| `tiny_A116/func_8010FBE0.c` | 132 | 144 | 29/32 |
| `tiny_A116/control.scalar_globals.c` | 132 | 144 | 30/32 |
| `tiny_A116/control.packet_view.c` | 124 | 128 | 30/32 |
| `tiny_A50/func_8010FBE0.c` | 124 | 128 | 30/32 |

All four report `extra_words=0`: that field counts only nonzero trailing words,
so it cannot establish actual function extent. Older "exact extent" wording
must not be reused. `verify.py` adds a packet-local conjunct requiring the
actual ELF extent to equal 128; it does not alter the canonical scorer.

## Reproduce and tests

Use the pinned IDO 5.3 environment; no remote builder is used:

```sh
python3 cloud/work/frontier/task_enqueue_20261005/verify.py
python3 cloud/work/frontier/task_enqueue_20261005/sweep.py
python3 -m pytest -q tests/cloud/test_task_enqueue_research.py
```

`verification.json` freezes source identity, all relocated-word results, ELF
extent and section padding separately, native ABI assertions, compiler/target
manifest identity, and the earlier source replays. `sweep.json` contains counts
and controls, never target bytes. The five tests cover nonmatch honesty, the
independent size/relocation gate, the bounded sweep, a fresh IDO replay when the
compiler is available, and host behavior with four task patterns and both
success/failure returns from the first enqueue. The host test is semantic
evidence only; the IDO probe establishes native layout and copy size.

No image-link, whole-program, compressed-stream, or full-ROM gate was run:
the candidate fails the prior matching/extent gates and is not promoted.
