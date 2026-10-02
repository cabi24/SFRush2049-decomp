/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned int u32;
extern u32 __OSGlobalIntMask;
extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32);
void __osPiGetCmdQueue(u32 mask) {
    register u32 saveMask=__osDisableInt();
    __OSGlobalIntMask|=mask;
    __osRestoreInt(saveMask);
}
