/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct CallbackWords { unsigned int words[8]; } CallbackWords;
extern CallbackWords D_80038000;

void func_80020598(const CallbackWords *callbacks)
{
    D_80038000.words[0] = callbacks->words[0];
    D_80038000.words[1] = callbacks->words[1];
    D_80038000.words[2] = callbacks->words[2];
    D_80038000.words[3] = callbacks->words[3];
    D_80038000.words[4] = callbacks->words[4];
    D_80038000.words[5] = callbacks->words[5];
    D_80038000.words[6] = callbacks->words[6];
    D_80038000.words[7] = callbacks->words[7];
}
