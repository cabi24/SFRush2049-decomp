# Codex Homework: AI/Drone System - Part 2 of 6
## Path Interval Finding & Position Interpolation

**Estimated Time**: 3-4 hours
**Difficulty**: Medium-Hard
**Prerequisites**: CODEX_AI_DRONE_01 completed

---

## Objective

Implement the core path-finding functions that determine where a car is relative to the path and calculate interpolated positions.

---

## Background

The maxpath system tracks each car's position relative to the pre-recorded path using:
- **Segment index**: Which path segment the car is on
- **xrel**: Forward distance along current segment
- **yrel**: Lateral distance from path centerline

This allows the AI to know if it's ahead of/behind schedule and how far off the racing line it is.

---

## N64 Functions to Match

| N64 Function | Address | Size | Arcade Equivalent |
|--------------|---------|------|-------------------|
| `ai_waypoint_next()` | 0x80041680 | ~200 bytes | `MP_FindInterval()` |
| `ai_path_position()` | (within ai_path_follow) | ~300 bytes | `MP_IntervalPos()` |

---

## Tasks

### Task 1: Implement MP_FindInterval()

**Arcade reference**: `maxpath.c:873`

This function finds which path segment a car is currently on by checking if it has moved past the current segment.

```c
/**
 * Find the current path interval for a car
 * Updates mpath_segment based on car position
 *
 * @param car_index Car to check
 * @return Current segment index
 */
s32 drone_find_interval(s32 car_index) {
    DroneControl *ctl = &drone_ctl[car_index];
    CarData *car = &car_array[car_index];
    MPATH *path;
    f32 dx, dy, dz;
    f32 seg_len;
    s32 seg = ctl->mpath_segment;

    // Get current path point
    path = &mpath_data[ctl->mpath_index][seg];

    // Calculate forward distance from segment start
    // Check if we've passed the segment end
    // If so, advance to next segment

    // Check if we've gone backwards
    // If so, go to previous segment

    return seg;
}
```

**Key arcade logic**:
```c
// From maxpath.c:873
if (xrel >= len) {
    // Moved to next segment
    mpath_segment++;
    if (mpath_segment >= num_points) {
        mpath_segment = lap_start;  // Wrap for next lap
    }
}
if (xrel < 0) {
    // Moved back a segment
    mpath_segment--;
    if (mpath_segment < 0) {
        mpath_segment = 0;
    }
}
```

### Task 2: Implement MP_IntervalPos()

**Arcade reference**: `maxpath.c:893`

Calculate the car's position relative to the current path segment:
- `xrel` = forward distance along segment
- `yrel` = lateral offset from path
- `cyrel` = curvature to next point

```c
/**
 * Calculate car position relative to current path segment
 *
 * @param car_index Car to calculate for
 * @param xrel_out Output: forward distance along segment
 * @param yrel_out Output: lateral offset from path
 */
void drone_interval_pos(s32 car_index, f32 *xrel_out, f32 *yrel_out) {
    DroneControl *ctl = &drone_ctl[car_index];
    CarData *car = &car_array[car_index];
    MPATH *p0, *p1;
    f32 seg_vec[3];
    f32 car_vec[3];
    f32 seg_len, seg_len_sq;
    f32 dot;

    // Get segment start and end points
    p0 = &mpath_data[ctl->mpath_index][ctl->mpath_segment];
    p1 = &mpath_data[ctl->mpath_index][ctl->mpath_segment + 1];

    // Segment vector
    seg_vec[0] = p1->pos[0] - p0->pos[0];
    seg_vec[1] = p1->pos[1] - p0->pos[1];
    seg_vec[2] = p1->pos[2] - p0->pos[2];

    // Vector from segment start to car
    car_vec[0] = car->pos[0] - p0->pos[0];
    car_vec[1] = car->pos[1] - p0->pos[1];
    car_vec[2] = car->pos[2] - p0->pos[2];

    // Segment length squared
    seg_len_sq = seg_vec[0]*seg_vec[0] + seg_vec[1]*seg_vec[1] + seg_vec[2]*seg_vec[2];
    seg_len = sqrtf(seg_len_sq);

    // Project car onto segment (dot product)
    dot = car_vec[0]*seg_vec[0] + car_vec[1]*seg_vec[1] + car_vec[2]*seg_vec[2];

    // xrel = forward distance along segment
    *xrel_out = dot / seg_len;

    // yrel = lateral distance (perpendicular)
    // Cross product magnitude for 2D (XZ plane)
    *yrel_out = (car_vec[0]*seg_vec[2] - car_vec[2]*seg_vec[0]) / seg_len;
}
```

### Task 3: Implement find_maxpath_dist()

**Arcade reference**: `maxpath.c:2215`

Find the path point at a given distance ahead of the current position.

```c
/**
 * Find path point at given distance ahead
 * Used for look-ahead steering
 *
 * @param car_index Car index
 * @param dist Distance to look ahead
 * @param pos_out Output position [3]
 */
void drone_find_path_dist(s32 car_index, f32 dist, f32 *pos_out) {
    DroneControl *ctl = &drone_ctl[car_index];
    s32 seg = ctl->mpath_segment;
    f32 remaining = dist;
    MPATH *p0, *p1;
    f32 seg_len;

    // Walk forward along path until we've covered 'dist'
    while (remaining > 0.0f) {
        p0 = &mpath_data[ctl->mpath_index][seg];
        p1 = &mpath_data[ctl->mpath_index][seg + 1];

        // Calculate segment length
        f32 dx = p1->pos[0] - p0->pos[0];
        f32 dz = p1->pos[2] - p0->pos[2];
        seg_len = sqrtf(dx*dx + dz*dz);

        if (remaining <= seg_len) {
            // Interpolate within this segment
            f32 t = remaining / seg_len;
            pos_out[0] = p0->pos[0] + t * dx;
            pos_out[1] = p0->pos[1] + t * (p1->pos[1] - p0->pos[1]);
            pos_out[2] = p0->pos[2] + t * dz;
            return;
        }

        remaining -= seg_len;
        seg++;

        // Wrap at lap boundary
        if (seg >= mpath_header[ctl->mpath_index].num_points - 1) {
            seg = mpath_header[ctl->mpath_index].lap_start;
        }
    }

    // Fallback: return current position
    pos_out[0] = mpath_data[ctl->mpath_index][seg].pos[0];
    pos_out[1] = mpath_data[ctl->mpath_index][seg].pos[1];
    pos_out[2] = mpath_data[ctl->mpath_index][seg].pos[2];
}
```

### Task 4: Add Path Tracking State to DroneControl

Update the DroneControl structure in `include/game/drone.h`:

```c
typedef struct DroneControl {
    // ... existing fields ...

    // Path tracking state
    s32 mpath_index;        // Which path set (0-7)
    s32 mpath_segment;      // Current segment index
    f32 xrel;               // Forward position on segment
    f32 yrel;               // Lateral offset from path
    f32 path_speed;         // Target speed from path
    f32 path_curvature;     // Path curvature at current point

    // ... rest of fields ...
} DroneControl;
```

---

## Verification

1. Build passes:
```bash
make VERSION=us NON_MATCHING=1 -j20
```

2. Test with debug output (optional):
```c
// Add to drone_update():
f32 xrel, yrel;
drone_interval_pos(car_index, &xrel, &yrel);
// xrel should increase as car moves forward
// yrel should be near 0 when car is on racing line
```

---

## Files to Modify

| File | Changes |
|------|---------|
| `include/game/drone.h` | Add path tracking fields to DroneControl |
| `src/game/drone.c` | Implement drone_find_interval(), drone_interval_pos(), drone_find_path_dist() |

---

## Arcade Source References

Read carefully:
1. `reference/repos/rushtherock/game/maxpath.c:873-950` - MP_FindInterval, MP_IntervalPos
2. `reference/repos/rushtherock/game/maxpath.c:2215-2280` - find_maxpath_dist

---

## Math Notes

The path system uses vector projection to find position relative to segments:

```
        Car
         *
        /|
       / |  yrel (lateral offset)
      /  |
     /   |
    *----+--------*
   P0   xrel     P1
        (forward distance)
```

- `xrel = dot(car_vec, seg_vec) / |seg_vec|`
- `yrel = cross(car_vec, seg_vec) / |seg_vec|` (2D cross = perpendicular distance)

---

## Success Criteria

- [ ] drone_find_interval() implemented
- [ ] drone_interval_pos() implemented
- [ ] drone_find_path_dist() implemented
- [ ] DroneControl has path tracking fields
- [ ] Build passes
- [ ] Git commit

---

## Next Homework

Proceed to **CODEX_AI_DRONE_03.md** for steering and speed control.
