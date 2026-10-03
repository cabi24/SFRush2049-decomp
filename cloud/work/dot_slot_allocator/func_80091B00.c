/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* NONMATCH research: 18/42 full instruction words differ.
 * Scans 128 24-byte slots; claims the first inactive slot and resets its ID.
 * Native fields: signed 16-bit ID at +0; signed byte active at +3.
 * Remaining bytes are opaque layout, not inferred behavioral fields.
 */
typedef struct Slot {
    short index;
    unsigned char reserved;
    signed char active;
    unsigned char opaque[20];
} Slot;

extern Slot D_80142DD8[128];

Slot *func_80091B00(void)
{
    int i;
    for (i = 0; i < 128; i++) {
        if (!D_80142DD8[i].active) {
            D_80142DD8[i].active = 1;
            D_80142DD8[i].index = -1;
            return &D_80142DD8[i];
        }
    }
    return 0;
}
