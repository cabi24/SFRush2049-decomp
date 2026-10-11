/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001E864 (static, boot 0x8001E864, 204 B, segment 0x1f340 -> src/rom/lib_1f340.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 34 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): numeric_memory_utility_8001E864.
 * Body + declarations verbatim from cloud/work/boot_tail/C11-numeric/func_8001E864_NONMATCH.c (Astra / C11-numeric).
 * Shaping devices: none. */
void *func_8001E864(const void *key, const void *base, int count, int size,
                  int (*compare)(const void *, const void *))
{
    int low;
    int high;
    int middle;
    int result;
    void *element;
    if (count != 0) {
        low = 1;
        high = count;
        do {
            middle = (low + high) >> 1;
            element = (void *)((const unsigned char *)base + size * (middle - 1));
            result = compare(key, element);
            if (result == 0) {
                return element;
            }
            if (result < 0) {
                high = middle - 1;
            } else {
                low = middle + 1;
            }
        } while (high >= low);
    }
    return 0;
}
