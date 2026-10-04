/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct CallbackWords { unsigned int words[8]; } CallbackWords;
extern CallbackWords D_80038000;

void func_80020598(const CallbackWords *callbacks)
{
    CallbackWords *destination;
    destination = &D_80038000;
    *destination = *callbacks;
}
