# Small registered heads — worker B (2026-10-01)

Two strict matches delivered, awaiting coordinator image/ROM gates. No integration,
commits, lock/layout edits, or pushes performed. Private Rocky scratch remains at
`~/agents/B/scratch/codex_B`. Compilation serial, at most one compute process.
All three were absent from the game lock at start. Read current derived target asm
`build/m2c_asm/<fn>.s`, registered-head seeds/scores, and arcade checkpoint source.
The arcade source supplies subsystem context, not exact N64 logic for these heads;
all corrections below are grounded directly in N64 target assembly.

Target objects exported read-only from n64_target.target_o_sha in conveyor.db:
- C7F4: d43a92d2d62448ce6fd92f1867579846af518f73c4d679e77be4e244cc0c1e89
- 5DA8: c620125271234d229335ed2341778c267e765419194446901a5e45f1fe49b8e8
- C2E4: d0378992a54f0e395f87b206f4a3695f4f378289730dcf4e4862d9d4df73968a

Flags: `-g0 -O2 -mips2 -G 0 -non_shared`; strict scorer adds r4300_mul.

## func_8010C7F4 — MATCH, 96 words (384 bytes)

Deliverable `cloud/matches/func_8010C7F4.c` has the exact flags line; that exact
file was copied to Rocky and independently rescored with score.py fn: bare MATCH.

Original registered-head seed: 93/96 at O2, 78/96 at O3. Its guessed scalar output
was read as integer; stat data was read as byte instead of word, and it passed a
redundant fourth call argument. Hand-written natural vector rewrite uses delta[3]
and local[3], local[2] float comparison, signed status-byte check, word stat load,
and three actual call arguments: 13/96. Dropping named index: 10/96, all remaining
words frame/homes. Moving car declaration after vectors: 4/96, but wrong car home.
Array-order/same-line probes remained 10. Unused pad at end yielded 8/96.

Final **unused s32 pad before car** yields target frame72, car64, delta52..60,
local40..48, and strict MATCH. This is a compile-affecting quirk; mention unused
padding in the integration commit. No uninitialized value is read. About ten
measured variants after the original seed, plus exact-file acceptance rescore.
Workbench correctly distinguished homes from allocation once the semantic errors
were fixed; no register mutation was needed.

## func_80105DA8 — MATCH, 64 words (256 bytes)

Deliverable `cloud/matches/func_80105DA8.c`, exact flags line, exact delivered file
rescored with score.py fn: bare MATCH.

Seed 35/64; diagnose identified structural width errors and temp advance. Assembly
requires signed halfword lookup in D_8014AA16 and signed byte lookup in D_80152907;
seed read both unsigned bytes. Also rewrite `(index < bound) == 0` to natural >=,
which causes assembler compare to use at rather than consume a temp. These fixes
produce 1/64: only symmetric beq operands are reversed.

Directly reversing source comparison operands, five var types, boolean arithmetic,
four constant-carrier shapes, comma and assignment condition probes do not close
that word. The successful correction **removes pass-through var_v1**, comparing
and returning the actual byte field directly. Both spelling orders of the direct
comparison match. Chose the simpler direct-field original order; no padding quirk.
Roughly 23 bounded variants. Workbench calls the lone word allocation/coalescing,
but both pool/temp lanes are identical: it is a copy-carrier/commutative operand
shape, and direct field access fixes it. This is another example of not taking
its forced-color recommendation literally.

## func_8010C2E4 — NONMATCH, best 3/89 words

Best whole TU: `cloud/work/new_heads_B/func_8010C2E4_best.c` (NONMATCH).
Original registered seed 89/89: it omitted an indirection through arg0+0x6C.
Adding nested dereference produced 85/89. Reusing arg1 as the first pointer carrier
rather than hiding both dereferences in one expression reproduces the target
argument-register use and removes the extra argument home: 24/89. Replacing the
pass-through temp_t9 update with `var_v1 |= 0x200; flags=var_v1` produces 7/89.
Typed 24-byte EdgeFlags table indexing instead of byte arithmetic fixes all four
commutative pointer-add operand pairs: **3/89**.

The remaining three strict words are exactly:
- target stores t9 to flags before move v1,t9; candidate moves before store (2 words)
- final target or t9,v1,t8; candidate or t9,t8,v1 (1 word)

All pool/temp lanes now identical, instruction counts identical, prefix exact
through word71. Integer/pointer operand reversals, flags widths, nested assignments,
local next-value carrier, direct field reloads, direct/typed compound stores,
flag-condition variants, and typed payload struct do not close it. Compound/direct
updates often destroy correct caching or web population (12-29 words); source
operand reversal is inert. About 32 bounded variants. Stopped below 40 instead of
repeating the now-known commutative spellings. No match claimed. Workbench identifies
schedule/commutation correctly; its generic forced-color or line suggestions alone
are insufficient evidence that a new source probe will reach this residual.

Image/ROM gates are coordinator work; none were run by this worker.
