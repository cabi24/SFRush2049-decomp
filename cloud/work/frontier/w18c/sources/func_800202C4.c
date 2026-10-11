/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_800202C4 - lib_207b0 (IDO 7.1). "Is the audio engine idle?": with the audio system up
 * (D_8002C630), raise the busy flag D_8004BE94 while polling: if func_800146AC() (locked,
 * always 0) reports nothing pending, OR together func_8001467C(i) (locked: voice i active) for
 * every voice i < D_8004FA18; otherwise count as active. Returns 1 when nothing is active.
 * w18c: D_8004BE94 is `volatile` - the declaration of the locked lib_113e0 TU
 * (`extern volatile unsigned char D_8004BE94`); retail materialises its address for both
 * stores (lui/addiu, sb 0(reg)), which the non-volatile D10 form does not (true=445).
 * No shaping devices. Prior: D10 BT03-high-runtime (5.3, 26 words). */
typedef unsigned char u8;
extern u8 D_8002C630;
extern volatile u8 D_8004BE94;
extern u8 D_8004FA18;
extern int func_800146AC(void);
extern u8 func_8001467C(int);

u8 func_800202C4(void)
{
    u8 active;
    int i;
    active = 0;
    if (D_8002C630) {
        D_8004BE94 = 1;
        if (func_800146AC() == 0) {
            for (i = 0; i < D_8004FA18; i++) {
                active |= func_8001467C(i);
            }
        } else {
            active = 1;
        }
        D_8004BE94 = 0;
    }
    return active == 0;
}
