/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
typedef int s32;
typedef unsigned int u32;
typedef unsigned char u8;
typedef struct Voice Voice;
struct Voice {
    Voice *next;
    u32 reserved4;
    f32 start;
    s32 handle;
    u8 pad10[12];
    u8 bytes[4];
    s32 enabled;
};
typedef struct VoiceLists {
    u8 reserved[16];
    Voice *active;
    Voice *other;
} VoiceLists;
extern VoiceLists D_80155220;
extern f32 D_8002EB90;
extern void func_8008C074(s32, s32, s32, s32, u8 *, s32, s32);
extern void func_8008D0C0(s32);
extern void func_800AFA84(VoiceLists *, Voice *);
void audio_voice_set(void) {
    Voice *p, *next;
    s32 count = 0;
    u32 amount;
    f32 rate;
    for (p = D_80155220.other; p; p = p->next) count++;
    p = D_80155220.active;
    while (p) {
        if (p->enabled) {
            rate = (D_8002EB90 - p->start) / (((f32)count + (f32)count)/100.0f + 2.0f);
            amount = (u32)(rate * 255.0f + 63.0f);
            if (amount < 255) {
                p->bytes[3] = 255 - amount;
                if (p->bytes[3] > 192) p->bytes[3] = 192;
                func_8008C074(p->handle, 4, 0, 0, p->bytes, 0, 0);
            } else {
                next = p->next;
                func_8008D0C0(p->handle);
                func_800AFA84(&D_80155220, p);
                p = next;
                continue;
            }
        }
        p = p->next;
    }
}
