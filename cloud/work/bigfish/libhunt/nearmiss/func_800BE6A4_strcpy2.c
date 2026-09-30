typedef unsigned char u8;
u8 *func_800BE6A4(u8 *d, u8 *s) {
    u8 c = *s;
    u8 *dp = d + 1;
    u8 *sp;
    u8 a;
    if (c == 0xFF) {
        *d = c;
        sp = s + 1;
        a = *sp;
        dp = d + 1;
        while (a != 0 || sp[1] != 0) {
            *dp = a;
            dp[1] = sp[1];
            dp += 2;
            sp += 2;
            a = *sp;
        }
        *dp = 0;
        dp++;
        *dp = 0;
    } else {
        a = c;
        *d = a;
        if (a != 0) {
            sp = s + 1;
            do {
                a = *sp;
                dp++;
                sp++;
                dp[-1] = a;
            } while (a != 0);
        }
    }
    return d;
}
