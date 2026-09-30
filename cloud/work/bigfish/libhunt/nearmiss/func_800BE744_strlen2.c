typedef unsigned char u8;
int func_800BE744(u8 *s) {
    int len = 0;
    u8 c = *s;
    u8 *p;
    u8 d;
    if (c == 0xFF) {
        p = s + 1;
        while (p[0] != 0 || p[1] != 0) {
            p += 2;
            len++;
        }
    } else if (c != 0) {
        p = s + 1;
        do {
            d = *p;
            len++;
            p++;
        } while (d != 0);
    }
    return len;
}
