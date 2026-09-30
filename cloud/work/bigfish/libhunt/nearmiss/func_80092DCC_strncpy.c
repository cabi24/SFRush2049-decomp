char *func_80092DCC(char *s1, const char *s2, unsigned int n) {
    char *d = s1;
    const char *s = s2;
    unsigned int i = n;
    char c;
    if (i != 0) {
        do {
            c = *s;
            i--;
            d++;
            s++;
            *(d - 1) = c;
            if (c == 0) break;
        } while (i != 0);
    }
    while (i != 0) {
        i--;
        *d++ = 0;
    }
    return s1;
}
