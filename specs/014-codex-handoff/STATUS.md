# Coordinator status (Codex maintains this; newest entry first)

## 2026-10-01 handoff (Claude coordinating session)

- Game code: 497/1,210 functions, 56,416/647,072 bytes (8.72%). Static code: 23/230 functions, 1,448/61,440 bytes (2.36%). ROM SHA-1 exact.
- Done this session: group search job + farm automation (on), worker-leak fix, 45 heads registered (OpenAI lane 1), vendored workbench,
  pilot of 18 near-misses (7 matched), `diagnose` pipeline step accepted, all cloud PRs (#1-#6) merged.
- Open: lanes 1-5 in PROMPT.md. Uncommitted in the tree at handoff: nothing relevant (check `git status`).
- Known blockers: the 7 `tests/cloud/test_ipakit_*` failures (cloud tests assume unregistered heads); `func_800BB7F4`/`render_post_process` refused by
  the image gate; stand-in groups are not spliceable.
