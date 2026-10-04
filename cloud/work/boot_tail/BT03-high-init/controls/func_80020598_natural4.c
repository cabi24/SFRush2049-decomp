/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct CallbackWords { unsigned int words[8]; } CallbackWords;
extern CallbackWords D_80038000;
extern void *memcpy(void *, const void *, unsigned int);

void func_80020598(const CallbackWords *callbacks)
{
    memcpy(&D_80038000, callbacks, sizeof(D_80038000));
}
