/* flags: -g0 -O3 -mips2 -G 0 -non_shared  (also code-identical at -O2) */
/*
 * func_800F0674 is the arcade fixword() (reference/repos/rushtherock/game/
 * hiscore.c:1623, the high-score name censor): find obscene word `op` in the
 * player's text `wp`, tolerating repeated letters, overwrite the hit with '!'
 * and recurse on the rest.  Returns 1 if anything was replaced.
 * N64 differences, all visible in the code:
 *   - chars are unsigned (lbu);
 *   - the player's word skips everything that is not A-Z or 0-9, where the
 *     arcade skipped only spaces;
 *   - strictcnt is the constant 0 (`for (cnt = 0; ...)`).
 * The arcade declaration list is kept verbatim: `static char *op1,*sp;
 * static S16 cnt;`.  The function-local statics are what makes it match:
 * with `extern` globals uopt must assume `*sp++ = '!'` aliases them and the
 * body is 110 words off (same length).
 *
 * STATE: code identical (118/118 words); the 18 references to the three
 * statics are own-.bss and unverified by the scorer.  Retail addresses:
 * op1 0x80156940, sp 0x80156948, cnt 0x80156950 (8 apart; this object has
 * them at .bss+0/+4/+8, one-to-one and in the same order).  Not spliceable
 * until something owns game .bss (blob_group/blob_splice refuse .bss);
 * tools.conveyor.pipeline.blob_unit reports it EQUAL ("own zero-initialised
 * data, addresses consistent").
 */
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
