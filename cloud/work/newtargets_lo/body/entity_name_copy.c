typedef s32 (*Cmp)(void *, void *);
void *entity_name_copy(void *key, u8 *base, u32 n, u32 size, Cmp cmp) {
    u8 *end;
    u32 half;
    u8 *mid;
    u8 *p;
    s32 c;
    s32 sub;
    if (n == 0 || size == 0) {
        return 0;
    }
    end = base + n * size;
    while (n != 0) {
        half = n >> 1;
        mid = base + half * size;
        p = mid;
        c = cmp(key, mid);
        if (c < 0) {
            n = half;
        } else if (c > 0) {
            base = p + size;
            if (n & 1) {
                sub = 0;
            } else {
                sub = 1;
            }
            n = half - sub;
        } else {
            return mid;
        }
    }
    if (base < end && cmp(key, base) == 0) {
        return base;
    }
    return 0;
}
