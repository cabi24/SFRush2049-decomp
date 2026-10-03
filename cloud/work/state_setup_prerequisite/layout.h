/* Native-evidenced views only. Unknown bytes are layout, not stack pressure. */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned int u32;
typedef struct SetupEntry76 { u8 player; u8 rest[75]; } SetupEntry76;
typedef struct SetupConfig8 { u8 prefix[7]; u8 mode; } SetupConfig8;
typedef struct SetupModel952 {
    u8 prefix[0xE8]; u32 flags_e8; u8 gap_ec[3]; u8 byte_ef;
    u8 gap_f0[0x269]; u8 byte_359; u8 gap_35a[0x26];
    u32 word_380; u8 tail[0x34];
} SetupModel952;
typedef struct SetupVehicle2056 {
    u8 prefix[8]; u8 mode; u8 byte_9; u8 byte_a;
    u8 gap_b[0x7BF]; s16 half_7ca; u8 byte_7cc;
    u8 gap_7cd[7]; u32 word_7d4; u8 tail[0x30];
} SetupVehicle2056;
extern s16 D_8014A108, D_8015274C, D_80152768, D_80153FD2;
extern s16 D_80153E84, D_80153F08, D_80153F24, D_80153F40;
extern s8 D_80152744;
extern SetupEntry76 D_8014A118[];
extern SetupConfig8 D_80153E88[];
extern SetupModel952 player_array[]; /* 80152818, stride 952 */
extern SetupVehicle2056 D_8014A250[];
extern s16 D_801527D8[], D_80152808[];
/* Existing B99 body establishes these consumed formals. */
extern void music_tempo_set(s16 player, u8 mode, int apply);
extern void init_state_continue(void); /* logical interface; non-O32 clobbers */
