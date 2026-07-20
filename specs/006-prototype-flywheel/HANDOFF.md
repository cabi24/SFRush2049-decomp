# Handoff: 006 close-out + 007 launch (written at the Fable gate, 2026-07-19)

Fable access ends tonight. This documents exactly what remains, in order,
with the routing that works without Fable: **Codex executes against the
contracts; an Opus session takes the review gates** (each gate has
machine-checkable oracles — use them, then read the diff).

## 1. Finish 006 (branch `006-prototype-flywheel`)

State: T001–T011 + T013 done and committed. The T012 flywheel window is
RUNNING: 23 searches at priority 60 on watchman (2 DONE at last check), a
watcher (`b92k6mnls`-style loop, 30-min polls) fires on scored==compiled.
Known history to record honestly in quickstart §4 actuals:

- The daemon's first flywheel cycles died on a transient coordinator
  `RemoteDisconnected` (2×) and submitted nothing; the cycle was primed
  manually 2026-07-19 through the same code path (23/23 submitted).
- The agent then lost the coordinator path for ~10 h overnight
  (watchman flakiness), retry-looped as designed, self-healed ~23:51 UTC.
- 3 transient `sqlite3 database is locked` errors in the coordinator log
  (concurrent local scripts).

Remaining steps:
1. When the window drains: write §4 actuals (scored==compiled==60 minus
   any target whose seed no longer compiles; include the history above),
   verify SC-006 (`promotion_record` zero extracted) and SC-005 (queue
   lease timestamps — no static job displaced).
2. **Two hardening fixes** (small, Codex-able, single commit): (a) wrap
   the farm flywheel cycle's coordinator calls in a transient-error retry
   (one retry, then log-and-skip-cycle — never crash the daemon loop);
   (b) set a SQLite busy_timeout (~5 s) on the coordinator's connection.
   Unit-test the retry path.
3. T014: full suite + quickstart walkthrough; fill every actuals
   placeholder.
4. Close-out scorecard in quickstart or a CLOSEOUT.md: SC-002/003/004/
   005/006 met; **SC-001 NOT MET** (60 < 200, structural — see
   research/t009-shortfall.md both sections). This is the honest result;
   do not massage it.
5. Merge `006-prototype-flywheel` → master (fast-forward if master hasn't
   moved). Note: master also carries ~35 pre-006 unpushed commits — push
   is the operator's call.

## 2. Run 007 (branch `007-population-closure`, complete package committed)

The full spec-kit package is on the branch: spec, plan (research inlined),
contracts/closure-and-datasyms.md (normative, with unit-test obligations
and acceptance oracles), tasks.md (11 tasks, routing note included),
checklist. Rebase the branch onto master after 006 merges.

Execution: Codex per phase (T002–T004 closure; T005–T007 datasyms;
T008–T009 re-measure + window), Opus gate after T004 and after T008 using
the contract oracles (closure idempotency = second run registers zero;
datasyms byte-stability; `x<addr>` and in-blob `func_` class elimination;
SC-004's ≥200-or-stop-rule). The prompts follow the established pattern —
see `rush2049:flow` on the wiki and the prompt files under the 005/006
scratchpad history; every prompt must carry: read-the-contract-first, the
stop rule, "never bypass the pre-commit hook", and `< /dev/null` on
`codex exec` in background shells.

## 3. Known residuals (do not chase inside 006/007)

- m2c capability classes behind `partial_decomp` (~281): "Read from unset
  register" (register-arg conventions), stack-arg detection, `cfc1`,
  jump tables (`jr $t9`), expression merging. A future feature may
  vendor-patch m2c; needs its own spec.
- Track A backlog (independent of Track B): 19 `reloc_only_diff`
  adoptions via symbol reconciliation; `osPiRawReadWord`/
  `osViGetFramebuffer` near-miss closes; `__osAiDeviceBusy` boundary;
  CC50 rename (watchman-gated). See `docs/SYMBOL_MISATTRIBUTION.md` and
  the improvements wiki page.

## 4. Operational cautions (learned this cycle)

- `codex exec` hangs on stdin in background shells — always `< /dev/null`.
- A running farm daemon does NOT pick up new code — restart it after
  merging pipeline changes (this bit us once).
- Scoped histogram runs write `build/m2c_probe.*`; only full-population
  runs touch the instrument. The flywheel refuses non-complete inputs.
- Watchman drops off the network under container load; the agent
  self-heals — check `~/.conveyor_agent.log` before assuming worse.
- Commit `work/**` evidence promptly; the pre-commit hook guards the 22
  locked functions — a hook failure means evidence broke: stop.
