/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct MessageQueue MessageQueue;
extern MessageQueue D_80038368;
extern void *D_80038380;
extern void osCreateMesgQueue(MessageQueue *, void **, int);
extern int osJamMesg(MessageQueue *, void *, int);
void func_80014550(void)
{
    osCreateMesgQueue(&D_80038368, &D_80038380, 1);
    osJamMesg(&D_80038368, 0, 0);
}
