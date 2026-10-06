/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef int s32;
typedef struct Pak772 { u8 index; s8 enabled; s8 rumble; s8 changed; u8 pad4[768]; } Pak772;
typedef struct State { u8 pad0[2]; u8 flags; u8 pad3; } State;
extern s8 D_8011EAE0;
extern volatile Pak772 D_80144030[4];
extern volatile State D_80149440[4];

void func_800A43FC(void)
{
    s32 i;

    if (D_8011EAE0) {
        for (i = 0; i < 4; i++) {
            if ((D_80149440[i].flags & 1) && !(D_80149440[i].flags & 2)) {
                if (!D_80144030[i].rumble) {
                    D_80144030[i].rumble = 1;
                }
            } else if (D_80144030[i].rumble) {
                D_80144030[i].rumble = 0;
                D_80144030[i].changed = 1;
            }
        }
    }
}
