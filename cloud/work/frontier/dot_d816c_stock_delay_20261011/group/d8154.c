/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Genuine six-word predicate. The leading player argument is retained for
 * the native private ABI. One unreachable assignment keeps umerge below the
 * D91A0 inline threshold but allows all three D816C inlines. It changes source
 * size only; it is a disclosed reconstruction lever, not original spelling.
 */
typedef signed char s8;
typedef signed int s32;
s32 func_800D8154(s8 player, s32 item) {
    s32 var_v1;
    if(0){
        var_v1 = 1;
    }

    var_v1 = 1;
    if (item == 8) {
        var_v1 = 0;
    }
    return var_v1;
}
