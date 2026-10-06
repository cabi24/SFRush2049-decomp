/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef float f32;
typedef signed int s32;
typedef unsigned char u8;

typedef struct TrackTimes {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ f32 times[3][5];
    /* 0x40 */ u8 pad40[0x20];
} TrackTimes; /* 0x60 */

typedef struct SaveData {
    /* 0x00 */ u8 pad0[0x8C];
    /* 0x8C */ TrackTimes tracks[12];
} SaveData;

typedef struct SaveSlot {
    /* 0x00 */ SaveData *data;
} SaveSlot;

typedef struct Owner {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ SaveSlot *slot;
} Owner;

typedef struct Entry {
    /* 0x00 */ Owner *owner;
} Entry;

extern TrackTimes D_80150F88[12];
extern Entry *D_80151690[12][3][5];

void draw_text(Entry *entry)
{
    SaveData *data;
    s32 i;
    s32 j;
    s32 k;
    s32 m;
    f32 *src;
    f32 *times;
    Entry **owners;

    if (entry->owner->slot == 0) {
        return;
    }
    data = entry->owner->slot->data;
    for (i = 0; i < 12; i++) {
        for (j = 0; j < 3; j++) {
            src = data->tracks[i].times[j];
            for (k = 0; k < 5; k++) {
                times = D_80150F88[i].times[j];
                if (0.0f < *src) {
                    if (0.0f == times[k] || *src < times[k]) {
                        owners = D_80151690[i][j];
                        for (m = 4; m > k; m--) {
                            times[m] = times[m - 1];
                            owners[m] = owners[m - 1];
                        }
                        times[k] = *src;
                        owners[m] = entry;
                        src++;
                    }
                }
            }
        }
    }
    if (i) {}
}
