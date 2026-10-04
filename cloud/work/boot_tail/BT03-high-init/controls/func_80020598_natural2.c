/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct CallbackWords { void (*words[8])(void); } CallbackWords;
extern CallbackWords D_80038000;

void func_80020598(const CallbackWords *callbacks)
{
    D_80038000 = *callbacks;
}
