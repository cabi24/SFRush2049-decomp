# MODELDAT Structure Documentation

## Overview

`MODELDAT` is the primary vehicle physics and state structure from the Rush The Rock arcade source code. It contains all data needed for physics simulation, dead reckoning, I/O, and network synchronization of a single vehicle.

**Source File**: `reference/repos/rushtherock/game/modeldat.h`
**Last Updated**: 1997/10/12 (Revision 3.43)
**Copyright**: Time Warner Interactive

The N64 version (Rush 2049) likely uses a similar structure, possibly referenced by `D_801461D0` which has 160+ references in the decompiled code.

---

## MODELDAT Structure (Main)

Total estimated size: ~1,800-2,000 bytes (varies by platform alignment)

### Body Axis Variables

| Offset | Field | Type | Size | Description |
|--------|-------|------|------|-------------|
| 0x000 | F[3] | F32[3] | 12 | Force vector (body axis) |
| 0x00C | M[3] | F32[3] | 12 | Moment vector (body axis) |
| 0x018 | A[3] | F32[3] | 12 | Angular acceleration |
| 0x024 | AA[3] | F32[3] | 12 | Accumulated angular acceleration |
| 0x030 | W[3] | F32[3] | 12 | Angular velocity (rad/sec) |
| 0x03C | V[3] | F32[3] | 12 | Velocity (body coordinates) |
| 0x048 | D[3] | F32[3] | 12 | Displacement |
| 0x054 | TIREFORCE[4][3] | F32[4][3] | 48 | Force at each tire |
| 0x084 | TIRER[4][3] | F32[4][3] | 48 | Tire positions (body coords) |
| 0x0B4 | TIREV[4][3] | F32[4][3] | 48 | Tire velocities |
| 0x0E4 | BODYFORCE[4][3] | F32[4][3] | 48 | Body forces at corners |
| 0x114 | BODYR[4][3] | F32[4][3] | 48 | Body reference positions |
| 0x144 | peak_body_force[2][3] | F32[2][3] | 24 | Peak body forces |
| 0x15C | CENTERFORCE[3] | F32[3] | 12 | Forces through center of gravity |
| 0x168 | CENTERMOMENT[3] | F32[3] | 12 | Moments at CG (anti-spin) |
| 0x174 | peak_center_force[2][3] | F32[2][3] | 24 | Peak CG forces |
| 0x18C | tireuvs[4] | uvect[4] | 112* | Tire orientation matrices |
| 0x1FC | G[3] | F32[3] | 12 | Gravity vector (body coords) |

*uvect is 28 bytes: short uvs[3][3] + F32 fpuvs[3][3] + short fixcnt

### Real World Axis Variables

| Offset | Field | Type | Size | Description |
|--------|-------|------|------|-------------|
| 0x208 | RWA[3] | F32[3] | 12 | Real World Acceleration (ft/sec^2) |
| 0x214 | RWV[3] | F32[3] | 12 | Real World Velocity (ft/sec) |
| 0x220 | RWR[3] | F32[3] | 12 | Real World Position (ft) |
| 0x22C | TIRERWR[4][3] | F32[4][3] | 48 | Tire positions (world coords) |
| 0x25C | BODYRWR[4][3] | F32[4][3] | 48 | Body positions (world coords) |
| 0x28C | EPRWR[3] | EPTYPE[3] | 24 | Extended precision positions |
| 0x2A4 | GRW[3] | F32[3] | 12 | Gravity in real world coords |
| 0x2B0 | UV | uvect | 28 | Rotation matrix (orientation) |

### Scalar Vehicle Variables

| Offset | Field | Type | Size | Description | Valid Range |
|--------|-------|------|------|-------------|-------------|
| 0x2CC | steerangle | F32 | 4 | Front tire steer angle (radians) | -0.6 to 0.6 |
| 0x2D0 | steerratio | F32 | 4 | Steering ratio (wheel/kingpin) | 10:1 to 24:1 |
| 0x2D4 | torque[4] | F32[4] | 16 | Torque at each tire |
| 0x2E4 | steergain | F32 | 4 | Steering gain factor |
| 0x2E8 | swtpg | F32 | 4 | Steering wheel torque per G |
| 0x2EC | trail | F32 | 4 | Caster trail |
| 0x2F0 | swtorquegain | F32 | 4 | Steering wheel torque gain |
| 0x2F4 | clutch | F32 | 4 | Clutch position | 0.0 to 1.0 |
| 0x2F8 | throttle | F32 | 4 | Throttle position | 0.0 to 1.0 |
| 0x2FC | brake | F32 | 4 | Brake position | 0.0 to 1.0 |
| 0x300 | brakeroll | F32 | 4 | Brake roll effect |
| 0x304 | brakegain[4] | F32[4] | 16 | Brake gain per wheel |
| 0x314 | pitchangle | F32 | 4 | Vehicle pitch angle (radians) |
| 0x318 | magvel | F32 | 4 | Magnitude of velocity |
| 0x31C | gear | S16 | 2 | Current gear | 0=neutral, 1-4 |
| 0x31E | commandgear | S16 | 2 | Requested gear |
| 0x320 | autotrans | S16 | 2 | Automatic transmission flag | 0=manual, 1=auto |
| 0x322 | ignition | S16 | 2 | Ignition state | 0=off, 1=on, 2=end-game |
| 0x324 | startermotor | S16 | 2 | Starter engaged flag |
| 0x326 | swtorque | S32 | 4 | Steering wheel torque (ft-lbs) |
| 0x32A | swdamp | S32 | 4 | Steering wheel damping |
| 0x32E | swfrict | S32 | 4 | Steering wheel friction |
| 0x332 | maxswdamp | S32 | 4 | Maximum steering damping | 0-16 |
| 0x336 | maxswfrict | S32 | 4 | Maximum steering friction | 0-0x100 |
| 0x33A | minswfrict | S32 | 4 | Minimum steering friction | 0-0x100 |
| 0x33E | carnum | S16 | 2 | Car model type | 0=beginner to 3=extreme |

### Engine and Drivetrain Variables

| Field | Type | Size | Description | Notes |
|-------|------|------|-------------|-------|
| catchup | F32 | 4 | Catchup factor | 0.0=none, 0.5=half, 1.0=full |
| engtorque | F32 | 4 | Engine torque (ft-lbs) |
| enginvmi | F32 | 4 | 1/moment of inertia of flywheel |
| engangvel | F32 | 4 | Engine rotational speed (rad/sec) |
| dwratio | F32 | 4 | Differential gear ratio |
| dwtorque | F32 | 4 | Torque at driving wheels |
| dwcf | F32 | 4 | Coefficient of friction at driving wheels |
| dwzforce | F32 | 4 | Vertical force at driving wheels |
| dwinvmi | F32 | 4 | 1/moment of inertia of driving wheels |
| efdwinvmi | F32 | 4 | 1/effective moment of inertia |
| dwangvel | F32 | 4 | Driving wheel angular velocity |
| dwinvradius | F32 | 4 | 1/radius of driving wheels |
| dwsliptorque | F32 | 4 | Torque from slipping rear wheels |
| fgtorquescale | F32 | 4 | First gear torque scale |
| sgtorquescale | F32 | 4 | Second gear torque scale |
| torquescale | F32 | 4 | Standard torque scale (gear > 2) |
| dirttorquescale | F32 | 4 | Off-road torque scale |
| dwslipflag | S16 | 2 | Tire slipping flag |
| roadtorque | F32 | 4 | Torque applied to road surface |
| nothrusttorque | S08 | 1 | Zero yaw torque from thrust flag |
| magicdif | S08 | 1 | Load sensitive torque split flag |

### Clutch Variables

| Field | Type | Size | Description |
|-------|------|------|-------------|
| clutchmaxt | F32 | 4 | Maximum torque with clutch fully engaged |
| clutchtorque | F32 | 4 | Current clutch torque |
| clutchangvel | F32 | 4 | Clutch angular velocity |

### Transmission Variables

| Field | Type | Size | Description |
|-------|------|------|-------------|
| transratio | F32 | 4 | Current gear ratio |
| totalratio | F32 | 4 | Total ratio (gear * differential) |
| torquecurve | S16* | 4 | Pointer to torque curve (pavement) |
| dirttorquecurve | S16* | 4 | Pointer to torque curve (dirt) |
| transarray | F32* | 4 | Transmission ratio array |
| rpmperent | S16 | 2 | RPM per entry in torque curve | Typically 1000 |
| topgear | S16 | 2 | Top gear for automatic | 1-4 |
| upshiftangvel | F32 | 4 | Angular velocity for upshift |
| downshiftangvel | F32 | 4 | Angular velocity for downshift |

### Tire Description Array

| Field | Type | Size | Description |
|-------|------|------|-------------|
| tires[4] | tiredes[4] | ~464 | Tire parameters for each wheel |

See [tiredes Structure](#tiredes-structure) below.

### Dashboard Gauges

| Field | Type | Size | Description |
|-------|------|------|-------------|
| amperes | F32 | 4 | Ammeter reading |
| enginetemp | F32 | 4 | Engine temperature |
| oilpressure | F32 | 4 | Oil pressure gauge |
| fuel | F32 | 4 | Fuel gauge |

### Car and World Constants

| Field | Type | Size | Description | Notes |
|-------|------|------|-------------|-------|
| mass | F32 | 4 | Car mass (slugs) | weight/32.2 |
| massinv | F32 | 4 | 1/mass |
| weight | F32 | 4 | Car weight (lbs) |
| wheelbase | F32 | 4 | Distance between axles (ft) |
| springrate[4] | F32[4] | 16 | Spring constant per wheel (lb/ft) |
| cdamping[4] | F32[4] | 16 | Compression damping (lb-sec/ft) |
| rdamping[4] | F32[4] | 16 | Rebound damping (lb-sec/ft) |
| rideheight[4] | F32[4] | 16 | Static ride height per wheel |
| suscomp[4] | F32[4] | 16 | Suspension compression (ft) |
| tpcomp[4] | F32[4] | 16 | Tire patch compression |
| airdist[4] | F32[4] | 16 | Vertical distance to road (ft) |
| airvel[4] | F32[4] | 16 | Vertical velocity to road (ft/sec) |
| farspringrate | F32 | 4 | Front anti-roll bar rate |
| rarspringrate | F32 | 4 | Rear anti-roll bar rate |
| roaduvs[4] | uvect[4] | 112 | Road surface orientation at tires |
| roadcode[4] | S32[4] | 16 | Road surface type per tire |
| sviscode[4] | U16[4] | 8 | Surface visual code |
| roadboost[4] | U16[4] | 8 | Boost value (0=none, 15=max) |
| sound_flags[4] | U16[4] | 8 | Sound effect flags |
| shortcut_flags[4] | U16[4] | 8 | Shortcut detection flags |
| gameover | S16 | 2 | Game over flag |
| lasttp[4] | STREETYP*[4] | 16 | Last track piece per tire |
| swampgain | F32 | 4 | Swamp effect gain |
| yawdampfix | F32 | 4 | Yaw damping fix factor |
| reflecttime | F32 | 4 | Next reflection allowed time |
| thetime | F32 | 4 | Current simulation time |
| airtime | F32 | 4 | Time in air |
| dt | F32 | 4 | Time step (seconds) |
| idt | F32 | 4 | Inverse time step |
| minspeed | F32 | 4 | Minimum speed threshold |
| dtcycles | S32 | 4 | Number of cycles per dt |
| I[3] | F32[3] | 12 | Moment of inertia vector |
| INVI[3] | F32[3] | 12 | Inverse moment of inertia |
| srefpcybo2 | F32 | 4 | Aerodynamic drag factor |
| rollresist | F32 | 4 | Rolling resistance (lbs) |
| iter | S32 | 4 | Iteration counter |

### Collision and Crash Variables

| Field | Type | Size | Description |
|-------|------|------|-------------|
| crashflag | S32 | 4 | Crash occurred flag |
| crashthreshold | S32 | 4 | Minimum force for crash |
| topscrape | BOOL | 4 | Top scraping ground flag |
| thumpflag | S32 | 4 | Thump sound trigger flag |
| lastthump | S32 | 4 | Time of last thump |
| thumpthreshold | F32 | 4 | Minimum force for thump |
| thump_side | S16 | 2 | -1=left, 0=middle, 1=right |
| forceavg | F32 | 4 | Force average for crash detection |
| swhpitch | F32 | 4 | Steering wheel pitch |
| viewheight | F32 | 4 | Observer height above CG |
| colrad | F32 | 4 | Collision radius |

### Scattered Variables

| Field | Type | Size | Description |
|-------|------|------|-------------|
| shifttime | F32 | 4 | Time until next shift allowed |
| bog_state | S16 | 2 | Engine bog state (0-3) |
| resurrect | resurrecttyp | ~100 | Resurrection data |
| mph | F32 | 4 | Speedometer reading |

### Model/Host Communication

| Field | Type | Description |
|-------|------|-------------|
| initin | initintyp | Initialization data (pole position, start line) |
| fastin | fastintyp | Per-iteration input (wheel, swvel, modelrun) |
| mainin | mainintyp | Less frequent input (clutch, brake, gear) |
| reckon | RECKON | Dead reckoning data |

### Network/Linking Data

| Field | Type | Size | Description | Valid Range |
|-------|------|------|-------------|-------------|
| slot | S16 | 2 | Index into gLink |
| net_node | S16 | 2 | Network node reference |
| in_game | S16 | 2 | Player in current game |
| we_control | S16 | 2 | Stack controls this vehicle |
| drone_type | S16 | 2 | 0=no car, 1=drone, 2=human |
| engine_type | S16 | 2 | Engine sound model type |
| body_type | S16 | 2 | Car body style |
| rpm | S16 | 2 | Engine RPM |
| appearance | U32 | 4 | Visual appearance flags |
| stuck_time | U32 | 4 | Time stuck (drone abort) |
| mcheckpoint | S8 | 1 | Model version checkpoint |
| mlap | S8 | 1 | Model version lap |
| mmpath_lap | S8 | 1 | Maxpath lap count |
| laps | S8 | 1 | Real laps completed |
| last_cp_index | S16 | 2 | Last checkpoint index |
| next_cp_index | S16 | 2 | Next checkpoint index |
| collidable | S16 | 2 | Collision enabled |
| drone_target | S16 | 2 | Player targeted by drone |
| drone_scale | F32 | 4 | Drone velocity scale factor |
| data_valid | BOOL | 4 | Data updated at least once |
| can_we_cheat | BOOL | 4 | Drone cheating allowed |
| hide_car | BOOL | 4 | Hide during resurrection |
| please_abort_me | S16 | 2 | Drone requesting abort |
| time_boost | F32 | 4 | Speed multiplier (1.0=normal) |
| time_fudge | F32 | 4 | Combined time adjustments |

---

## Related Structures

### tiredes Structure

**Source**: `reference/repos/rushtherock/game/tiredes.h`

Tire physics parameters based on Milliken vehicle dynamics model.

| Field | Type | Size | Description | Typical Values |
|-------|------|------|-------------|----------------|
| tradius | F32 | 4 | Tire radius (ft) | 1.0 |
| springK | F32 | 4 | Static lateral stiffness (lb/ft) | 340*12 |
| rubdamp | F32 | 4 | Lateral damping | 200 |
| PaveCstiff | F32 | 4 | Cornering stiffness on pavement (lb/rad) | 16000-64000 |
| PaveCfmax | F32 | 4 | Max sliding friction on pavement | 1.15-1.3 |
| Cstiff | F32 | 4 | Current cornering stiffness | Same as PaveCstiff |
| Cfmax | F32 | 4 | Current max sliding friction | Same as PaveCfmax |
| invmi | F32 | 4 | 1/moment of inertia | 0.1 |
| Zforce | F32 | 4 | Loading for cornering stiffness | Calculated |
| Afmax | F32 | 4 | Slip angle at max friction | Calculated |
| k1, k2, k3 | F32 | 12 | Cf polynomial coefficients | Calculated |
| l2, l3 | F32 | 8 | Additional coefficients | Calculated |
| m1, m2, m3, m4 | F32 | 16 | Aligning torque coefficients | Calculated |
| patchy | F32 | 4 | Contact patch Y |
| angvel | F32 | 4 | Tire angular velocity (rad/sec) |
| sliptorque | F32 | 4 | Torque from tire slipping |
| sideforce | F32 | 4 | Lateral force (for squeal sound) |
| traction | F32 | 4 | Traction coefficient |
| slipflag | char | 1 | Tire slipping flag |

**Total size**: ~116 bytes

### RECKON Structure (Dead Reckoning)

**Purpose**: Network dead reckoning for multiplayer synchronization

| Field | Type | Size | Description |
|-------|------|------|-------------|
| base_time | S32 | 4 | Base time (10 microsec units) |
| base_fudge | F32 | 4 | Time fudge factor |
| base_RWA[3] | F32[3] | 12 | Base acceleration |
| base_RWV[3] | F32[3] | 12 | Base velocity |
| base_RWR[3] | F32[3] | 12 | Base position |
| base_W[3] | F32[3] | 12 | Base angular velocity |
| base_UV[3][3] | F32[3][3] | 36 | Base rotation matrix |
| base_airdist[4] | F32[4] | 16 | Base shadow distances |
| base_airvel[4] | F32[4] | 16 | Base shadow velocities |
| base_quat[4] | F32[4] | 16 | Base quaternion |
| delta_quat[4] | F32[4] | 16 | Quaternion delta |
| num_quat_steps | S16 | 2 | Steps to target quaternion |
| torque | S16 | 2 | Engine torque (ft-lbs) |
| rpm | U16 | 2 | Engine RPM |
| steerangle | F32 | 4 | Steering angle |
| tireW[4] | F32[4] | 16 | Tire angular velocities |
| suscomp[4] | F32[4] | 16 | Suspension compressions |
| airdist[4] | F32[4] | 16 | Air distances |
| look | U32 | 4 | Engine type (spare) |
| appearance | U32 | 4 | Visual appearance |
| RWV[3] | F32[3] | 12 | Dead reckoned velocity |
| RWR[3] | F32[3] | 12 | Dead reckoned position |
| UV[3][3] | F32[3][3] | 36 | Dead reckoned rotation |

### CAR_DATA Structure (Game Task View)

**Purpose**: Display-side view of car state, updated via dead reckoning

| Field | Type | Description |
|-------|------|-------------|
| dr_pos[3] | F32[3] | Dead reckoned position |
| dr_vel[3] | F32[3] | Dead reckoned velocity |
| dr_uvs[3][3] | F32[3][3] | Dead reckoned orientation |
| dr_tirepos[4][3] | F32[4][3] | Dead reckoned tire positions |
| dr_acc[3] | F32[3] | Base acceleration |
| RWV[3], RWR[3] | F32[3] | Real world velocity/position |
| V[3] | F32[3] | World velocity |
| TIRERWR[4][3] | F32[4][3] | Tire world positions |
| TIRER[4][3] | F32[4][3] | Tire body positions |
| tireW[4] | F32[4] | Tire angular velocities |
| mph | F32 | Speedometer reading |
| crashflag | S16 | Crash flag |
| rpm | S16 | Engine RPM |
| engine_type | S16 | Engine type |
| body_type | S16 | Body style |
| appearance | U32 | Visual flags |
| data_valid | BOOL | Data valid flag |
| place | S8 | Race position (1st, 2nd, etc.) |
| place_locked | S8 | Position locked |
| gameover_time | U32 | Game over timestamp |
| score | U32 | Player score |
| objnum | S32 | ZOID object number |
| distance | F32 | Distance from start |
| we_died | S8 | Needs resurrection |
| just_crashed | S8 | In death animation |
| crashtime | U32 | Crash timestamp |
| visuals[] | Visual[] | Visual effects overlay |
| collidable | S16 | Collision enabled |
| collide_time | U32 | Collision timestamp |
| collide_count | S8 | Transparency countdown |
| collide_state | S8 | Transparency state |
| mpath | MPCTL | Maxpath data |
| fwd_progress_t | S32 | Help arrow timer |
| last_distance | F32 | Previous progress |
| sound_flags | U16 | Sound flags |
| shortcut_flags | U16 | Shortcut flags |
| checkpoint | S8 | Current checkpoint |
| laps | S8 | Current lap |

### Car Structure (Static Parameters)

**Purpose**: Template for car types (beginner, advanced, expert, extreme)

| Field | Type | Description |
|-------|------|-------------|
| name | char* | Car name |
| mass | F32 | Mass in slugs |
| I[3] | F32[3] | Moment of inertia |
| springrate[4] | F32[4] | Suspension spring rates |
| farspringrate | F32 | Front anti-roll bar |
| rarspringrate | F32 | Rear anti-roll bar |
| cdamping[4] | F32[4] | Compression damping |
| rdamping[4] | F32[4] | Rebound damping |
| steerratio | F32 | Steering ratio |
| swtpg | F32 | Steering torque per G |
| srefpcybo2 | F32 | Aero drag factor |
| rollresist | F32 | Rolling resistance |
| tires[4] | tiredes[4] | Tire descriptions |
| TIRER[4][3] | F32[4][3] | CG to tire vectors |
| brakebal | F32 | Front brake balance |
| engmi | F32 | Engine inertia |
| dwratio | F32 | Differential ratio |
| clutchmaxt | F32 | Max clutch torque |
| viewheight | F32 | View height |
| torquescale | F32 | Torque multiplier |
| transarray[] | F32[MAXGEAR+2] | Gear ratios |
| torquecp | S16* | Torque curve |
| rpmperent | S16 | RPM per curve entry |
| topgear | S16 | Highest gear |
| upshiftangvel | F32 | Upshift threshold |
| downshiftangvel | F32 | Downshift threshold |

---

## Physics Usage

### Tire Force Calculation

The tire model uses a polynomial approximation to generate lateral force:
```
Cf = k1*a + k2*|a|*a + k3*a^3
```
Where `a` is slip angle in radians.

**Key relationships**:
- `Cstiff`: Initial slope of force vs. slip angle curve (lb/radian)
- `Cfmax`: Maximum lateral force coefficient (typically 0.7-1.5 g)
- `Zforce`: Vertical tire loading

### Suspension Model

Spring force calculation:
```c
F_spring = springrate[wheel] * suscomp[wheel]
F_damp = (suscomp_vel > 0) ? rdamping[wheel] : cdamping[wheel]
F_damp *= suscomp_vel
```

Anti-roll bar (stabilizer):
```c
F_antiroll = farspringrate * (suscomp[0] - suscomp[1])  // Front
F_antiroll = rarspringrate * (suscomp[2] - suscomp[3])  // Rear
```

### Drivetrain Model

Power flow:
1. Engine torque from `torquecurve[throttle][rpm]`
2. Scaled by `torquescale` (varies by gear)
3. Multiplied by `transratio * dwratio`
4. Split by differential (optionally load-sensitive via `magicdif`)
5. Applied as `dwtorque` to driving wheels

### Speed Calculation

```c
mph = dwangvel * (1.0 / dwinvradius) * 3600 / 5280  // Convert rad/sec to mph
```

---

## N64 Mapping

### Potential D_801461D0 Correspondence

Based on 160+ references in `src/game/game.c`, `D_801461D0` appears to be used as:
- Main game state variable
- Accessed via `state_get()` and `state_set()` functions
- Used with DMA operations (`dma_start`)

The arcade source shows MODELDAT instances stored in:
```c
MODELDAT model[NCARS];  // NCARS = 12
```

If `D_801461D0` is the N64 equivalent of the `model[]` array base:
- Each MODELDAT is ~1,800-2,000 bytes
- 12 cars * 2,000 bytes = ~24,000 bytes

### Road Surface Codes

| Code | Name | Description |
|------|------|-------------|
| 0 | PAVEMENT | Regular pavement (full grip) |
| 1 | DIRT | Dirt surface (low friction) |
| 2 | WATER | Water (low friction) |
| 5 | COLLIDE | Collision with immovable object |
| 6 | AIR | Airborne |
| 7 | CRASH | Crash state |

### Visual Types (NUM_VISUALS = 20)

```c
enum VisualTypes {
    VIS_BLAST = 0, VIS_BRAKES, VIS_LSPARK, VIS_RSPARK, VIS_BSPARK,
    VIS_TIRE1, VIS_TIRE2, VIS_TIRE3, VIS_TIRE4,
    VIS_SHADOW,
    VIS_QUAD0, VIS_QUAD1, VIS_QUAD2, VIS_QUAD3, VIS_QUAD4,
    VIS_WINDOW0, VIS_WINDOW1, VIS_WINDOW2, VIS_WINDOW3,
    VIS_FRAME,
    NUM_VISUALS
};
```

---

## Constants

| Constant | Value | Description |
|----------|-------|-------------|
| MAXGEAR | 4 | Maximum forward gears |
| NCARS | 12 | Maximum selectable cars |
| NUM_VISUALS | 20 | Visual effect slots per car |

---

## References

- Original Milliken vehicle dynamics model (Doug Milliken, 1991)
- "Typical Vehicle Parameters for Dynamic Studies", Rasmussen et al., GM Proving Ground, 1970
- SAE papers on vehicle dynamics (available via Milliken Research)
