/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct OSMesgQueue OSMesgQueue;
extern int osRecvMesg(OSMesgQueue *, void **, int);
extern int osJamMesg(OSMesgQueue *, void *, int);
extern OSMesgQueue D_80152770;
extern void audio_reverb_update(u32 address, s32 tag);
typedef struct GhostState {
    u8 pad0[5]; s8 state; u8 pad6[50]; f32 value;
    u8 pad60[16]; u32 allocation; u32 word80, word84;
} GhostState;
typedef struct GhostDescriptor { GhostState *state; } GhostDescriptor;
typedef struct GhostHandle GhostHandle;
typedef struct GhostData {
    u32 word0; GhostHandle *adjust; u8 pad8[28]; GhostDescriptor *allocation;
    GhostDescriptor *descriptor; u8 pad44[20]; u32 count, word68; GhostDescriptor *resource;
} GhostData;
struct GhostHandle { GhostData *data; };
typedef struct GhostPlayer { u8 pad0[239]; s8 ready; f32 value; u8 tail[708]; } GhostPlayer;
typedef struct GhostObject { u8 pad0[1820]; s16 reset; u8 tail[234]; } GhostObject;
extern GhostHandle *D_80152698[];
extern GhostPlayer D_80152818[];
extern GhostObject D_8014A250[];
extern GhostState *D_80140800;
extern s16 D_8014A108;
extern s32 D_8014A110;
extern volatile s8 D_80114738;
extern void menu_transition(GhostHandle *handle);

void ghost_race_setup(void)
{
    s32 i;
    GhostHandle *handle;
    GhostState *state;
    GhostPlayer *player;

    D_80140800 = 0;
    if (D_8014A110 == 2) {
        D_80114738 = 1;
        for (i = 0; i < D_8014A108; i++) {
            handle = D_80152698[i];
            if (handle != 0) {
                D_80152698[i] = 0;
                D_8014A250[i].reset = 0;
                player = &D_80152818[i];
                state = handle->data->descriptor->state;
                if (state->state >= 0) {
                    if (state->state > 0) {
                        menu_transition(handle);
                    }
                } else {
                    state->state = 0;
                    if (player->ready != 0) {
                        state->value = player->value;
                        state->word84 = state->word80;
                        D_80140800 = state;
                    } else {
                        u32 allocation = state->allocation;
                        osRecvMesg(&D_80152770, 0, 1);
                        audio_reverb_update(allocation, 0);
                        osJamMesg(&D_80152770, 0, 0);
                        state->allocation = 0;
                    }
                }
            }
        }
        D_8014A108 = 1;
        D_80114738 = 0;
    }
}
