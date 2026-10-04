/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct MessageQueue MessageQueue;
extern MessageQueue D_80038368;
extern int osRecvMesg(MessageQueue *, void **, int);

void func_80014624(void)
{
    osRecvMesg(&D_80038368, 0, 1);
}
