/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct MessageQueue MessageQueue;
extern MessageQueue D_80038368;
extern int D_8002C5DC;
extern int osRecvMesg(MessageQueue *, void **, int);
void func_80014594(void)
{
    if (D_8002C5DC == 0) {
        osRecvMesg(&D_80038368, 0, 1);
    }
    D_8002C5DC++;
}
