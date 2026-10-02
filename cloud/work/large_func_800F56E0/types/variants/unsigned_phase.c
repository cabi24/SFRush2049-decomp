/* Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared */
/*
 * Complete reconstruction from retail instructions at 0x800F56E0 (2072 bytes).
 * Purpose: update resource-local and aggregate per-course race statistics,
 * inserting this player's samples and mean into sorted top-five tables.
 * No corresponding persistence routine was found in the Rush The Rock source.
 * Names below describe observed offsets; unknown members are actual record
 * storage needed to represent the confirmed game layouts.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

typedef struct RaceStats {
    u32 unknown00;
    float samples[5];
    float means[5];
    u8 unknown44[20];
    float sample_sum;
    u16 sample_count;
    u16 finished_count;
    u16 first_count;
    u16 second_count;
    u16 third_count;
    u16 event_count;
    u16 flag_count;
    u16 flag_all_samples_count;
    u32 unknown84;
    u32 distance;
    u16 bits;
    u8 unknown94[2];
} RaceStats;

typedef struct StatsPayload {
    u8 *data;
} StatsPayload;
typedef struct ResourceRoot {
    u32 unknown00[2];
    s32 enabled;
    u8 unknown12[32];
    StatsPayload *payload;
} ResourceRoot;
typedef ResourceRoot **ResourceHandle;
typedef struct PlayerRecord {
    u8 car_index;
    u8 resource_index;
    u8 unknown02[62];
    u16 event_count;
    u8 unknown66[6];
    ResourceHandle resource;
} PlayerRecord;
typedef struct CarRecord {
    u8 unknown00[238];
    s8 place;
    s8 finished;
    u8 unknown240[24];
    float distance;
    u8 unknown268[684];
} CarRecord;

extern s8 D_8014978C;
extern s8 D_80152570;
extern s16 active_player_count;
extern PlayerRecord input_rec0[];
extern ResourceRoot *D_80146150[];
extern u8 D_80144018[];
extern float D_80149A78[][8];
extern RaceStats D_80150F88[];
extern ResourceHandle D_80151690[][15];
extern s8 D_80151AC0[];
extern CarRecord D_80152818[];
extern s8 D_80152744;
extern s8 D_80142760;
extern s16 D_80152734;
extern void func_800CD8EC(ResourceHandle, u8);

void func_800F56E0(void)
{
    s32 course;
    s32 resource_course;
    s32 player;
    u32 phase;
    CarRecord *car;
    s32 lap;
    s32 rank;
    s32 shift;
    float mean;
    RaceStats *stats;
    PlayerRecord *record;

    resource_course = D_8014978C;
    course = resource_course;
    if (D_80152570) {
        resource_course += 19;
        course += 6;
    }
    for (player = 0; player < active_player_count; player++) {
        record = &input_rec0[player];
        if (record->resource == 0) {
            record->resource = &D_80146150[record->resource_index];
        }
        if ((*record->resource)->payload == 0) {
            return;
        }
        for (phase = 0; phase < 2; phase++) {
            if (phase == 0) {
                stats = (RaceStats *)((*record->resource)->payload->data + 140) + course;
            } else {
                stats = &D_80150F88[course];
            }
            car = &D_80152818[record->car_index];
            for (lap = 0; lap < D_80144018[player]; lap++) {
                for (rank = 0; rank < 5; rank++) {
                    if (stats->samples[rank] == 0.0f ||
                        D_80149A78[player][lap] < stats->samples[rank]) {
                        for (shift = 4; shift > rank; shift--) {
                            stats->samples[shift] = stats->samples[shift - 1];
                            if (phase == 1) {
                                D_80151AC0[shift] = D_80151AC0[shift - 1];
                                D_80151690[course][shift] = D_80151690[course][shift - 1];
                            }
                        }
                        stats->samples[rank] = D_80149A78[player][lap];
                        if (phase == 1) {
                            D_80151AC0[rank] = player;
                            if ((*record->resource)->enabled) {
                                D_80151690[course][rank] = record->resource;
                            }
                        }
                        break;
                    }
                }
            }
            if (D_80144018[player] > 0) {
                mean = 0;
                for (shift = 0; shift < D_80144018[player]; shift++) {
                    mean += D_80149A78[player][shift];
                }
                mean /= (u32)D_80144018[player];
                for (rank = 0; rank < 5; rank++) {
                    if (stats->means[rank] == 0.0f || mean < stats->means[rank]) {
                        for (shift = 4; shift > rank; shift--) {
                            stats->means[shift] = stats->means[shift - 1];
                            if (phase == 1) {
                                D_80151AC0[shift + 5] = D_80151AC0[shift + 4];
                                D_80151690[course][shift + 5] = D_80151690[course][shift + 4];
                            }
                        }
                        stats->means[rank] = mean;
                        if (phase == 1) {
                            D_80151AC0[rank + 5] = player;
                            if ((*record->resource)->enabled) {
                                D_80151690[course][rank + 5] = record->resource;
                            }
                        }
                        break;
                    }
                }
            }
            if (car->finished) {
                stats->finished_count++;
                if (car->place == 0 && D_80152744 >= 2) {
                    stats->first_count++;
                } else if (car->place == 1 && D_80152744 >= 3) {
                    stats->second_count++;
                } else if (car->place == 2 && D_80152744 >= 4) {
                    stats->third_count++;
                }
                if (D_80142760) {
                    stats->flag_count++;
                    if (D_80152734 == D_80144018[player]) {
                        stats->flag_all_samples_count++;
                    }
                }
            }
            stats->event_count += record->event_count;
            stats->sample_count += D_80144018[player];
            for (shift = 0; shift < D_80144018[player]; shift++) {
                stats->sample_sum += D_80149A78[player][shift];
            }
            stats->distance += car->distance / 528.0f;
        }
        func_800CD8EC(record->resource, resource_course);
    }
}
