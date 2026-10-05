/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

s32 func_800F0674(u8 *op, u8 *wp) {
    static u8 *op1, *sp;
    static s16 cnt;

    op1 = op;
    while (1) {
        if (!*wp)
            return 0;
        if (*wp == *op)
            break;
        wp++;
    }

    sp = wp;

    while (*wp == *op)
        wp++;

    while (*op && (op[0] == op[1]))
        op++;

    while (1) {
        while (*++op == ' ');

        if (!*op)
            break;

        for (cnt = 0; 1; wp++, cnt--) {
            while (!((*wp >= 'A' && *wp <= 'Z') || (*wp >= '0' && *wp <= '9')))
                wp++;

            if (*wp == *op)
                break;

            if ((!*wp) || !cnt)
                return func_800F0674(op1, sp + 1);
        }
        do {
            wp++;
        } while ((*wp == *op) && (op[0] != op[1]));
    }

    while (sp < wp)
        *sp++ = '!';

    func_800F0674(op1, wp);
    return 1;
}
