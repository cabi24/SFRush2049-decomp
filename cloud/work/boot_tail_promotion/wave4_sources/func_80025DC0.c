/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80025DC0.c to the production
 * src/rom/lib_25bb0.c service-hook view (ServiceHooks_8002574C); code unchanged. Wave 4. */
typedef struct ServiceHooks_8002574C { unsigned char unknown_00[28]; void (*release)(void *); } ServiceHooks_8002574C;
extern ServiceHooks_8002574C D_80038000;
extern volatile unsigned char D_8002D480[];
extern unsigned char D_8002D484;
extern void (*D_80038024)(void);
extern void *D_8005868C;
extern void *D_80058698[2];
extern unsigned int D_800586A0;
extern void func_800254D4(int);
extern void func_80025D84(void);
extern void func_80014594(void);
extern void func_8001061C(void);
extern void func_800145DC(void);

void func_80025DC0(void)
{
    int i;

    for (i = 0; i < 2; i++) {
        func_800254D4(i);
    }
    func_80025D84();
    D_8002D480[0] = 0;
    func_80014594();
    D_80038024 = 0;
    func_8001061C();
    func_800145DC();
    if (D_8005868C && D_8002D484) {
        D_80038000.release(D_8005868C);
    }
    if (D_800586A0 & 1) {
        for (i = 0; i < 2; i++) {
            D_80038000.release(D_80058698[i]);
        }
    }
}
