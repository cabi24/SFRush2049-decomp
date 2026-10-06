/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * draw_text (0x800C734C, 556 bytes) -- the name is a historical label.
 *
 * Real semantics: merge one save slot's best times into the global top-5
 * tables.  For 12 tracks x 3 categories, walk the five global ranks k and the
 * slot's own sorted list with a separate cursor l: whenever the slot's next
 * positive time beats (or fills an empty) global rank k, shift the tail of
 * D_80150F88[i].times[j] and of the owner table D_80151690[i][j] down one place,
 * insert the time and the owning entry at k, and advance l.
 * No arcade ancestor found (N64 save/records code).
 *
 * Shaping (wave 10, w10b; earlier best 103/139 words, w2c):
 *  - the slot list is read through an index l (src[l], l++), not a walking
 *    pointer: the extra induction web raises the interference degree of the
 *    loop-invariant webs (bounds 4/5/60/12, &D_80150F88, the j*20 and
 *    srcbase IVs) to 22, so uopt colours them in its priority phase into
 *    a1-a3/t0-t3 as retail does, instead of last in phase two (s4-s8);
 *  - times/owners are assigned at the j level before the k loop: phase-two
 *    webs are coloured in order of first appearance, and retail has times (t4)
 *    and owners (t5) before k (s0).
 * Also matches standalone at -O2.
 */
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
    s32 l;
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
            l = 0;
            times = D_80150F88[i].times[j];
            owners = D_80151690[i][j];
            for (k = 0; k < 5; k++) {
                if (0.0f < src[l]) {
                    if (0.0f == times[k] || src[l] < times[k]) {
                        for (m = 4; m > k; m--) {
                            times[m] = times[m - 1];
                            owners[m] = owners[m - 1];
                        }
                        times[k] = src[l];
                        owners[m] = entry;
                        l++;
                    }
                }
            }
        }
    }
}
