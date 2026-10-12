/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct MessageQueue MessageQueue;
extern MessageQueue D_80038368;
extern int D_8002C5DC;
extern int osJamMesg(MessageQueue *, void *, int);
void func_800145DC(void)
{
    if (D_8002C5DC > 0) {
        if (--D_8002C5DC == 0) {
            osJamMesg(&D_80038368, 0, 0);
        }
    }
}
