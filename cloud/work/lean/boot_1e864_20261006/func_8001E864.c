/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* N64 O32 adaptation of pinned MusyX sndBSearch. */
void *func_8001E864(const void *key, const void *base, int count, int size,
                  int (*compare)(const void *, const void *))
{
    long low;
    long high;
    long middle;
    long result;
    void *element;
    if (count != 0) {
        low = 1;
        high = count;
        do {
            if ((result = compare(key, (element = (void *)((unsigned long)base +
                size * ((middle = (low + high) >> 1) - 1))))) == 0) {
                return element;
            }
            if (result < 0) {
                high = middle - 1;
            } else {
                low = middle + 1;
            }
        } while (low <= high);
    }
    return 0;
}
