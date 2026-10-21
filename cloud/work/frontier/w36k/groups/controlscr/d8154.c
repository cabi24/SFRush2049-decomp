/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* func_800D8154 = validOption (MAT3 controlscr.c validOption(signed char p, int option); GC body is
 * `return 1`, the N64 port rejects option 8). Same six words as the locked single; the switch form
 * (w36k) is the source size at which umerge inlines it into updateButtons (D816C) and
 * calcControlMenuMaxSize (D9058) but not into updateControlScreen (D91A0, six jal sites), as retail.
 * It replaces Astra's PR #322 `if (0) { var_v1 = 1; }` lever (that form also stopped uopt folding
 * `i + k == 8` in D9058's unrolled loop). The unused player parameter is the private-ABI stack word.
 */
typedef signed char s8;
typedef signed int s32;
s32 func_800D8154(s8 player, s32 item) {
    s32 var_v1;

    var_v1 = 1;
    switch (item) {
    case 8:
        var_v1 = 0;
        break;
    }
    return var_v1;
}
