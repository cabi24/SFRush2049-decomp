# Homework 001: Analyze Remaining D_80 Global Variables

## Objective
Analyze the remaining 509 unique D_80XXXXXX global variables in `src/game/game.c` and propose descriptive names for the most frequently used ones.

## Context
We've already renamed 127 globals (35% of references). The remaining globals need analysis to determine their purpose. Use context clues from:
- How they're used in the code
- Comments in the decompiled source
- Arcade source equivalents in `reference/repos/rushtherock/`

## Tasks
1. Run this command to get the top 50 remaining globals:
   ```bash
   grep -oE 'D_80[0-9A-Fa-f]+' src/game/game.c | sort | uniq -c | sort -rn | head -50
   ```

2. For each of the top 30 globals, search for usage context:
   ```bash
   grep -n "D_80XXXXXX" src/game/game.c | head -10
   ```

3. Cross-reference with arcade globals in:
   - `reference/repos/rushtherock/game/globals.h`
   - `reference/repos/rushtherock/game/game.h`

4. Propose names following these conventions:
   - Use lowercase with underscores: `player_score`, `race_timer`
   - Be specific: `car_velocity_x` not `vel1`
   - Include type hints where helpful: `is_paused`, `num_players`

## Deliverables
Create `docs/globals_remaining_analysis.md` with:

```markdown
# Remaining Global Variables Analysis

## High Priority (6+ references)
| Address | Refs | Proposed Name | Evidence | Confidence |
|---------|------|---------------|----------|------------|
| D_80XXXX | N | proposed_name | "used in X context" | High/Med/Low |

## Medium Priority (3-5 references)
[same table format]

## Arcade Cross-References Found
| N64 Address | Arcade Variable | File |
|-------------|-----------------|------|

## Recommended Next Batch for Renaming
[List top 20 with sed commands ready to use]
```

## Reference Files
- `src/game/game.c` - Main game source (9400+ lines)
- `docs/globals_mapping.md` - Already-mapped globals
- `reference/repos/rushtherock/game/globals.h` - Arcade globals

## Success Criteria
- At least 30 globals analyzed with proposed names
- At least 10 arcade cross-references identified
- Ready-to-use sed commands for next renaming batch
