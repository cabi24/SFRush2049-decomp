# Codex Task: AI/Drone System Deep Analysis

## Objective
Create a comprehensive mapping of the N64 AI/drone system to the arcade AI implementation, including pathfinding, behavior, and rubber-banding.

## Background
Rush 2049's AI drones use a maxpath-based navigation system with dynamic difficulty adjustment (rubber-banding). The arcade source has ~25K lines of drone code.

---

## Task 1: Maxpath Navigation System

### Arcade Files to Analyze
- `reference/repos/rushtherock/game/maxpath.c` (3,730 lines) - Core pathfinding
- `reference/repos/rushtherock/game/cpinit.c` - Checkpoint/path initialization

### Key Arcade Functions
```c
void InitMaxPath(S32 record)           // Initialize path tables
void MaxPath(MODELDAT *m)              // Update path following for car
void MP_TargetSpeed(MODELDAT *m, MPCTL *cp)    // Compute target speed
void MP_TargetSteerPos(MODELDAT *m, MPCTL *cp) // Compute steering target
void AdjustSpeed(MODELDAT *m, F32 tspd)        // Apply throttle/brake
void AdjustSteer(MODELDAT *m, F32 *pos)        // Apply steering
void avoid_areas(S16 drone_index)              // Obstacle avoidance
```

### N64 Functions to Map
- `maxpath_init` / `ai_waypoint_next`
- `ai_path_follow` / `ai_obstacle_avoid`
- `path_follow` / `track_get_node_pos`

### Deliverable
Create `docs/ai_maxpath_mapping.md` with:
1. Path point data structure analysis
2. Target speed/steering algorithm
3. Obstacle avoidance logic
4. Path recording system (for ghost mode)

---

## Task 2: Drone Behavior System

### Arcade Files
- `reference/repos/rushtherock/game/drones.c` (24,931 bytes) - Main drone AI
- `reference/repos/rushtherock/game/model.c` - Catchup/rubber-band

### Key Arcade Functions
```c
void InitDrones(void)           // Initialize drone system
void DoDrones(void)             // Per-frame AI update
void assign_drones(void)        // Assign personalities/difficulty
void EndDrones(void)            // Cleanup
F32 linear_interp(...)          // AI tuning interpolation
void set_catchup(void)          // Rubber-band activation
void no_catchup(void)           // Disable rubber-band
```

### N64 Functions to Map
- `drone_spawn` / `drone_update` / `drone_destroy`
- `ai_input_generate` / `ai_behavior_select`
- `ai_rubber_band` / `ai_catch_up`
- `ai_difficulty_set` / `ai_aggression_set`
- `ai_speed_limit_set` / `ai_error_inject`

### Deliverable
Create `docs/ai_drone_behavior_mapping.md` with:
1. Drone personality system
2. Difficulty scaling algorithm
3. Rubber-band implementation details
4. Error injection for realism

---

## Task 3: Overtaking and Defense

### Arcade Analysis Points
- How drones decide to overtake
- Defensive driving logic
- Player proximity awareness
- Speed adjustments near other cars

### N64 Functions
- `ai_overtake`
- `ai_defend`
- `ai_aggression_set`

### Deliverable
Create `docs/ai_racing_tactics_mapping.md` with:
1. Overtake decision tree
2. Defensive positioning
3. Aggression levels and effects
4. Multi-car awareness

---

## Task 4: Data Structure Analysis

### Arcade Structures to Document
From `reference/repos/rushtherock/game/modeldat.h`:
- `MPCTL` - Maxpath control data
- `DRONEDAT` - Per-drone state
- Path point tables

### Deliverable
Create `docs/ai_data_structures.md` with:
1. Complete MPCTL structure with field descriptions
2. Drone state machine diagram
3. Path data format
4. Memory layout comparison (arcade vs N64)

---

## Analysis Commands

Use these to find relevant N64 code:
```bash
# Find AI-related functions in game.c
grep -n "ai_\|drone_\|maxpath\|rubber" src/game/game.c

# Find path/waypoint functions
grep -n "waypoint\|path_\|node_" src/game/game.c

# Count AI function stubs
grep -c "TODO.*[Aa][Ii]\|TODO.*[Dd]rone" src/game/*.c
```

## Success Criteria
- Complete mapping of maxpath algorithm
- Rubber-band formula documented
- At least 15 drone behavior functions mapped
- Pseudocode for key AI decision points
- Data structure byte-level comparison where possible
