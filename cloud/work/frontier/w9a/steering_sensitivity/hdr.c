/*
 * w9a: steering_sensitivity (0x800AD128, 232 words) -- strict MATCH in this
 * real -O3 group (no stand-ins; the four context bodies are the locked
 * frontier_traction_control sources, unchanged).
 *
 * steering_sensitivity(unused, idx, position, outPosition, outMatrix,
 * threshold) is a historical label; no arcade ancestor found (N64-only path
 * code, like traction_control).  It places a world position in the cross
 * section of path segment idx (next record wraps to 0):
 *   outMatrix = the segment's basis (math_utility); rel = position in that
 *   frame (vector_diff_process, register-parameter internal callee);
 *   the forward coordinate rel[2] is clamped to [0, skew1] and gives the
 *   fraction along the segment; width (+0x58) and halfWidth (+0x54) are
 *   lerped between the two records; height = halfWidth - width - 2.5.
 *   If the basis is upside down (outMatrix[1][1] < 0) it is turned over
 *   (func_800AD090(0, -1)) and x/y are mirrored (y measured from the other
 *   side, + 5.0).  Inside the flat part (|x| <= width) outPosition[1] is the
 *   height above/below and returns; otherwise the point is in a rounded
 *   corner of radius `height`: dist from the corner centre, outPosition[1] =
 *   height - dist, and below `threshold` the basis is rotated onto the
 *   curved surface (func_800ACFF8 / func_800AD090 with sine/cosine pairs).
 *
 * Shaping that the retail bytes require (each one moved the colouring, found
 * with the traced uopt; see cloud/work/frontier/w9a/RESULTS.md):
 *   - one variable `frac` first holds r0->skew1 and then z / skew1 (gives the
 *     shared $f0 web);
 *   - `z = 0;` int literal (a second zero web: two mtc1 zero);
 *   - `height -= width; height -= 2.5f;` as separate statements (height in
 *     $f20 and the final subtract in the bc1f delay slot);
 *   - `x = rel[0]; x += r0->skew0;` after the lerps (x before width in web
 *     order, loads after the lerp loads);
 *   - `y = rel[1];` is dead on the else path, which re-reads rel[1].
 *   0.01f is the function's own .rodata word at 0x80123BFC (verified).
 */
