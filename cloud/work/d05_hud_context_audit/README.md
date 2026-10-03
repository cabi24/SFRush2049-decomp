# D05: published HUD context and receipt audit

Status: **NONMATCH, frozen research; claims: []**. Owner: round10 lane E.
Base: `d701b59592463d1e33bcee7011ed2a1e6c07b066`.
Branch: `dot/round10-e-hud-context`. No accepted coverage, image, or ROM claim.

## Outcome and new evidence

1. PR9 at `433611408270ede7fe6acdb027f1b82179209511` does **not** contain
   `module_campaign_20261002/reconstruction/minimap_dots/`. Its interrupted
   map-offset receipts cannot be audited or declared complete from published
   repository evidence. No private recovery locations were accessed. The
   correct repository-only status is **unavailable, not recovered**, not a
   pending compiler sweep and not a completed minimap reconstruction.
2. All eight published `large_hud_marker` files agree with PR9's snapshot
   manifest. However, `reconstruction/baseline.c` hashes to `e04446e4…`, while
   its historical `baseline_O2.json` attributes its result to `b095e94f…`.
   Matching a snapshot manifest does not establish source-to-receipt identity.
   The fresh replay here binds the actual published source to a new receipt.
   Why the old hashes differ is unknown; this report does not assert semantic
   equivalence between the unavailable old source and the published source.
3. Fresh IDO replay of the published baseline recovers **431/433 differences**,
   frame 120, candidate extent 1,696 bytes versus native 1,732. The actual Input
   O3 source hash agrees with its historical receipt. It reproduces **408/433
   differences**, **9 nonzero excess words**, and the unpaired HI16 error for
   `D_8011407C`; frame 112, candidate extent including padding 1,776 bytes.
   Fewer positional differences do not make this overflowing candidate closer
   to acceptance. `audit.py` calls the unchanged canonical compile/compare
   APIs. The archived group descriptor is a compile recipe without `members`,
   so the stock group CLI cannot consume it directly; no claims were invented
   to bypass that distinction.
4. A native signedness defect is visible in the older registered-head minimap
   seed (`cloud/work/registered-heads/seeds/func_80109A60/seed.c`): `sp1A` reads through `u8 *` at `D_80142DB4`, whereas target +0x20 uses a
   signed byte load. Byte 255 must become -1 before the `(slot + 1) == 0` test.
   The unsigned seed misses that sentinel and can reach invalid player-array
   indexing. A focused exhaustive byte-domain regression demonstrates this
   difference. That generated skeleton is not the missing donor reconstruction
   and has not been promoted or silently repaired here.

## Native geometry before any new source sweep

| Body | Full native bytes | Frame | Prologue saves | Direct calls |
|---|---:|---:|---|---|
| `func_80109A60` | 1,268 | 40 | ra at 20 | Input 3, SelectBlit 2 |
| `game_results_input` | 1,732 | 120 | ra at 28 | Input 5, SelectBlit 2, projection 2, asset/name 1 |
| `Input_ApplyPadConfig` | 192 | 40 | ra at 36, s0 at 32 | four real low-level helpers |
| `stat_race_update` | 396 | 24 | ra at 20 | asset/name 1, Input 1 |
| `input_new_data_wrapper` | 60 | 24 | ra at 20 | Input 1 |

The `Input` label means `Input_ApplyPadConfig` at `80094EC8`; `SelectBlit`
means historical `stat_race_update` at `800FE5B0`. These are rendering/sprite
operations here, not evidence of controller configuration or race timers.

Both real callbacks contain the Hidden compare/store/refresh/reload expansion,
with no call to the accepted Hidden wrapper at `80094F88`. The minimap's sprite
is in t0 and is explicitly saved at **40(sp)** around calls; the marker uses t1
at **120(sp)**. These offsets equal their frames: they are incoming argument
home slots, not an extra local array and not out-of-frame corruption. Both
callbacks save only ra in their prologues. Native minimap additionally uses ra
as a temporary global-base register; the marker uses ra as a decoded selector
and spills that value at 108(sp) while its true return address remains at 28.

Thus the native register geometry does **not** require pretending that Input
preserves t0/t1 or adding hidden arguments. It explicitly preserves those live
values at calls. The difference from the marker O2 baseline is allocation:
that source saves s0 as a persistent sprite register, moving ra's save to 36
while retaining frame 120. The real-helper O3 control removes that persistent
save but changes the frame to 112 and still has a broad instruction residual.
These observations constrain a future explanation; they do not identify the
original compiler's exact inlining policy or prove which source expression
will match. The prior audit already tested Hidden-only, actual Input, and all
four actual callee contexts. Repeating those broad controls is not justified.

The marker's two projection calls have genuine fifth-argument stack slots,
and its screen result is consumed at 88/90(sp), as documented in the archived
ABI report. A full liveness or projection-semantic proof is outside this narrow
structural extractor. The minimap's final Hidden path returns the signed byte
reloaded after Input, rather than an unconditional 1. It is unsafe to propagate
the marker's constant-one result to this related callback.

## Reproduction and evidence scope

Fetch the public archive commit, use the approved IDO 5.3 toolchain, then run:

```
git fetch origin pull/9/head:refs/remotes/origin/pr9
IDO_DIR=/path/to/approved/ido python3 cloud/work/d05_hud_context_audit/audit.py --replay --output /tmp/d05-evidence.json
python3 -m pytest -q tests/conveyor/test_d05_hud_context_audit.py
```

`evidence.json` records source/target/tool/object SHA256 values, exact flags,
full extents, calls and stack-access metadata, manifest checks, and the fresh
canonical verdicts. Objects and native disassembly remain ignored under
`build/`. The script pins the archive commit even if PR9 advances. Without
`--replay`, it audits current protected target geometry and pinned archive
availability only. The tests need no archive fetch or compiler: they check
protected current targets and the supplied receipt metadata. They are seven
structural/sentinel checks, **not** full callback gameplay tests or compiler
acceptance gates. No raw native instruction array is published by this packet.

Complete historical sources stay at these pinned paths rather than being
repackaged as new reconstruction:

- [Marker baseline](https://github.com/cabi24/SFRush2049-decomp/blob/433611408270ede7fe6acdb027f1b82179209511/cloud/work/large_hud_marker/reconstruction/baseline.c)
- [Actual Input group](https://github.com/cabi24/SFRush2049-decomp/blob/433611408270ede7fe6acdb027f1b82179209511/cloud/work/large_hud_marker/compiler/actual_input_group/group.c)
- [Historical ABI controls](https://github.com/cabi24/SFRush2049-decomp/blob/433611408270ede7fe6acdb027f1b82179209511/cloud/work/large_hud_marker/compiler/ABI_REPORT.md)

## Continuation recommendation

Freeze both callbacks for matching work. A maintainer may publish the audited
interrupted minimap source and original receipts so a repo-only worker can
check actual source/receipt identity and map-offset hypothesis results. Until
then, no result for those controls is inferred. Any future reconstruction must
preserve signed slot mapping and the final reload-derived return. Reopen the
marker only with a specific source/inlining explanation consistent with its
native home-slot spills and the already failed complete helper controls.
Move execution effort to a separately preflighted HUD/render packet rather
than spend another budget on the same O2/O3 sweep.

## Independent review

Lane F independently reran both pinned compiler controls and obtained an exact
JSON match, including object/tool/source hashes. All seven focused tests passed.
The reviewer approved this as a bounded research audit, requested a direct path
to the older signedness-defective seed, and that documentation fix is included.
This is independent agent review, not a maintainer integration approval.

Local validation also passed the full `tests/conveyor` suite with
`-m 'not node_required'` after initializing the existing test submodules and
providing MIPS binutils. The standard scorer sanity checks reproduced all four
existing accepted bodies (single plus three-member group). Initial aggregate
attempts without those dependencies failed and were not counted as passes.
