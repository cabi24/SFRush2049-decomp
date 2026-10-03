/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern volatile unsigned char D_8002D480[];
extern int func_80025264(unsigned int);
extern void func_800254D4(int);

int func_80025594(unsigned int token)
{
    int selected;

    if (token != (unsigned int)-1 && D_8002D480[0]) {
        selected = func_80025264(token);
        if (selected != -1) {
            func_800254D4(selected);
            return 1;
        }
    }
    return 0;
}
