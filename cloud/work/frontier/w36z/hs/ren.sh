#!/bin/sh
# ren.sh IN OUT: rename the high-score file's address-less helpers (suffix _z) so a candidate does not collide with the
# installed group highscore_anim's copies in the unit (blob_unit renames duplicate stand-ins per file, __uK).
sed -E 's/\b(du_gfx_lock|du_gfx_unlock|du_font_set|hs_CenterText|hs_RightText|hs_font_set|hs_SetFont|DrawCircuitCode|DrawScores|hs_SetJust|gh_font_set|gh_SetFont|hs_DrawEllipseText)\b/\1_z/g' "$1" > "$2"
