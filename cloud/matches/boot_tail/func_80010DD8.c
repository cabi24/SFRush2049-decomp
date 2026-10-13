/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Static audio delay-buffer allocation, IDO 7.1, 168 native bytes.
 * The unsigned product wraps before conversion from milliseconds to samples.
 * Positive duration always rounds strictly UP to the next 192-sample block,
 * including an exact multiple and a zero sample count. Zero duration clears
 * only the buffer pointer. The allocator takes (byte_count, alignment_mode).
 *
 * Expression spelling is significant: quantum-minus-remainder comes before
 * adding count, as in the already matched neighbor func_80010E80. Replacing
 * this with compound += shortens the body by six instructions. No inline
 * helper, false prototype, unused local, or padding is used.
 */
extern void *(*D_80038018)(unsigned int, unsigned int);
extern unsigned int D_800382F4;
extern void *D_800382E8;
extern void *D_800382F0;

void func_80010DD8(unsigned int rate, unsigned short duration)
{
    unsigned int count;
    if (duration != 0) {
        count = duration * rate / 1000;
        count = 192 - count % 192 + count;
        D_800382F4 = count;
        D_800382E8 = 0;
        D_800382F0 = D_80038018(count * 2, 0);
    } else {
        D_800382F0 = 0;
    }
}
