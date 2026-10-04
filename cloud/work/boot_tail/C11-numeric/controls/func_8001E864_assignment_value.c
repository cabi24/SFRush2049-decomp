/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
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
            element = (void *)((const unsigned char *)base +
                size * ((middle = (low + high) >> 1) - 1));
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
