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

struct AudioState;
extern void func_80011A3C(struct AudioState *);

void func_80014B3C(int index)
{
    if (D_80038294[index].active != 0) {
        D_80038294[index].release_count = 20;
        if (D_80038294[index].pending != 0) {
            D_80038294[index].pending = 0;
        } else {
            func_80011A3C((struct AudioState *)&D_80038294[index]);
        }
    }
}
