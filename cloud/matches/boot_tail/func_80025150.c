/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct MessageQueue MessageQueue;
extern MessageQueue D_800586A8;
extern int osRecvMesg(MessageQueue *, void **, int);

void func_80025150(void)
{
    osRecvMesg(&D_800586A8, 0, 1);
}
