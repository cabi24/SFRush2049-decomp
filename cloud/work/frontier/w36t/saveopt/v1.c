/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* audio_bus_mix = SavePlayerOption and voice_stop_2 = _SaveRaceSetting (MAT3 GameSource/src/save.c,
 * GC 0x800389xx SavePlayerOption, 0x80038BB0 _SaveRaceSetting; _SaveBattleSetting 0x80038CA4,
 * _SaveStuntSetting, _SaveObstacleSetting 0x80038E74 inlined into SavePlayerOption).
 * N64 save layout: no raceType byte (settings blocks 4 bytes earlier than GC), datah at profile+0x2C. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

typedef struct { unsigned int checksum; unsigned char track, car, trans, carView;
    signed char laps, drones, gravity, difficulty, catchup, backwards, mirror, fog, wind, deaths; } anon_checksum_track_14;
typedef struct { unsigned int checksum; unsigned char track, car, trans, carView;
    signed char gravity, mirror, fog, wind, mode, points, minutes, neg; } anon_checksum_track_10;
typedef struct { unsigned int checksum; unsigned char track, car, trans, carView;
    signed char gravity, mirror, fog, wind, mode, flow, points, minutes, deaths; } anon_checksum_track_14_2;
typedef struct { unsigned int checksum; unsigned char track, car, trans, carView;
    signed char gravity, mirror, fog, wind; } anon_checksum_track_c;

typedef struct tSaveData {
    unsigned int nameChecksum;      /* 0x00 */
    unsigned int id[2];             /* 0x04 */
    unsigned char revision;         /* 0x0C */
    unsigned char name[13];         /* 0x0D */
    unsigned int optChecksum;       /* 0x1C */
    signed char options[21];        /* 0x20 */
    unsigned int contChecksum;      /* 0x38 */
    signed char controls[11];       /* 0x3C */
    signed char rumble;             /* 0x47 */
    anon_checksum_track_14 raceSettings;      /* 0x48 */
    anon_checksum_track_10 battleSettings;    /* 0x5C */
    anon_checksum_track_14_2 stuntSettings;   /* 0x6C */
    anon_checksum_track_c obstacleSettings;   /* 0x80 */
} tSaveData;

typedef struct tPS_File tPS_File;
typedef struct tPlayerProfile {
    void *next, *prev;              /* 0x00 */
    tPS_File **fileh;               /* 0x08 */
    unsigned char pad0C[0x2C - 0x0C];
    tSaveData **datah;              /* 0x2C */
} tPlayerProfile;

extern signed char D_8014978C;      /* gTrackNum */
#define gTrackNum D_8014978C

unsigned int format_string_parse(void *data, int size);          /* _CalcChecksum */
void PS_WriteFile(tPS_File **fileh, void *data, int size);

void voice_stop_2(tPlayerProfile **player, unsigned char option, signed char value, unsigned char track)
{
    tSaveData *data = *(*player)->datah;
    signed char *setting;
    switch (option) {
    case 21: setting = &data->raceSettings.laps; break;
    case 22: setting = &data->raceSettings.drones; break;
    case 23: setting = &data->raceSettings.gravity; break;
    case 24: setting = &data->raceSettings.difficulty; break;
    case 26: setting = &data->raceSettings.catchup; break;
    case 27: setting = &data->raceSettings.backwards; break;
    case 28: setting = &data->raceSettings.mirror; break;
    case 29: setting = &data->raceSettings.fog; break;
    case 30: setting = &data->raceSettings.wind; break;
    case 31: setting = &data->raceSettings.deaths; break;
    default: return;
    }
    if (*setting != value) {
        *setting = value;
        data->raceSettings.checksum = format_string_parse(&data->raceSettings.track, sizeof(data->raceSettings) - 4);
        PS_WriteFile((*player)->fileh, &data->raceSettings, sizeof(data->raceSettings));
    }
}

static void _SaveBattleSetting(tPlayerProfile **player, unsigned char option, signed char value, int track)
{
    tSaveData *data = *(*player)->datah;
    signed char *setting;
    switch (option) {
    case 23: setting = &data->battleSettings.gravity; break;
    case 28: setting = &data->battleSettings.mirror; break;
    case 29: setting = &data->battleSettings.fog; break;
    case 30: setting = &data->battleSettings.wind; break;
    case 37: setting = &data->battleSettings.mode; break;
    case 38: setting = &data->battleSettings.points; break;
    case 39: setting = &data->battleSettings.minutes; break;
    case 40: setting = &data->battleSettings.neg; break;
    default: return;
    }
    if (*setting != value) {
        *setting = value;
        data->battleSettings.checksum = format_string_parse(&data->battleSettings.track, sizeof(data->battleSettings) - 4);
        PS_WriteFile((*player)->fileh, &data->battleSettings, sizeof(data->battleSettings));
    }
}

static void _SaveStuntSetting(tPlayerProfile **player, unsigned char option, signed char value, int track)
{
    tSaveData *data = *(*player)->datah;
    signed char *setting;
    switch (option) {
    case 23: setting = &data->stuntSettings.gravity; break;
    case 28: setting = &data->stuntSettings.mirror; break;
    case 29: setting = &data->stuntSettings.fog; break;
    case 30: setting = &data->stuntSettings.wind; break;
    case 32: setting = &data->stuntSettings.mode; break;
    case 33: setting = &data->stuntSettings.flow; break;
    case 34: setting = &data->stuntSettings.points; break;
    case 35: setting = &data->stuntSettings.minutes; break;
    case 36: setting = &data->stuntSettings.deaths; break;
    default: return;
    }
    if (*setting != value) {
        *setting = value;
        data->stuntSettings.checksum = format_string_parse(&data->stuntSettings.track, sizeof(data->stuntSettings) - 4);
        PS_WriteFile((*player)->fileh, &data->stuntSettings, sizeof(data->stuntSettings));
    }
}

static void _SaveObstacleSetting(tPlayerProfile **player, unsigned char option, signed char value, int track)
{
    tSaveData *data = *(*player)->datah;
    signed char *setting;
    switch (option) {
    case 23: setting = &data->obstacleSettings.gravity; break;
    case 28: setting = &data->obstacleSettings.mirror; break;
    case 29: setting = &data->obstacleSettings.fog; break;
    case 30: setting = &data->obstacleSettings.wind; break;
    default: return;
    }
    if (*setting != value) {
        *setting = value;
        data->obstacleSettings.checksum = format_string_parse(&data->obstacleSettings.track, sizeof(data->obstacleSettings) - 4);
        PS_WriteFile((*player)->fileh, &data->obstacleSettings, sizeof(data->obstacleSettings));
    }
}

void audio_bus_mix(tPlayerProfile **player, unsigned char option, signed char value)
{
    tSaveData *data;
    if ((*player)->datah == 0) return;
    data = *(*player)->datah;
    if (option < 21) {
        if (data->options[option] != value) {
            data->options[option] = value;
            data->optChecksum = format_string_parse(data->options, 21);
            PS_WriteFile((*player)->fileh, &data->optChecksum, 25);
        }
    } else if (gTrackNum >= 0 && gTrackNum < 6) voice_stop_2(player, option, value, gTrackNum);
    else if (gTrackNum >= 19 && gTrackNum < 25) voice_stop_2(player, option, value, gTrackNum);
    else if (gTrackNum >= 6 && gTrackNum < 14) _SaveBattleSetting(player, option, value, (unsigned char)(gTrackNum - 6));
    else if (gTrackNum >= 14 && gTrackNum < 18) _SaveStuntSetting(player, option, value, (unsigned char)(gTrackNum - 14));
    else _SaveObstacleSetting(player, option, value, (unsigned char)(gTrackNum - 18));
}
