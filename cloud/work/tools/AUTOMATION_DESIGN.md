# Automation design (shared spec for the tool packages)

Goal: move the matching loop from LLM calls to deterministic local compute. LLMs are for the residue: a
function the machine could not finish, presented as a compact diff report. Everything here is **stdlib-only
Python 3.9+**, runs from the repo root, needs only IDO (`tools/cloud/setup.sh`) and the committed
`asm/us/blob` targets, and prints machine-readable JSON with `--json`. Nothing edits protected paths.

## Packages (all under `cloud/work/tools/`)

### `amatch/` — the matching engine
| module | job |
|---|---|
| `aligned.py` | fast word-sequence scoring: strict positional diff, **instruction-aligned** exact/opcode/opcode+reg counts (bit-parallel LCS), per-hunk diff. Pure functions over `list[int]`. |
| `builder.py` | in-process compile + score reusing `tools/cloud/score.py` internals: `compile_fn(src, fn, flags)` and `compile_group(dir_or_spec, overrides)`; content-hash cache on disk; returns words, size, per-member strict + aligned scores, unresolved-symbol list. |
| `mutate.py` | the **mutation catalog**: deterministic source transforms encoding the recurring fixes (see below). `mutations(src, fn) -> [(name, new_src)]`, each tagged with a cost and the diff-shape it targets. |
| `search.py` | hill-climb / beam / annealing over mutation sequences with a parallel worker pool; objective = aligned exact, then strict diff; stops on strict MATCH or budget; writes a JSON log and the best source. |
| `corpus.py` | regression corpus mined from git history: (first-attempt source, matching source) pairs for every function matched so far; `bench` runs search from the start state and reports auto-match rate and cost. |
| `probe.py` | allocation probe matrix for a group: vary keep membership, callee parameter count, return type, inlining, stand-in call sites; record emitted register usage per member. |
| `report.py` | residual report for the LLM: for an unmatched result, print only the aligned diff hunks, frame/register summary and which mutations were already tried. |
| `autopilot.py` | batch driver: candidate list -> seed -> build -> search -> report, resumable, N jobs; emits a worklist JSON of what needs a human/LLM. |

### `ipakit/` — IPA analysis (static, from the retail words)
| module | job |
|---|---|
| `mipsdec.py` | minimal MIPS-III decoder: per word, opcode/mnemonic, GPR/FPR defs and uses, branch/jal target, delay-slot aware. |
| `analyze.py` | per function: entry-read registers (args, incl. non-ABI), callee-saved usage vs save/restore (unsaved `s*`), per-call-site **registers live across each `jal`**, return-register use, call targets with alternate-entry (`callee+4`) resolution, unresolved targets. |
| `clobber.py` | per function and transitive: registers a callee may write (minus those it saves/restores). |
| `deps.py` | **register-specific dependency edges**: for each register a caller keeps live across a call, the callee clobber set that explains it; minimal closure = functions needed to explain every IPA evidence; classification ABI / IPA-leaf / IPA-caller with evidence lines. |
| `heads.py` | function-head audit: heads inside opaque runs (prologue after `jr ra` delay slot), missing symbols, alternate entries. |
| `sigs.py` | parameter inference: home-slot stores (`sw aN,off(sp)`, `sw t0..`), width from following `sll/sra`/`andi`, float vs int from `swc1`, order from slot offset (not register name); caller setup cross-check. |
| `groupgen.py` | emit `cloud/work/ipa-groups/<name>/` (group.json, skeleton group.c with inferred signatures, stand-in callers, keep list) from `deps.py`; root vs helper distinction (address-taken or external callers = keep). |

## Interfaces everyone must follow
- Word lists are `list[int]` (big-endian 32-bit words). Targets come from `tools/cloud/score.py` `targets()`; heads not in the blob come from the inflated image as `extscore.py` does.
- Score dict keys (always these names): `strict_diff` (int), `size`, `target_size`, `extra`, `aligned_exact`, `aligned_opcode`, `aligned_opcode_reg`, `target_words`, `matched` (bool, strict and no unresolved/unverified problems), `unverified` (list).
- Every CLI: `python3 cloud/work/tools/<pkg>/<module>.py ... [--json]`; exit 0 on success of the tool, not on "matched".
- Flags default `-g0 -O2 -mips2 -G 0 -non_shared`; score.py adds `-r4300_mul` itself. Group flags come from group.json.
- Temp work in `tempfile`, cache under `build/amatch_cache/` (gitignored if not already; do not commit it).
- Tests: `tests/cloud/test_<module>.py`, runnable with `python3 -m unittest`; tests needing IDO skip cleanly if `tools/cloud/ido/cc` is missing.

## Mutation catalog (from this session; see cloud/PLAYBOOK.md)
Loop form `do/while` <-> `for` <-> goto; counter type `s16`<->`s32`; named local drop / inline (pointer and flag locals); declaration-order permutation;
unused pad local / `volatile s32 pad[n]` for frame size; `extern` <-> defined symbol (shared `lui at`); `(u32)` pointer laundering; K&R callee prototype;
parameter `s16/u8` <-> `s32`; shared-exit `result =` form; `x *= 2` <-> `x <<= 1`; commutative operand flips (`a+b`/`b+a`, `a*b`); int <-> float literals
(`0` / `0.0f`, `1/len`); `&&`/`||` chains <-> nested `if`; `if/else` swap with negated condition; scale-then-add FP idiom; switch case order permutation;
statement-order permutation of independent statements; `-O1/-O2/-O3` flag sweep; group-level: keep membership, stand-in call-site count, callee parameter count.

## Rules
- Deterministic: every search takes `--seed`; same inputs give the same log.
- Never claim a match without the strict scorer; `search.py` re-verifies the final source with `score.py fn|group`.
- LLM residue stays small: `report.py` output is the only thing a model should read for an unfinished function.
