# Directional positional sound: full 608-byte match

`stat_lap_split`, `[0x800FEA00, 0x800FEC60)`, is a **strict MATCH of all
152 words**, including its four-byte owned coefficient. Its historical name
does not describe the recovered behavior. New matching candidate: **608 bytes**.
Accepted/spliced/ROM coverage gain claimed here: **zero**.

Base: `cc4d5fdd`. Source: [`cloud/matches/stat_lap_split.c`](../../../matches/stat_lap_split.c).
The source and evidence are for independent review, not automatic integration.

## Selection and new evidence

The current lock, tracked claims, wave-5 results, the parent's active ranges,
and open PRs #98/#99 were checked before source edits. This target was unlocked
and unclaimed in those sources. This does not expose unpublished work.
The existing B8 and A170 packets were read; both left six differing words.
This is an evidence-led reopening, not a claim that the function had never
been attempted.

Two new leads justified the pass:

- The pinned arcade [`game/carsnd.c`, `target_sound`](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/carsnd.c#L770-L878)
  has the distinctive signed-square directional selector, the `0.924` squared
  coefficient, a car pointer initialized in its declaration before the guards,
  and compound multiplication of both signed squares.
- Wave 5 supplies the genuine accepted `high_scores_display` sound-request
  group. Both actual callees and four accepted actual callers can now be
  compiled with this body, without placeholders.

The ancestor's entire N64 identity or original source text is not claimed.
The arcade uses coordinate conversions, different dispatch tables and the
double expression `.924 * .924`. The N64 body uses x/z directly and a
single-precision coefficient. The checked-in vector macro is the substantive
three-component operation also present as `mvecsub` in the pinned arcade
`game/vecmath.h`; this does not establish whether the original N64 boundary
was a macro or a function. An authentic pointer-increment `vecsub` inline
control added 16 frame bytes and did not match.

## Actual contract

Inputs are a sound index, a valid player slot, a world-position vector and an
unsigned-byte mode. A disabled global or player state other than 6 returns
the all-ones failure value without a call. Otherwise:

1. Subtract the player's position and transform the delta by its 3×3 matrix.
2. Use the transformed x/z squared magnitude. Above a unit squared radius,
   sign each squared component and compare with `.924f * .924f * magnitude`.
3. Select signed-halfword x/y pan-table entries. Sound 6 uses centered pan.
4. Call the real seven-argument sound API with scale 1 and return its tag.

The player stride is 952 bytes, position offset 8 and matrix offset 44.
The C contract requires a valid player slot even on early-return paths,
because the car pointer is formed at declaration. Native early-return behavior
with an invalid slot is not a defined-host-C claim. The source does not modify
the position vector, player records, state flags or pan tables.

## What closed the match

Fresh stock IDO controls, always `-g0 -O3 -mips2 -G 0 -non_shared`:

| Source | Complete-body differences | ELF bytes |
|---|---:|---:|
| Archived B8 pointer-first source | 6/152 | 608 |
| New source with car pointer assigned after the guards | 5/152 | 608 |
| New source with expanded second sign product | 1/152 | 608 |
| Final declaration initializer plus compound products | **0/152** | **608** |
| Final source at O2 | 144/152; two own references unverified | 596 |

Workbench diagnosis preceded the bounded source pass. The one-word product
gap is FP operand order. The other five words are scheduling of the three
delta stores and following player loads.

Retaining the stock compiler's intermediate listing isolated a single
compiler-generated no-alias declaration for the player pointer and stack.
Removing that one metadata declaration from the late-initialization listing
makes the diagnostic output exact. This is **diagnostic only**, ineligible for
promotion. The ordinary C declaration initializer independently causes IDO
to omit that declaration and produces the exact body with the unmodified
compiler. `verify.py` reruns both the source controls and the counterfactual;
all intermediate listings/objects remain temporary, never committed.

No padding, artificial reads, dead checks, unused formals, volatile shaping,
stand-in callers, register assignments, or protected flag/scorer/target edits
were used. Reused accepted context is byte-for-byte unchanged, including its
pre-existing source conventions.

## Verification

`verify.py` checks:

- Exact 608-byte ELF function extent, all 152 native words and all 15 GNU
  relocations, including the three actual call sites. Text needs no extra
  alignment bytes. GNU ld independently links at `0x800FEA00`.
- All four owned literal bytes at `0x80124824`, independently placed by GNU
  ld and checked against the protected data. The coefficient is
  `0.8537760376930237`; 12 trailing zero bytes are section alignment outside
  that literal. The unadapted arcade double expression fails the native proof.
- An eleven-body O3 context regression: this caller; its real matrix callee;
  all five existing sound-group bodies with their accepted keep list unchanged;
  and all four already-accepted direct callers. Every body remains strict
  MATCH at its full ELF extent. Context sources are bound by hashes.
- **4,578** cases comparing protected native instructions, independently
  GNU-linked instructions, an arithmetic oracle and unchanged-source host C
  under UBSan. Native execution includes the actual 37-word matrix callee.
  Sound dispatch uses a synthetic callback that records all seven arguments,
  returns a controlled tag and clobbers every ordinary caller-save register.
- Native/GNU-linked read/write traces agree; source/player inputs remain
  unchanged; stack and callee-save registers are restored. Signed tables,
  byte narrowing, both early exits, the centered sound, directional boundaries,
  random matrices and full-width return tags are covered.
- Six deliberately wrong contracts are rejected: coefficient, axis, state,
  centered-sound selection, signed pan tables and failure return.

All 151 dynamically reachable caller instruction offsets execute. Offset
`+0x1BC` is an unreachable duplicated negation after an unconditional branch,
not an omitted runtime arm; it is still included in the full static proof.

The synthetic domain is eight valid player records and finite binary32 values
with host round-to-nearest. This is not a claim about signaling NaNs, FCSR
flags, arbitrary addresses, actual gameplay pan-table contents, or complete
sound-subsystem behavior. No full-game shadow-unit, splice, image, compressed
stream, or ROM hash gate was run.

## Reproduce

With the pinned IDO toolchain and GNU MIPS binutils available:

```sh
python3 tools/cloud/score.py fn cloud/matches/stat_lap_split.c stat_lap_split --flags '-g0 -O3 -mips2 -G 0 -non_shared'
python3 cloud/work/frontier/dot_directional_sound_20261005/verify.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_directional_sound_match.py -q
```

The receipt binds source, compiler, verifier and accepted-context inputs.
The ordinary verifier does not rewrite it; `--write` explicitly records a new
reviewed replay. No ROM bytes, raw assembly streams, proprietary data tables,
objects, compiler binaries or credentials are part of this packet.
