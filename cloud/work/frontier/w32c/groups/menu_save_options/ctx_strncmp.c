/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned int u32;
typedef int s32;
typedef signed char s8;

int func_800950AC(const unsigned char *s1, const unsigned char *s2, unsigned int n) {
    if (n == 0) return 0;
    while (n-- != 0 && *s1 == *s2) {
        if (n == 0 || *s1 == 0 || *s2 == 0) break;
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

