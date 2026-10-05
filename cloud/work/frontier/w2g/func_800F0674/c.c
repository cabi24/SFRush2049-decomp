/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

u8 *D_80156940;
u8 *D_80156948;
s16 D_80156950;

#define op1 D_80156940
#define sp D_80156948
#define cnt D_80156950

s32 func_800F0674(u8 *op, u8 *wp) {
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
