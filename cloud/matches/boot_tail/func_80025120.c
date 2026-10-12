/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Use the SDK tag from PR/os_message.h; this wrapper never needs its layout. */
typedef struct OSMesgQueue_s OSMesgQueue;
extern OSMesgQueue D_800586A8;
extern int osJamMesg(OSMesgQueue *, void *, int);

void func_80025120(int token)
{
    osJamMesg(&D_800586A8, 0, 0);
}
