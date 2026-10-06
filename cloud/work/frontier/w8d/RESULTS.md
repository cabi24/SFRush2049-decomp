# w8d — frontier wave 8, sibling-family lane (2026-10-05)

Flags everywhere `-g0 -O3 -mips2 -G 0 -non_shared`. Builder scratch `~/rush2049/scratch/frontier/w8d`
(from `base`), unit tag `w8d`. Tools in `tools/` (w7b copies retargeted to w8d; `sc.sh` now passes the -O3
flags by default — the w7b copy passed none and so scored at the scorer's default; `fb.sh`/`fbx.sh` batch
`full.py` aligned-row counts, `fbx.sh` also prints chosen got-columns, e.g. frame and a slot offset). Traced
uopt copied from w3a into `~/rush2049/scratch/frontier/w8d/uopt/`. Nothing committed or spliced.

| Function | Bytes | State | Where |
|---|---:|---|---|
| check_mpath_save (9 dependents) | 276 | **strict MATCH**, unit EQUAL | `cloud/matches/check_mpath_save.c` |
| func_800E7710 | 248 | **strict MATCH** (own .bss placed at 0x80150F58, zero-init, by address), unit EQUAL | `cloud/matches/func_800E7710.c` |
| func_8010D680 | 476 | 22 aligned rows (w6d: 49) | `func_8010D680/best.c`, `NOTES.md` |
| track_collision_setup, MaxPathZeroControls | 468, 396 | skipped: callee `track_data_decompress` unmatched | — |
| func_800E73D8 | 596 | skipped: callees `func_800A43FC`, `func_800E7134` unmatched | — |
| func_80106874 | 712 | skipped: `object_bytes_sum_global`, `slot_state_setup` unmatched | — |
| func_801084D4 | 1,500 | skipped: `slot_state_setup`, `state_utility` unmatched | — |
| func_80109F54 | 1,504 | skipped: `slot_state_setup` unmatched | — |
| func_8010C974 | 2,636 | skipped: `camera_trigger_check`, `save_write_data` unmatched | — |

2 strict, 524 bytes. Both new matches unblock nothing by themselves except check_mpath_save's 9 dependents.

---

## check_mpath_save — strict MATCH

```
tools/sc.sh cloud/matches/check_mpath_save.c check_mpath_save
check_mpath_save:
  MATCH
python3 -m tools.conveyor.pipeline.blob_unit --tag w8d score check_mpath_save --with cloud/matches/check_mpath_save.c
  EQUAL check_mpath_save: 69 words (kept, c_check_mpath_save.c)
blob_unit score: 1/1 equal
```
-O2: 68/69 (helpers not inlined). Semantics: Rumble Pak reset for 4 ports under the SI/pak lock: clear
D_8011EAE4; for each present pak (byte +6): clear +125, SDK `osMotorInit(&D_80035458, &pfs, i)` (0x8000A194,
label osMotorStart), `__osMotorAccess(&pfs, MOTOR_STOP)` (0x80009F20, label osMotorInit), clear +124 if it
returned 0.

The w7b residual (retail `bnez; b next` jump-around, ours `beqz`) closed by: **the per-pak body is an inlined
static helper with a value early return** `if (!p->present) return -1;` and *no* value on the normal path. ugen
emits `li $2,-1; b $join` for the return path (as1 deletes the dead `li`, leaving retail's `b` with the
increment copied into its delay slot). A `void` helper (w7b's h1/h2), `continue`, `goto`, if-blocks: single
`beqz`. `return 0;` at the end adds `move $2,$0` at the join, which blocks as1's `bnezl` fill (3 rows).
Then the frame: 88 with the OSMesg at sp+68 needs main to keep `Pak772 *p` and the unlock to be a direct
`osJamMesg` (pak_unlock helper → message at 76; flat pak_lock → frame 80). Sweeps `check_mpath_save/s1..s9`,
`e/` (frame experiments).

## func_800E7710 — strict MATCH (own .bss)

```
tools/sc.sh cloud/matches/func_800E7710.c func_800E7710
func_800E7710:
  MATCH
    own .bss placed at 0x80150F58 (2 bytes; zero-initialised: verified by address only, nothing to compare)
python3 -m tools.conveyor.pipeline.blob_unit --tag w8d score func_800E7710 --with cloud/matches/func_800E7710.c
  EQUAL func_800E7710: 62 words (kept, c_func_800E7710.c); own zero-initialised data (addresses consistent, inside
  game .bss, no overlaps; nothing to compare); stub tail: 2 words of deleted-procedure stubs follow (NOT as in the image)
blob_unit score: 1/1 equal
```
-O2: 46/62. Semantics: controller init — `osCreateMesgQueue(&D_80035458, D_80150F18, 8)`,
`osSetEventMesg(OS_EVENT_SI=5, &D_80035458, &msg)` with a static s16 msg = 5, pak lock, `osContInit(&D_80035458,
&D_80111950, D_80149440)` (0x80009450, label __osContBuildPacket), unlock, clear 4 rumble bytes.
w7b's residual (retail stores via `lui at; sh t6,%lo(at)` and builds the argument separately; ours one CSE'd
address register) closes when the message is a **function-local static**: a scan of every retail
`lui 0x8015` + `0x0F58` reference finds only this function, so the variable is its own .bss. Also identical:
an extern struct based at D_80150F18 with the message at +0x40 (different base expression, no CSE) — rejected,
D_80150F38/F40 belong to other code (engine_torque_calc, net_state_validate...). Making the buffer D_80150F18
a local static too also scores MATCH, but it would sit 32 bytes before the message, not 64, so it stays extern.

**Integrator note (stub tail):** the stub after func_800E7710 in the unit is not from this file. Without it,
the unit already has `func_800E7030 …; jr ra; nop; jr ra; nop; func_800E79F0` (stubs from a neighbouring
locked file); inserting this file between them by address leaves one of those stubs after it. The three
stubs *before* it are this file's inlined statics, as for the locked func_800A1A60.

## func_8010D680 — 22 aligned rows (see `func_8010D680/NOTES.md`)

```
tools/full.sh func_8010D680/best.c func_8010D680  ->  want 119 words, got 119; differing rows 22; unverified 2
tools/sc.sh func_8010D680/best.c func_8010D680     ->  82/119 words differ (2 section-relative relocations unverified …)
```
`(Holder *, s16 on)` callback: on == 0 → unlink; paused (D_801170FC) → return; flag bit 2 → sound at
vel·dt; else if node D_8012E700[(s16)handle] sign bit: type 350..360 → slot 0..7 (353/354/359 → -1); if no
car (D_80152818[i].index, +900, stride 952) holds the slot, model_transform_setup(handle, 0, 15) and set bit 2;
else model_data_load(handle, 0, 15).
Found: the sign-bit test is an inlined `static s32 model_visible(s16 id)` (gives retail's v1 narrowing and the
`sll 0; bgez`; 49 → 26 rows), a separate `ok = 1` flag (`li a1,1`), one extra word slot before `v[3]`
(`s32 pad` stand-in; 26 → 22). Residual lane: liveness of `on`. Traced uopt (proc 939): `on` coloured a1 in
both, nocs 1 (ours) vs 7 when reused; retail keeps `on` live in a1 past the entry block, which also explains
as1 not hoisting `move a1,zero`/`move a0,t0` above the `bgez` (retail `bgezl` + delay fill) and the `lui v1`
placement. ~45 variants tried (listed in NOTES.md). Next: a read of `on` between the visible test and the loop
that survives dead-store elimination but emits nothing.

---

## What generalises

1. **Value early-return from an inlined helper = `bne; b` jump-around.** `if (!x) return -1;` inside an
   inlined static makes ugen emit `li $2,-1; b join`; as1 deletes the dead `li` and fills the `b` delay slot
   from the target. When retail shows `bnez X,body; …; b next` with nothing in the skip path, look for this.
   A helper that returns a value on only one path (falls off the end) is what fits a join with no `move $2`.
2. **Every inlined helper call moves the frame**, and not monotonically: in check_mpath_save each of
   pak_queue_init-inside-pak_lock, pak_unlock, the motor helper and main's `p` shifted the frame or the
   OSMesg slot by 8. Sweep "which calls are helpers" (2^n combinations, `fbx.sh` printing frame + slot) when
   the frame or one local slot is off by 8 in a helper-family function.
3. **Global address CSE vs `lui at; s? %lo(at)`:** if retail stores to a global through `at` and separately
   builds its address as an argument, the variable is probably a function-local static (or at least not the
   same base expression). Scan retail for other references to the address before deciding.
4. Family status: pak-lock family now 3/6 matched (func_800A1A60, check_mpath_save, func_800E7710); the other
   three wait on track_data_decompress / func_800A43FC / func_800E7134. The `(Obj *, s16 mode)` family's
   remaining members (except func_8010D680) all wait on slot_state_setup or other unmatched callees.
