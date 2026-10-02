# Tire friction force reconstruction

Target: historical symbol `camera_dolly`, address `0x800BC2BC`, full extent 575 instructions / 2,300 bytes. Accepted into the source-built image and original ROM.

## Source ancestry

The direct arcade counterpart is `reference/repos/rushtherock/game/tires.c:frictioncircle`. The native callee `func_800BC21C` corresponds to `calcalpha`; the only caller `camera_follow_target` corresponds to `dotireforce`. These historical camera names do not describe the native tire physics.

The ordinary ABI has seven arguments: model pointer, tire velocity vector pointer, normal force, torque, tire description pointer, side force output pointer, and traction output pointer. Native vector indices differ from the arcade conventions. The function updates wheel angular velocity, slip torque, contact patch deflection, slip flags, and lateral/longitudinal force outputs.

## N64 adaptation leads

The no-contact branch runs before the road-speed slip branches and applies extra braking drag. The negative-slip branch includes additional model-dependent conditions and damping. One force branch adds low-speed regularization to the vector magnitude denominator. Exact thresholds, record offsets, expression order, and float constants must be reconstructed from native code before acceptance.

## Team

Two agents, both GPT-6.1-sol at High reasoning, are independently reconstructing the body and investigating compiler matching. Their files belong in `reconstruction/` and `compiler/`. Raw assembly, objects and opcode arrays stay in ignored `build/large_camera_dolly/`.

Accepted cartridge coverage: game 13.38% (628/1,216 functions; 86,576/647,072 bytes), static 46.76%. Independent strict comparison proves all 575 instructions exact, with no extra executable words or unresolved/unverified relocations. The source-built image and full ROM hash gates passed.
