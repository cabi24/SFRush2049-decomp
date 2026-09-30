# audio_heap (first-fit audio heap allocator)

**2 of 6 members MATCH** (`audio_helper` 57 w, `func_800E7B44` 58 w), scored with
`python3 tools/cloud/score.py group cloud/work/ipa-groups/audio_heap` (round 5, hand-written from
the assembly; `claims` lists only those two).

| function | words | result |
|---|---|---|
| `audio_helper(size, heap, owner, tag)` | 57 | **MATCH** |
| `func_800E7B44(heap, count, a)` | 58 | **MATCH** (needs the dead `if (a == 0) {}`, see below) |
| `audio_dma_sync(heap, size)` (`keep`) | 31 | aligned except one `move a2,v0` (see below) |
| `func_800E7C2C(heap, size, count, a)` (`keep`) | 54 | aligned except the same `move a2,v0`; list walk matches as `for (p = head; p; p = p->next) if (!p->next) { p->next = h; break; }` |
| `func_800E7D0C(count, a)` (`keep`) | 49 | 7 words: flag address CSE (see below) |
| `audio_task_complete(heap, size)` (`keep`) | 104 | about 25 words: frame 56 vs 48, registers of `heap`/`h`/`i` |

## What this is

The game's heap. A block header is 32 bytes (`magic 0xFEDCBA98, next, prev, size, owner, used, tag`);
`audio_helper` is first-fit with a split when at least 64 bytes remain, and it does not check for
out-of-memory (the target dereferences the null block). `func_800E7B44` formats a heap
(`magic, next heap, first/last block, end, maxHandles, handle table {count, slots, next}`),
`func_800E7C2C` carves a sub-heap out of another and links it on the global list `D_801527C8`,
`func_800E7D0C` sets up the global heap in `D_8017A640`, `audio_task_complete` hands out a handle slot.
`D_80152770` is the heap lock (message queue).

## IPA facts learned

- `audio_helper` takes `(size, heap, owner, tag)` in `a0, a2, t0, t1`: IPA skips `a1`/`a3` because the
  body uses them as temporaries. Declaring a dummy `a1` parameter shifts every register by one. The body
  must therefore load `b->size` once per iteration into one local (`bs`) and reuse it after the loop.
- `func_800E7B44(heap, count, a)` is `t2, a1, a0`; the home slots are `a0`->32, `a1`->28. IPA gave
  `count` `a0` until the first use of `a` preceded the first use of `count`; a dead
  `if (a == 0) {}` at the top reproduces the register order (compile-affecting quirk, not necessarily
  the original source).
- The retail callers pass `lhu a1,26(sp); lhu a0,30(sp)`: `func_800E7D0C(count, a)` calls
  `func_800E7B44(heap, count, a)`.

## Open problems

1. `audio_dma_sync`/`func_800E7C2C`: retail evaluates `heap ? heap : D_801527C8` into `v0` and then
   `move a2,v0` (param `heap` reloaded from its home slot); mine puts the ternary straight in `a2` with the
   parameter in `a3`. Tried: local temp, `if` assignment, casts, `register`, reassigning the parameter.
2. `func_800E7D0C`: retail reads the flag `D_80116488` with `lui t6; lb t6` and stores it with `lui at; sb`
   (two symbolic accesses). Mine commons the address (`addiu v0`). Reading through `*(s8 *)0x80116488`
   gives the `lui at` store but moves the load registers.
3. `audio_task_complete`: retail frame is 48 with `heap` param in `s0` shared with `i`; mine needs 56.
   Structure (loops, handle-table growth) is right; register/frame shape is not.
