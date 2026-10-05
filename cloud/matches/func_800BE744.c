/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Count characters in the game's byte / two-byte string encoding.
 * A leading 0xFF selects two-byte characters, terminated by a zero pair;
 * ordinary strings terminate at the first zero byte. No arcade ancestor
 * has been established. Keep one cursor initialized from the input, test
 * its first byte directly, and use the narrow postincrement condition:
 * caching the first byte changes IDO's narrow-loop register allocation.
 * No padding locals, artificial formals, inline assembly or owned data.
 */
typedef unsigned char u8;

int func_800BE744(u8 *string)
{
    int count = 0;
    u8 *cursor = string;

    if (*cursor == 0xFF) {
        cursor++;
        while (cursor[0] || cursor[1]) {
            cursor += 2;
            count++;
        }
    } else {
        while (*cursor++) {
            count++;
        }
    }
    return count;
}
