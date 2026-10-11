/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80025AB4 - lib_25bb0 (IDO 7.1). Open a stream sample bank: with the stream system up
 * (D_8002D480[0]) release the previous table if this module allocated it (D_8002D484); for a
 * SOURCE outside KSEG0 (ROM/cart address) translate it (service hook D_80038014), read the
 * 4-byte entry count through a temporary 4-byte buffer (hooks D_80038018 allocate /
 * D_8003801C release, func_80010628 = copy), allocate count * 4 bytes and copy the table that
 * follows the count (D_8005868C, owned: D_8002D484 = 1); for a KSEG0 SOURCE use it in place
 * (table = SOURCE + 4, count = *SOURCE, not owned) and translate DATA. D_80058684 / D_80058688
 * keep the bank / data addresses, D_80058690 the count. Returns 1, or 0 when the system is
 * down or an allocation fails. No direct callers (entry point).
 * Types: the TU's declarations (D_8002D480, D_8003801C (func_800254D4's block), `void *`
 * D_8005868C and `unsigned char` D_8002D484 (later blocks)); the service hooks as the separate
 * function-pointer words the target relocates against (D_80038014 / D_80038018 as declared
 * in lib_11640). No struct view of D_80038000 (D10's ServiceHooks view links to
 * the same bytes but names D_80038000+0x14.. : true=370 with it).
 * w19a: from D10 BT07-stream-pair (5.3; 7.1 true=370): hooks by their own symbols, no
 * `byte_count` local (count * 4 in the arguments), one `return 1` after the two branches
 * (retail's store-then-return order). No shaping devices, no unused locals. */
extern volatile unsigned char D_8002D480[];
extern unsigned char D_8002D484;
extern void *(*D_80038014)(void *);
extern void *(*D_80038018)(unsigned int, unsigned int);
extern void (*D_8003801C)(void *);
extern void *D_80058684;
extern void *D_80058688;
extern void *D_8005868C;
extern unsigned int D_80058690;
extern void func_80010628(void *, void *, unsigned int);

int func_80025AB4(void *source, void *data)
{
    unsigned int *count;

    if (D_8002D480[0]) {
        if (D_8005868C && D_8002D484) {
            D_8003801C(D_8005868C);
        }
        if (((unsigned int)source & 0xF0000000) != 0x80000000) {
            D_80058688 = D_80058684 = D_80038014(source);
            count = D_80038018(4, 0);
            if (!count) {
                return 0;
            }
            func_80010628(count, D_80058684, 4);
            D_80058690 = *count;
            D_8003801C(count);
            D_8005868C = D_80038018(D_80058690 * 4, 0);
            if (!D_8005868C) {
                return 0;
            }
            func_80010628(D_8005868C, (unsigned char *)D_80058684 + 4, D_80058690 * 4);
            D_8002D484 = 1;
        } else {
            D_80058684 = source;
            D_80058688 = D_80038014(data);
            D_8005868C = (unsigned int *)source + 1;
            D_80058690 = *(unsigned int *)source;
            D_8002D484 = 0;
        }
        return 1;
    }
    return 0;
}
