typedef s32 (*Cmp)(void *, void *);
void *entity_name_copy(void *key, u8 *base, u32 n, u32 size, Cmp cmp) {
    u8 *end;
    u32 half;
    u8 *mid;
    u8 *p;
    s32 c;
    if (n == 0 || size == 0) {
        return 0;
    }
    end = base + n * size;
    while (n != 0) {
        half = n >> 1;
        p = mid = base + half * size;
        c = cmp(key, p);
        if (c < 0) {
            n = half;
            continue;
        }
        if (c > 0) {
            base = p + size;
            if (n & 1) {
                n = half;
            } else {
                n = half - 1;
            }
            continue;
        }
        return p;
    }
    if (end > base) {
        if (cmp(key, base) == 0) {
            return base;
        }
    }
    return 0;
}
