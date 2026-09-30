char *func_800A464C(char *s1, const char *s2) {
    const char *p;
    const char *q;
    if (*s1 == 0) {
        if (*s2 != 0) return 0;
        return s1;
    }
    while (*s1 != 0) {
        p = s1;
        q = s2;
        while (1) {
            if (*q == 0) return s1;
            if (*q != *p) break;
            p++;
            q++;
        }
        s1++;
    }
    return 0;
}
