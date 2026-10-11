/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_800205E4 - lib_207b0 (IDO 7.1). Forwarder: calls func_80014D1C() (locked, lib_11640,
 * `void func_80014D1C(void)`). Caller-less in static code (table/indirect use). No shaping. */
extern void func_80014D1C(void);

void func_800205E4(void)
{
    func_80014D1C();
}
