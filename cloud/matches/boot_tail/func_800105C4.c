/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Initialize the callback queue once. Native-only reconstruction; arcade equivalent unknown. */
typedef struct OSMesgQueue_s OSMesgQueue;
typedef void *OSMesg;
extern void osCreateMesgQueue(OSMesgQueue *, OSMesg *, int);
extern unsigned char D_80037FE0[];
extern OSMesg D_80037FF8[];
extern volatile unsigned char D_80037FA0;
extern void (*D_80038020)(void);
extern void func_80010450(void);
void func_800105C4(void)
{
    if (D_80038020 == 0) {
        osCreateMesgQueue((OSMesgQueue *)D_80037FE0, D_80037FF8, 1);
        D_80037FA0 = 0;
        D_80038020 = func_80010450;
    }
}
