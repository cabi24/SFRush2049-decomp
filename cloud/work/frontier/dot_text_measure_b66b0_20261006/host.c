/* Compile the exact unchanged matching translation unit. Uncalled menu context
 * is discarded by the host linker; it is not executed or behaviorally claimed. */
#include <stdio.h>
#include <stdlib.h>
#include "cloud/matches/dot_text_measure_b66b0_20261006/group.c"

static unsigned int read_u32(FILE *f)
{
    unsigned char bytes[4];
    if (fread(bytes, 1, 4, f) != 4) exit(2);
    return ((unsigned int)bytes[0] << 24) | ((unsigned int)bytes[1] << 16)
         | ((unsigned int)bytes[2] << 8) | bytes[3];
}
int main(int argc, char **argv)
{
    FILE *f;
    unsigned int count, i, j, raw, length, expected;
    unsigned char *buffer, *before;
    int actual, changed;
    if (argc != 2 || (f = fopen(argv[1], "rb")) == NULL) return 2;
    count = read_u32(f);
    for (i = 0; i < count; i++) {
        raw = read_u32(f); length = read_u32(f); expected = read_u32(f);
        buffer = (unsigned char *)malloc(length);
        before = (unsigned char *)malloc(length);
        if (buffer == NULL || before == NULL) return 2;
        if (fread(buffer, 1, length, f) != length) return 2;
        for (j = 0; j < length; j++) before[j] = buffer[j];
        actual = func_800B66B0(buffer, (s16)(raw & 65535U));
        changed = 0;
        for (j = 0; j < length; j++) if (before[j] != buffer[j]) changed = 1;
        if ((unsigned int)actual != expected || changed) {
            fprintf(stderr, "fixture %u failed: actual=%d expected=%u\n", i, actual, expected);
            return 1;
        }
        free(buffer); free(before);
    }
    if (fgetc(f) != EOF) return 2;
    fclose(f);
    printf("%u unchanged-source C89 UBSan/bounds cases passed\n", count);
    return 0;
}
