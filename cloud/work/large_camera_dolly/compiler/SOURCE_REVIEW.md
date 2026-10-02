# Independent native and source review

Target: historical `camera_dolly`, `0x800BC2BC`, 575 executable words / 2300 bytes.

## Identity and ABI

The raw retail function corresponds directly to `frictioncircle` in
`reference/repos/rushtherock/game/tires.c:248`, a tire friction and slip calculation.
The historical camera name is not a semantic description. Its only native caller,
`camera_follow_target` at `0x800BCBB8`, corresponds to arcade `dotireforce`.

The full native ABI is ordinary O32:

1. `Model *m` in `$a0`.
2. `f32 tirev[3]` in `$a1`.
3. `f32 normalforce` as integer register bits in `$a2`.
4. `f32 torque` as integer register bits in `$a3`.
5. `Tire *tire` at caller stack +16.
6. `f32 *sfp` at caller stack +20.
7. `f32 *trp` at caller stack +24.

The native caller preserves the model/vector parameters and supplies these seven
arguments. The callee dereferences the model immediately. Old prototypes with
floating first and second parameters conflict with the native evidence.

`func_800BC21C` at `0x800BC21C` takes the vector pointer in `$a0`, returns float
in `$f0`, and corresponds to `calcalpha` at arcade `tires.c:458`. It computes
lateral / abs(longitudinal) with denominator capped at 400, preserving lateral
when longitudinal is zero. Native axes are lateral index 0, longitudinal index 2.
All live caller-save values at this call are saved/reloaded; no IPA context is
needed to compile the complete target independently.

## Layout evidence

| Native model offset | Field |
|---|---|
| 944 | unnamed braking multiplier, absolute value read |
| 980 | brake input |
| 1008 | speed used for >20 damping condition |
| 1472 | mass |
| 1588 | time step |

| Native tire offset | Field |
|---|---|
| 0 | tradius |
| 4 | springK |
| 8 | rubdamp |
| 16 | Cfmax |
| 20 | invmi |
| 28 | Afmax |
| 32 / 36 / 40 | k1 / k2 / k3 |
| 68 / 72 / 76 | patchy / angvel / sliptorque |
| 80 / 84 | sideforce / traction |
| 88 | signed byte slipflag |

Opaque model/tire ranges represent actual unaccessed struct members, not stack
padding. Source declares only meaningful parameters and computations.

## N64 differences from arcade

- Unloaded-tire path executes before slip force calculations and includes
  braking decay with a nonnegative angular velocity clamp.
- A braking condition snaps angular velocity toward road velocity using the
  multiplier at model+944.
- The negative rotational slip path softens lateral force above speed 20 using
  the length of the lateral/longitudinal tire velocity vector plus 2.
- Saturation limits are evaluated in their actual branches rather than kept
  across the alpha helper call.

The completed source retains the original algorithm, ordinary C89 declarations,
signed byte slip flags and real float operation order. No dummy arguments,
artificial pressure, no-op arithmetic or raw instruction arrays are used.

Compiler: IDO 5.3, pinned toolkit
`796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5`.
Flags: `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.
`fabsf` and `sqrtf` are declared as intrinsics to generate the native floating
instructions. Exact source and independent canonical results are recorded in
`final_independent.json` after final source publication.
