/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef void *OSMesg;
typedef struct OSMesgQueue {
    void *mtqueue;
    void *fullqueue;
    int validCount;
    int first;
    int msgCount;
    OSMesg *msg;
} OSMesgQueue;
extern void osCreateMesgQueue(OSMesgQueue *, OSMesg *, int);
extern int osJamMesg(OSMesgQueue *, OSMesg, int);
extern OSMesgQueue D_800586A8;
extern OSMesg D_800586C0;

void func_800250AC(void)
{
    osCreateMesgQueue(&D_800586A8, &D_800586C0, 1);
    osJamMesg(&D_800586A8, 0, 0);
}
