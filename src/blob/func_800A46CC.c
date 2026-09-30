/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
char *func_800A46CC(char *s1, const char *s2, unsigned int n) {
    char *r = s1;
    while (*s1 != 0) s1++;
    while (n-- != 0 && (*s1++ = *s2++) != 0) {
        if (n == 0) *s1 = 0;
    }
    return r;
}
