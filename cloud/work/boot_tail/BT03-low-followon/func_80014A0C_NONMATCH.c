/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct AudioSlot {
    u8 active;
    u8 pending;
    u8 unknown02[2];
    u16 rate;
    u8 unknown06[14];
    u32 position;
    u8 unknown18[16];
    u16 release_count;
    u8 unknown2A[22];
    u16 value40;
    u16 value42;
    u16 value44;
    u16 value46;
    u8 unknown48[32];
} AudioSlot;
extern AudioSlot *D_80038294;

void func_80014A0C(int index, u16 rate)
{
    if (rate <= 8192) {
        D_80038294[index].rate = rate;
    } else {
        D_80038294[index].rate = 8192;
    }
}
