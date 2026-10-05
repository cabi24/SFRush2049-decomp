# race_position_update — not matched (best.c, structurally right, -O3)

Semantics (historical label): fit a (possibly wide, 0xFF-prefixed) string into maxWidth pixels:
w = text width (object_manager_update); if it fits, strcpy (func_800BE6A4) and return; else copy, build a
tail "..." + last two characters (wide: byte copies; narrow: strcpy+strcat via func_800BE6A4/BE4F0),
truncate dst, then shorten dst one character at a time until width(dst) <= maxWidth - width(tail) -
letter spacing (D_80149B70), and append the tail. "..." is own .data at 0x80121DE8.

Found: the loop's maxlen argument is a register variable holding -1 (`li s4,-1` + sll/sra each call): a
late-folded expression reproduces it exactly (`n = len * 0 - 1`, cf. camera_lerp_position's `*= 0`).
Residual: retail keeps `width` in a2 (caller-saved, saved/reloaded at sp+92 around every call) and saves
s5 without using it; frame 120 (tail[24] at sp+96). Mine: width memory-homed (traced: web split, save 0.5,
nocs 4), one fewer callee-saved register, frame differs. ~45 variants. Next: uopt trace of why retail's
width web gets a2 — likely one more late-folded variable (the s5 web) or width passed as a third argument.
