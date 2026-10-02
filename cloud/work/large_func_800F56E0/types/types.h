/* Native access context for func_800F56E0, full 518-word body. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Resource { u8 *data; } Resource;
typedef struct Object {
    u8 unknown0[8];
    u32 owner_valid;                 /* +8, read solely as != 0 */
    u8 unknown12[32];
    Resource *resource;              /* +44 */
} Object;
typedef struct Handle { Object *object; } Handle;
typedef struct Player76 {
    u8 index, selector;
    u8 unknown2[62];
    u16 count;                       /* +64, added into stats +78 */
    u8 unknown66[6];
    Handle *handle;                  /* +72 */
} Player76;
typedef struct Model952 {
    u8 unknown0[238];
    s8 place;                       /* +238: zero/one/two correspond to podium */
    s8 finished;                    /* +239: nonzero enables completion counters */
    u8 unknown240[24];
    f32 distance;                   /* +264: divided by 528.0f */
    u8 unknown268[684];
} Model952;
typedef struct Stats96 {
    u32 unknown0;
    f32 best_individual[5];          /* +4..20 */
    f32 best_average[5];             /* +24..40 */
    u8 unknown44[20];
    f32 total_time;                  /* +64 */
    u16 total_samples;               /* +68, adds sample count */
    u16 completions;                 /* +70 */
    u16 firsts, seconds, thirds;     /* +72,74,76, subject to participant threshold */
    u16 count_sum;                   /* +78, adds Player76.count */
    u16 enabled_completions;         /* +80 */
    u16 enabled_full_runs;           /* +82: requested laps == completed samples */
    u32 unknown84;
    u32 distance;                    /* +88, accumulated via float round trip */
    u16 bits;                       /* +92, neighboring func_800B78F0 */
    u8 unknown94[2];
} Stats96;
typedef struct Owners60 {
    Handle *individual[5];           /* +0..16 */
    Handle *average[5];              /* +20..36 */
    u8 unknown40[20];
} Owners60;
typedef struct Times32 { f32 samples[8]; } Times32;

extern Player76 input_rec0[];       /* 0x8014A118, stride 76 */
extern Model952 player_array[];     /* 0x80152818, stride 952 */
extern Object *D_80146150[];        /* Handle points at one pointer slot */
extern s16 active_player_count;    /* 0x8014A108 */
extern s8 D_8014978C;               /* mode */
extern s8 D_80152570;               /* alternate stats bank */
extern s8 D_80152744;               /* participant count for podium thresholds */
extern s8 D_80142760;               /* enabled special mode flag */
extern s16 D_80152734;              /* compared to sample count for enabled win */
extern u8 D_80144018[];             /* per-player sample counts */
extern Stats96 D_80150F88[];        /* default stats bank */
extern Owners60 D_80151690[];       /* default-table owners, mode stride60 */
extern s8 D_80151AC0[10];           /* insertion player tags; no mode indexing */
extern Times32 D_80149A78[];        /* per-player time samples, stride32 */
extern void func_800CD8EC(Handle *, u8);
void func_800F56E0(void);
