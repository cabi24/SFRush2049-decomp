/* Genuine MAT3 HAL_text.c _TextLen; original two locals and return contract. */
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned int u32;
u32 func_800B66B0(const u8 *text, s16 numChars)
{
    u32 len;
    int nc;
    nc = 0;
    if (text[0] == 255) {
        len = 1;
        do {
            len += 2;
            nc++;
        } while ((text[len-1] || text[len-2]) &&
                 (numChars < 0 || nc <= numChars));
    } else {
        len = 0;
        do {
            len++;
            nc++;
        } while (*text++ &&
                 (numChars < 0 || nc <= numChars));
    }
    return len;
}
