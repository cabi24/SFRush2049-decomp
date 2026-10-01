# Independent C11 flag/type reconciliation — frozen

2026-10-01, read-only integration audit. No shared headers, source, locks, layout or state writes. Private Rocky A compiles against a current shared-header snapshot. All evidence below is hashes/counts/source only; target/candidate objects remain private.

## Existing neighbor bodies also match O1

Actual accepted source definitions were extracted unchanged, including current rom_tu.h headers. Compiled with exact `-g0 -O1 -mips2 -G 0 -non_shared`, include paths and _LANGUAGE_C. Independently stack-sensitive scored against current authoritative objects exported read-only from n64_target/BlobStore; no fallback or target normalization.

| Accepted body | Existing pin | O1 strict/raw | Words | Unchanged normalized lock hash |
|---|---|---|---|---|
| dll_get_priority (lib_cc50) |O2|0/0|8/8|2c12a312d5e992cd9ae033704f757fb22d27d69d5741ef129dccfb35e47be5ad|
| osPfsReAllocate (lib_c990) |O2|0/0|8/8|bc7dd30bd4e3cc59f08b7c806e66e98446cc8ca018cb942b81a68fcd95799d7f|

Priority target SHA256 is `1c220b7382b3db545dcfabba77fd3e4569248a6bdc5f2cdb2ffe4b84e89de9d3`, the root's supported refreshed relocation-aware object with no gate/fallback. Thread-ID target is `54aa6e159bd6a9c509f6b4fba50ee7d0706f85ad89c1a5a6ad67339ab7ea6e88`. proof/neighbors.json preserves complete original lock provenance and independent results. Both compiler exit codes 0, stderr empty. Existing body hashes were asserted equal before compile and were not changed.

This supplies legitimate O1 byte evidence for the accepted neighbors. Root can reconcile flag evidence while preserving body hashes/original verification provenance, then derive/write opt overrides AFTER lock updates and perform full-ROM acceptance. Worker did not change pins or claim that the resulting combined TU gate has already passed. It can unlock the otherwise flag-blocked C11 dll_insert/dll_update and osPfsChecker_full without compiling accepted neighbors at an unproven level.

## Timer-head type audit

Only accepted static body using __osTimerList/__OSTimerNode is osSetTimer in lib_efd0. Its current source explicitly casts `((OSTimer *)__osTimerList)->next`, and its unchanged normalized lock hash is `7ebbfdde607de07818a4bcec8dee278b3ff6af8187e45d4f1a8a11383b87f491`. proof/timer_locked_uses.json retains that actual source definition.

__OSTimerNode remains a 32-byte split-word overlay (next/prev, reload_hi/lo, delta_hi/lo, queue/message). Canonical OSTimer has identical field storage offsets: interval8, value16, queue24, message28; u64 members require the canonical alignment. The proposed minimal change is ONLY replacing `extern __OSTimerNode *__osTimerList;` in m2c_types.h with `extern OSTimer *__osTimerList;`. Keep the old overlay type definition available; existing explicit casts remain valid. The private adapted rom_tu.h adds `extern u32 __osTimerCounter;` and `extern u32 osGetCount(void);`, both absent from the current shared context. Explicitly typed osGetCount preserves exact timer bytes and all 63 existing promoted TU texts.

Privately compiled every currently promoted ROM TU before versus after this type/context change under existing opt_overrides.mk flags: **63/63 compile and every complete C .text is raw-identical**. No shared writes. proof/timer_header_regression.json gives all names/counts/equality; header_snapshot_sha256.json identifies baseline headers. GLOBAL_ASM passthrough parts are omitted by direct-C compile, so this does not replace root's final placement/ROM gate.

All three additional C11 bodies extracted unchanged from the frozen packet independently compile against the adapted shared headers with strict/raw 0: dll_insert 100/100 words, dll_update 96/96 words, and osPfsChecker_full 48/48 words. The historical checker label implements void osStopThread(OSThread*); its definition compiles with existing thread/cleanup/list prototypes, without a new conflicting public declaration. Their actual definitions, prototypes and symbol names compile without a full standalone prelude. proof/timers.json records these independent outcomes.

Reproduce in `Rocky:~/agents/A/scratch/codex_A11/` from private checkout:

```sh
cd ~/agents/A/wt
python3 ~/agents/A/scratch/codex_A11/verify.py
python3 ~/agents/A/scratch/codex_A11/compare.py
```

Private source/target objects and timer compile sources remain there. Artifact reproducer scripts cover neighbor O1 verification and baseline/adapted promoted-TU comparison. Header/body checks supplement required root lock and full-ROM gates.
