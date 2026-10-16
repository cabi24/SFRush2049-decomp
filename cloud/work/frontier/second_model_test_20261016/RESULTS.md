# Second-model drafting test (DeepSeek V4.1 Flash via OpenCode; local qwen3.8:27b fallback), 2026-10-16

Owner request: try a single matching task on DeepSeek V4.1 Flash through OpenCode on nvmepi1 (192.168.50.58),
with `watchman2/qwen3.8:27b` as the fallback. Rules: never inside the real repo; output is a draft until the
IDO pipeline confirms it; send only the asm and context needed; stop after a few failed attempts and record.

**Outcome: no usable draft from either model. Nine attempts, zero lines of compilable C. Recorded here; not retried.**

## Target and baseline

- `entity_physics_update` (0x800908A0, 712 B, 178 words; GC `AnimateBlast`). Chosen because it is small, has a
  complete near-miss (PR #379 `blast/effects.c`, colour-only residual) and a known baseline.
- Baseline reproduced first on the Pi in the whole-program unit (the same scorer a second-model draft would face):
  `blob_unit --tag dsk score entity_physics_update --with .../blast/effects.c --internal func_80090308 --neighbours`
  → `FAIL entity_physics_update: 21 of 178 words differ`, 0 locked bodies differ.
- Inputs sent: `build/m2c_asm/entity_physics_update.s` (357 lines, 6.5 KB) and a 36-line `context.h` holding only
  the typedefs/externs/prototypes the function needs (lines 33–68 of effects.c). Prompt ≈ 9.3 KB total.
  Nothing else from the repo was sent by me.

## DeepSeek V4.1 Flash (`opencode/deepseek-v4.1-flash-free`, OpenCode 1.19.2 on nvmepi1)

OpenCode is not on the non-login PATH: use `export PATH=$HOME/.local/bin:$PATH`. `opencode models` lists the model;
a smoke prompt ("reply OK") answers correctly in seconds. Scratch dir `~/scratch/rush2049/entity_physics_update/`.

| # | Invocation | Result |
|---|---|---|
| 1 | `opencode run -m MODEL -f asm -f ctx "prompt"` | `-f` is variadic and swallowed the prompt as a file name: "File not found". Prompt must come BEFORE `-f`. |
| 2 | prompt first, `-f asm -f ctx` (default `build` agent) | The agent ignored the task and probed the host for a MIPS compiler: `sudo -n true`, `ls /proc/sys/fs/binfmt_misc`; permission auto-rejected; exited with no text. **During this run it `git clone`d the public GitHub repo into `/tmp/opencode/SFRush2049-decomp` on nvmepi1** (timestamp 23:22:59, inside the run window) and grepped it. Left in place for the owner to see. |
| 3 | same + explicit "do NOT run commands / compile / read files" | rc=0 in ~1 min, empty stdout (only the `> build · deepseek…` header). |
| 4 | same with `--format json` | Hung 15 min, 0 bytes, timeout. |
| 5 | asm + header INLINED in the prompt (no `-f`), default agent | Ran shell commands again (`ls`, `grep -rln func_80090308` over the /tmp clone, read its own log files); 10-min timeout, no C. |
| 6 | custom tool-less agent `.opencode/agent/draft.md` (`tools: bash/read/write/edit/glob/grep/list/patch/webfetch/todo*/task: false`, `mode: primary`), inlined prompt | Agent is recognised (`opencode agent list` shows `draft`); hung 10 min, 0 bytes. |
| 7 | built-in read-only `--agent plan`, inlined prompt | Hung 7 min, 0 bytes. |

Conclusion for DeepSeek via OpenCode: the free-tier model inside OpenCode's agent loop either goes tool-hunting,
returns an empty text part, or stalls on a ~9 KB prompt. The model itself answers trivial prompts. Not usable
for this task in this form; a direct chat-completion call (no agent layer) was not possible because the free tier
is reachable only through OpenCode's own auth.

## Local fallback `qwen3.8:27b` (ollama on watchman2)

| # | Path | Result |
|---|---|---|
| 8 | OpenCode, tool-less `draft` agent, inlined prompt | rc=0 in seconds, empty text (header only). The `--format json` rerun hung 15 min with 0 bytes — the same as DeepSeek's run 4, so `--format json` itself stalls in OpenCode 1.19.2 here regardless of model; do not use it for diagnosis. |
| 9a | Direct `POST watchman2:11434/api/chat`, think=false, num_ctx 16384, temp 0.2, num_predict 4096 | 54 s, 4096 tokens: degenerate — `s16 id; s16 state; s16 count; s16 temp1; … s16 temp416;` and no body before the budget ran out. |
| 9b | Direct, think=true, repeat_penalty 1.15, num_predict 8192 | 197 s: 21,418 characters of thinking, 0 characters of answer (budget exhausted while reasoning). |

## What to change before trying again (not done)

- Put the prompt before `-f`, or inline the files. Use a tool-less agent definition (`.opencode/agent/*.md`) in
  the scratch dir, never the default `build` agent: it will run shell commands and reach the network.
- Expect the default agent to clone the public repo; if that matters, run on a host without network access to
  GitHub, or in a sandbox. The scratch dir alone is not a boundary.
- Prompts of ~9 KB stalled the free tier; try a much smaller target (≤ 60 words of asm) to find out whether size
  or the agent loop is the cause. Ask for the answer inside a file-less `<c>…</c>` block and parse it.
- For the local model, cap thinking (or disable it) and set `num_predict` well above the expected output; the
  27B model at Q4 did not produce a body in either mode.
- Score any draft only with `blob_unit --tag <lane> score FN --with draft.c --neighbours`; the 21/178 baseline
  is the number to beat for this target.

Files: scratch `/tmp/claude-…/scratchpad/dsk/entity_physics_update/` on the Pi (prompt.txt, qwen_direct*.c);
nvmepi1 `~/scratch/rush2049/entity_physics_update/` (run1.err … run6.out, `.opencode/agent/draft.md`) and
`~/scratch/rush2049/epu_qwen/`.
