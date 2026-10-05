# Frontier wave 5 — agent w5d (pre-colouring trace) results

Builder scratch: `~/rush2049/scratch/frontier/w5d` (copied from `base` on 2026-10-05). Unit tag: `w5d`.
Nothing was committed, spliced, or copied to `cloud/matches/`. func_80087110 was not touched.

**Summary.**
- **Tool:** a w5d build of IDO 5.3 uopt traces the decisions made before register colouring:
  PRE/CSE, loop-invariant hoisting, and live ranges for constants and addresses. Its output is
  byte-identical to the stock uopt with tracing off and with tracing on, checked on two whole-unit
  snapshots. It also adds a PRE oracle, `W5D_NOPRE`. Details are in `tools/README.md`.
- **audio_mixer_main:** the oracle shows that the entire 31-word residual is one PRE decision. If uopt
  does not delete the redundant `k + 1` occurrences, the output equals retail (0 of 183 rows differ).
  No natural source spelling that produces this was found: about 20 traced variants, listed below.
- **net_state_validate:** both lanes are now explained as specific pre-colouring decisions (lane B is a
  PRE loop-invariant hoist; the "regime flip" is the sign of a split piece's save). It is not closed.
- **No new matches.** `audio_priority_find` stays provisional.

## 1. The tool

`tools/` contains `build_uopt.sh`, `instrument_w5d.py`, `run_fidelity.sh`, `pretrace.sh`, `prereport.py`,
`precmp.py`, `vrun.sh`, `nopre.sh`, `force.sh` and `cmparse.py`. `tools/README.md` gives usage and the
uopt data layouts found.

- **uopt has its own dumps.** Stock IDO 5.3 uopt contains listing routines (`printitab`, `printcm`,
  `printprecm`, `printhoist`, …). They are switched on by an undocumented debug level, `-zdbug:N`, and
  written to `-l FILE`. The stock binary aborts at the end of the listing, because the recompiled libc's
  `ecvt` asserts. The w5d build fixes that and routes every read of the debug level through a hook, so
  `W5D_LEVEL=3 W5D_PROC=name` dumps one procedure in about 4 s.
  - Level 3 gives, for every basic block and expression bit: antloc, avloc, alters, antin, antout, ppin,
    ppout, insert, delete, subdelete, subinsert, the IV and strength-reduction candidate sets, and the
    register-candidate sets (`@ iscolored 1/2`).
- **Added hooks:**
  - Every expression bit is rendered as text, with variables, offsets, addresses and constants. This is
    done after code motion and at globalcolor entry. **Bit number = web number in the w3a colouring
    trace**, so a web can now be read as an expression.
  - Every constant or address operand that register-allocation preparation tests for a live range
    (`makelivranges` → `constinreg`/`ldainreg`) is logged with its block and decision.
  - The procedure's colouring ordinal is printed, and the CDX trace is restricted by name.
- **Oracle `W5D_NOPRE=bits`:** clears bits from the delete vector after code motion and shows whether
  the code would then match. For colouring, w3a's `CDX_FORCE` already does this.
- **Fidelity.** Three runs on the same snapshot wrote identical `opt`: the stock toolkit uopt, the w5d
  uopt with tracing off, and the w5d uopt with tracing on (all procedures, level 3, `W5D_OUT`, `CDX_LOG`).
  - `st_fid`: `f6597ed9…01001ba99`
  - `st_nsv_best`: `a516afce…6130ce9b23`

## 2. What each phase does in IDO 5.3 uopt (as observed)

Order per procedure (from the listing): local optimisation → patchvectors → copy propagation → redundant
store removal → findinduct → code motion (PRE) → induction-variable elimination / strength reduction →
register-allocation preparation (makelivranges) → globalcolor → reemission.

1. **Local optimisation** hashes expressions into ichains (the bit table). Same operator, same operands
   and same dtype give one ichain, so one bit. cfe/uopt canonicalise:
   - `k + 1U`, `(unsigned)k + 1`, `1 + k`, `k - -1` and `next = k; next++` are all `add.J(k,1)` (traced);
   - loads at different offsets are different bits.
2. **Copy propagation** runs *before* PRE. `next = k + 1; if (next == …)` is propagated into the same
   expression, which is why "name the value" spellings never separate a CSE.
3. **Code motion is Chow/Morel–Renvoise PRE over the bit vectors.**
   - An occurrence is removed (`delete`) when the expression is available or placement-possible at the
     block's entry. The insert vector moves computations into predecessors, which is how loop-invariant
     loads are hoisted.
   - **Measured over the whole unit** (912 procedures, levels 3 + 25, `tools/redundancy_scan.py`): all
     3,675 arithmetic, address or load occurrences that were fully available at their block's entry were
     deleted. The only fully-redundant occurrences ever kept are comparisons (`neq`/`les`/`equ`/`geq`,
     93 cases).
   - **Lever:** a recomputation in retail never comes from "uopt chose not to CSE". It means the value
     was killed in between (store to an operand, a call when the operand is aliased or global), or the
     occurrences are different expressions.
4. **Strength reduction / IV:** `findinduct` marks IVs at the loop latch (`iv`). Linear address
   expressions (`mpy(k,80)`, `ixa(base, mpy(k,80))`) become `cand` and are taken out of PRE's local sets.
   `k + 1` is never a candidate. Two counters (`k`, `n = k + 1` both incremented) stay two IVs; uopt does
   not eliminate one in favour of the other here (traced, 42 words).
5. **Register-allocation preparation:**
   - **Every** address operand gets a live-range candidate. `ldainreg` returns 1 unless
     `-no_const_in_reg` is set or the use context is 4.
   - Constants get one when they do not fit the immediate field or are multiplied (context 5),
     `constinreg`.
   - Expression temporaries from PRE get webs too.
   - Whether a candidate becomes a register is then purely colouring.
   - An uncoloured constant or address range is **rematerialised** at each use (`lui`/`li`). An
     uncoloured *expression* temp is **spilled**: forcing `k + 1` to split gives `sw t9,68(sp)` /
     `lw`, not a recomputation.
6. **Colouring** (w3a): `save = totalsave/nocs`. When a range cannot be coloured whole it is split. A
   split piece is coloured only if its own `save` is positive.

## 3. Levers from C

| Decision | Lever (verified) |
|---|---|
| CSE of an expression across blocks | Only a kill or a different ucode expression prevents it. Casts, `U` suffixes, operand order and copies are canonicalised. Making the operand address-taken stops CSE across a call (the increment), but puts the variable in memory |
| Loop-invariant hoist of a global load (`insert` in preheader) | The load must be anticipated on every path from the loop entry. A read placed after a `continue` test is not hoisted (net_state_validate `j5.c`: `del=0 ins=0`) |
| Constant/address in a register vs `lui` at each use | Only colouring decides. The candidate always exists. Rematerialisation needs the range, or its split piece, to have `save ≤ 0` or to lose the contest |
| "Regime" flips (whole-function changes from one statement) | A big low-priority constant or address range is split. Whether the split piece's `save` is positive depends on the interference of unrelated webs (net_state_validate below) |

## 4. audio_mixer_main — 31/183, root cause proved, not closed

```
EXTRA="audio_priority_find --internal audio_priority_find --keep audio_mixer_main" \
  tools/pretrace.sh audio_mixer_main audio_mixer_main/base.c audio_mixer_main_base
  FAIL audio_mixer_main: 31 of 183 words differ          (base.c = w4b/w3b best)
  EQUAL audio_priority_find: 108 words (internal, c_base.c)
report row:
  80  add.J(varM(3437-36)r,1)
      antloc 29,31,40 | DELETE 31,40 | iscolored1 indmults | p1:w80 save=10.00 nocs=4 color->s2
```

The blocks are 29 (the test `k + 1 == count`), 31 (`next = k + 1`) and 40 (the increment `k++`). PRE
deletes the second and third occurrences. The temporary becomes web w80 in s2, and `k++` becomes
`move s0,s2`.

**Oracle (bit 80 left in place):**
```
tools/nopre.sh audio_mixer_main_base audio_mixer_main audio_mixer_main 80
want 183 words, got 183; differing rows 0
```
So the source only has to stop PRE from treating `k + 1` in blocks 31 and 40 as redundant. Every other
register already falls into place.

Because a fully available arithmetic occurrence is always deleted (section 2.3), the two retail
recomputations need one of two things:
- `k` is killed between the test and the arm and between the test and the increment; or
- the three `k + 1` are different ucode expressions.

**Traced variants** (in `audio_mixer_main/`, runs in `runs/audio_mixer_main_*`):

| Variant | Effect on `k + 1` | Unit result |
|---|---|---|
| `u_test` `k + 1U ==`, `u_else` `next = k + 1U`, `a_ucast` `(unsigned)k + 1`, `a_nextinc` `next = k; next++` | canonicalised to the same `add.J`: DELETE 31,40 | 31 |
| `a_notneg` `next = -~k` | a different expression in the arm (`neg(not k)`, not folded); the increment is still deleted | 59 |
| `a_kp1var` `(next = k + 1) == count` | one computation in the test; the increment is still deleted | 58 |
| `s16_test` `(s16)(k + 1) ==` | still `add.J` + cvtl; DELETE 31,40 | 60 |
| `k_s16` (s16 counter) | still one `add.J`; DELETE 31,40 | 61 |
| `v_deadptr` (`&k` taken), `v_stepfn` (inlined `step(&k)`), `v_nextfn` (inlined `next_of(&k)`) | `k` loses its register flag; the call kills `k + 1` at 40 (DELETE 31 only). `k` lives in memory | 57–63 |
| `iv_a`/`iv_b` (`n` incremented beside `k`), `iv_c` (`n = k + 1` copy), `iv_d` (loop on `n`, `k = n - 1`) | two IVs kept / copy-propagated / reversed | 42 / 42 / 31 / 59 |

- **Leads not yet tried:**
  - a store between the test and the arm that may alias `k`, *without* `k` living in memory. In the
    corpus an aliased local can still be promoted to a register after strength reduction; see w3a
    func_8008E26C;
  - a test operand of a different dtype that ugen emits as `addiu`, for example an address-typed add.
- **Next step:** compare against a matched function that recomputes `x + 1`. The whole-unit scan
  (`tools/redundancy_scan.py`, section 2.3) found none, which is consistent with the rule above. The
  provisional `audio_priority_find` stays EQUAL internal with this caller.

## 5. net_state_validate — 27/682, both lanes explained, not closed

```
tools/pretrace.sh net_state_validate net_state_validate/best.c nsv_best     # FAIL 27 of 682
tools/pretrace.sh net_state_validate net_state_validate/t1.c nsv_t1         # FAIL 474 (total = 0 moved)
tools/pretrace.sh net_state_validate net_state_validate/j5.c nsv_j5         # FAIL 126 (flag read after the test)
tools/precmp.py runs/nsv_best runs/nsv_t1 net_state_validate
```

**Lane B (the `D_80156994` read in the last loop) is a PRE hoist, not colouring.**
- Bit 321 is `ilod.J@0(ldaS(19+0))`, that is `D_80156994`. In best.c its pattern is
  `antloc 120,125 | DELETE 125 | INSERT 124`: the in-loop read is moved to the preheader (block 124).
  That is ours `lui a3; lb a3` before the loop.
- Retail reads it inside the loop after the data test. With `flag = D_80156994` after
  `if (data4 == 0) continue;` (j5.c) the load is no longer hoisted (`del=0 ins=0`). The read is
  anticipated only on the non-continue path, so lane B's load placement is right.
- But then the *address* gets a register range: `regcand lda ldaS(19+0)` decision 1 in blocks 120 and
  127, web w467, save 1.0, nocs 10 → t0. The cheat-branch read (block 120) and the loop read share one
  range. Retail rematerialises both, which needs that range's save ≤ 0.
- The oracle cannot remove an insertion (uopt segfaults), so the hoist itself cannot be tested by
  oracle.

**Lane A / the "regime flip" (t1.c, `total = 0` before the zero loop) is a split-piece sign change.**
- The row pointer moves to a1 as intended (w237).
- Two low-priority ranges get split in both builds: the record size `76` (bit 465, constant ×76, context
  5) and the `D_8014A118` table address `ldaS(412+0)` (bit 464). Both start at save 1.4, nocs 35.
  - In best.c the split pieces have **save −0.167 over 12 blocks**. They are never coloured and are
    rematerialised, which is retail's look.
  - In t1.c the pieces cover 13 blocks with **save +0.538**. They are coloured t1 and s2, and everything
    after them shifts. That is the 474-word regime.
- So retail needs both: `total` live before the zero loop (lane A) *and* a split boundary that leaves
  the 76/`D_8014A118` pieces with no positive saving. The next step is to read the split pieces' block
  sets (`CDX_DETAIL_WEB=464`), then move the statement whose block tips the piece from 12 to 13 blocks.

## 6. What generalises

1. **Ask the oracle before writing variants.** `nopre.sh` or `force.sh` tells in one run whether a
   pre-colouring decision is the whole residual. audio_mixer_main's 31 words are exactly one deletion
   vector.
2. **"Retail recomputes it" means the value was killed, or the occurrences are different expressions.**
   In 912 procedures uopt never kept a fully redundant arithmetic or load occurrence. Spellings that
   only rename or cast (U suffix, casts, operand order, temporaries) are canonicalised or
   copy-propagated before PRE and cannot work.
3. **Every address and most large constants get a register candidate.** "Retail never makes a candidate"
   really means the candidate's range, or its split piece, has `save ≤ 0`. That is controlled by the
   number of use blocks inside the range: move or merge uses, not the declaration.
4. **A loop-invariant global read is hoisted only if it is anticipated on every path from the loop
   entry.** Putting the read after a `continue` test keeps it in the loop.
5. Bit numbers are web numbers. `prereport.py` prints the expression for every web in a w3a colouring
   trace.
