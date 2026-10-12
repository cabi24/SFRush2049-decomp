# Image-B stunt/score callback trace

Reserved on 2026-10-12 from master 3bec9ce30. Baseline source is the existing
lean runtime_b_stunt_hud_20261006 candidate. Stock IDO 5.3 O3 baseline reproduced:
28/468 words differ, no extra words, unresolved symbols, unverified references
or own-data errors. New source currently has 2/468 differing words under the
same strict authenticated image-B context. This is NONMATCH, zero claims and
zero new accepted/image/ROM bytes. Existing accepted overlays remain untouched.

The native pointer callback suppresses/prints stunt counters, scores and timed
multiplier messages. Trace fixes: separate pending-message x/y locals produce
native s0/s1 versus the main x/y s2/s3; naming the existing pending count restores
the two native spill homes; table-first coordinate addition restores operand
order. Declaring the unchanged 32-byte text buffer first places it at the top of
the locals region. Two explicitly unrecovered unused s32 locals close exactly
the remaining 8-byte declared-local residual (176 -> 184 bytes), per the Wave 16
rule. These are disclosed shaping devices, not recovered original variable names.
Text capacity is neither enlarged nor asserted safe for every runtime string.

Current exact residual: offsets +0x398 and +0x39c contain the sentinel constant
and pending-count store in opposite order. Workbench diagnosis and retained
stock ugen listing were read before source changes. The only code residual is
that schedule pair; constants, arithmetic, registers, frame and stack homes now
match. All 86 relocation sites resolve and the 2.69f owned literal is verified.
No modified compiler or scorer supplies match evidence.

Rejected bounded controls: reversing the two memory stores requires an alias
reload and grows the body; a named countdown carried through all arms likewise
grows the body; decrementing countdown or the named pending variable changes
register webs broadly. A same-line/comma spelling leaves the pair unchanged.
These remain local diagnostics and are not matching deliverables.

## Compact replay and limitations

Run `python3 cloud/work/frontier/dot_runtime_b_stunt_callback_20261012/verify.py`.
The current-repository asset is authenticated in memory; no historical-commit
lookup is needed. Sources/verifier, selected target words/extent and inherited
baseline source are bound. Shared tooling, lock files and generated asm text
are deliberately not pinned. This is image B under stock recognized IDO 5.3;
no static ROM TU is rebuilt, so no per-TU compiler override is involved.

Exact current scorer output (`verify.py --score`):

```
func_80392894: 2/468 words differ
own .rodata verified at 0x80394F50..0x80394F54
```

The independent GNU link preserves the whole 1,872-byte function and `.text`,
all 86 relocations, the 4-byte literal and its 12 zero alignment bytes. There are
no extra function bytes, unresolved symbols, remaining relocations or unknown
allocated linked sections. GNU finds exactly the same two offsets and checks
that they are precisely the reverse-order pair, rather than arbitrary errors.
The no-unused-locals control remains 10/468; the inherited baseline remains 28/468.
Missing/corrupt own-data controls are rejected by distinct verifier fields.
A one-function canonical whole-program group control also remained 2/468;
no original TU or new helper context is claimed from that control.

The 32-byte buffer declaration is retained from the baseline. The native frame
has 32 bytes from its address at sp+152 to frame end 184, which is a placement
constraint rather than an independent proof of the original declaration or
unrestricted formatter safety. Same-buffer input/output remains a game-specific
formatter assumption. Player/table bounds, representable alpha and valid string
pointers retain the baseline's documented domain. No host/gameplay behavioral
proof, private replacement oracle, image splice, compression or ROM hash is
claimed. All nine helper boundaries remain external; no caller/body/ABI closure
is claimed, and historical generic prototypes elsewhere are not authoritative.

The alternate 9244C was only inspected. Its existing packet explicitly freezes
ordinary compiler work pending authentic alias/TU evidence, so it was released
without a new matching experiment or publication claim.

## Validation on the publication tree

- Packet replay passed normally and with Python optimization.
- One focused collected test passed with `REQUIRE_TOOLCHAIN=1`; one clean skip
  with IDO deliberately unavailable. The actual test is well below one second
  locally, within protocol-v2 budgets.
- `git diff --check` passed. No protected paths, accepted source or lock changes.
- Native dump/object/intermediate diagnostics remain untracked local build data
  and are excluded from the six-file source/test/research payload.
