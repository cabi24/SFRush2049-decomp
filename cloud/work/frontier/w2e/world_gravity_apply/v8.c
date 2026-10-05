/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct PathPoint {
    s16 pos[3];
} PathPoint;

typedef struct PathSet {
    /* 0x0 */ u8 type;
    /* 0x1 */ u8 pad1[9];
    /* 0xA */ u16 count;
    /* 0xC */ PathPoint *points;
} PathSet; /* 0x10 */

typedef struct PathGraph {
    /* 0x0 */ u16 count;
    /* 0x4 */ PathPoint *points;
    /* 0x8 */ u8 numSets;
    /* 0xC */ PathSet *sets;
} PathGraph;

typedef struct Track {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ char padA[0x24];
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ char pad30[0x20];
} Track; /* 0x50 */

typedef struct TrackLen {
    /* 0x00 */ char pad0[0x58];
    /* 0x58 */ f32 unk58;
} TrackLen;

typedef struct Model {
    /* 0x000 */ char pad0[0x22C];
    /* 0x22C */ f32 pos[3];
    /* 0x238 */ char pad238[0x58E];
    /* 0x7C6 */ s16 slot;
    /* 0x7C8 */ char pad7C8[0x1A];
    /* 0x7E2 */ s16 unk7E2;
    /* 0x7E4 */ s16 unk7E4;
    /* 0x7E6 */ s16 unk7E6;
    /* 0x7E8 */ s8 unk7E8;
    /* 0x7E9 */ char pad7E9[0x1F];
} Model; /* 0x808 */

typedef struct Car {
    /* 0x000 */ char pad0[0xEF];
    /* 0x0EF */ s8 unkEF;
    /* 0x0F0 */ char padF0[0xA];
    /* 0x0FA */ s16 unkFA;
    /* 0x0FC */ s16 unkFC;
    /* 0x0FE */ s16 unkFE;
    /* 0x100 */ f32 unk100;
    /* 0x104 */ char pad104[0x2B4];
} Car; /* 0x3B8 */

extern s8 D_80152744;
extern Model D_8014A250[];
extern Car D_80152818[];
extern PathGraph D_801407F0;
extern Track D_80151CE8[];
extern f32 D_801543AC;
extern f32 D_80152800;

s32 func_800CF604(s16 slot);
void func_800B9F60(s32 set, s32 index, s32 *outSet, s32 *outIndex);
s32 func_800D3430(s32 set, s32 index, s32 *outSet, s32 *outIndex, s32 flags);
f32 func_80098A54(f32 *v);
f32 audio_channel_alloc(s32 from, s32 to);

void world_gravity_apply(void)
{
    s16 i;
    s32 slot;
    s32 j;
    s16 end;
    s32 k;
    Model *m;
    Model *m2;
    Car *gc;
    s32 idx;
    s32 pad1;
    s32 bestSet;
    s32 bestIdx;
    s32 next;
    s32 pad2;
    s16 pos[3];
    f32 d[3];
    f32 rel[3];
    f32 best;
    f32 dist;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 off;
    PathSet *set;
    PathPoint *p;
    s32 limit;
    s16 last;

    for (i = 0; i < D_80152744; i++) {
        slot = D_8014A250[i].slot;
        m = &D_8014A250[slot];
        m2 = &D_8014A250[slot];
        if (func_800CF604(slot)) {
            gc = &D_80152818[slot];
            if (gc->unkEF == 1) {
                continue;
            }
            pos[0] = m->pos[0];
            pos[1] = m->pos[1];
            pos[2] = m->pos[2];
            best = 1e20f;
            bestIdx = 0;
            bestSet = -1;
            end = 5;
            idx = gc->unkFE;
            for (j = 0; j < end; j++) {
                dx = D_801407F0.points[idx].pos[0] - pos[0];
                dy = D_801407F0.points[idx].pos[1] - pos[1];
                dz = D_801407F0.points[idx].pos[2] - pos[2];
                dist = dx * dx + dy * dy + dz * dz;
                if (dist < best) {
                    best = dist;
                    bestIdx = idx;
                }
                func_800B9F60(-1, idx, 0, &idx);
            }
            if (best > 10000.0f) {
                if (gc->unkFA >= 0) {
                    if (D_801407F0.sets[gc->unkFA].count - gc->unkFC >= 6) {
                        end = gc->unkFC + 5;
                    } else {
                        end = D_801407F0.sets[gc->unkFA].count;
                    }
                    for (j = gc->unkFC; j < end; j++) {
                        dx = D_801407F0.sets[gc->unkFA].points[j].pos[0] - pos[0];
                        dy = D_801407F0.sets[gc->unkFA].points[j].pos[1] - pos[1];
                        dz = D_801407F0.sets[gc->unkFA].points[j].pos[2] - pos[2];
                        dist = dx * dx + dy * dy + dz * dz;
                        if (dist < best) {
                            bestIdx = j;
                            bestSet = gc->unkFA;
                            best = dist;
                        }
                    }
                } else if (gc->unkFC != gc->unkFE) {
                    idx = gc->unkFC;
                    for (j = 0; j < end; j++) {
                        dx = D_801407F0.points[idx].pos[0] - pos[0];
                        dy = D_801407F0.points[idx].pos[1] - pos[1];
                        dz = D_801407F0.points[idx].pos[2] - pos[2];
                        dist = dx * dx + dy * dy + dz * dz;
                        if (dist < best) {
                            best = dist;
                            bestIdx = idx;
                            bestSet = -1;
                        }
                        func_800B9F60(-1, idx, 0, &idx);
                    }
                }
            }
            if (best > 40000.0f) {
                if (m->unk7E4 < m->unk7E2) {
                    end = D_801407F0.count;
                } else {
                    end = D_80151CE8[m->unk7E4].unk2E;
                }
                for (j = D_80151CE8[m->unk7E2].unk2E; j < end; j++) {
                    dx = D_801407F0.points[j].pos[0] - pos[0];
                    dy = D_801407F0.points[j].pos[1] - pos[1];
                    dz = D_801407F0.points[j].pos[2] - pos[2];
                    dist = dx * dx + dy * dy + dz * dz;
                    if (dist < best) {
                        best = dist;
                        bestIdx = j;
                        bestSet = -1;
                        if (dist <= 10000.0f) {
                            break;
                        }
                    }
                }
                if (j == end) {
                    set = D_801407F0.sets;
                    for (k = 0; k < D_801407F0.numSets; k++, set++) {
                        if (set->type == 2) {
                            continue;
                        }
                        for (j = 0; j < set->count; j++) {
                            dx = set->points[j].pos[0] - pos[0];
                            dy = set->points[j].pos[1] - pos[1];
                            dz = set->points[j].pos[2] - pos[2];
                            dist = dx * dx + dy * dy + dz * dz;
                            if (dist < best) {
                                best = dist;
                                bestIdx = j;
                                bestSet = k;
                                if (dist <= 2500.0f) {
                                    break;
                                }
                            }
                        }
                        if (j < set->count) {
                            break;
                        }
                    }
                    if (k == D_801407F0.numSets) {
                        j = gc->unkFE;
                        p = &D_801407F0.points[j];
                        for (;;) {
                            dx = p->pos[0] - pos[0];
                            dy = p->pos[1] - pos[1];
                            dz = p->pos[2] - pos[2];
                            dist = dx * dx + dy * dy + dz * dz;
                            if (dist < best) {
                                best = dist;
                                bestIdx = j;
                                bestSet = -1;
                                if (dist <= 10000.0f) {
                                    break;
                                }
                            }
                            j++;
                            if (j >= D_801407F0.count) {
                                j = 0;
                            }
                            if (j == gc->unkFE) {
                                break;
                            }
                            p = &D_801407F0.points[j];
                        }
                    }
                }
            }
            gc->unkFA = bestSet;
            gc->unkFC = bestIdx;
            if (best > 40000.0f) {
                continue;
            }
            if (bestSet >= 0) {
                func_800D3430(bestSet, bestIdx, &bestSet, &bestIdx, 0);
            }
            if (bestIdx < D_80151CE8[m->unk7E2].unk2E) {
                continue;
            }
            if (m->unk7E4 < m->unk7E2) {
                limit = D_801407F0.count;
            } else {
                limit = D_80151CE8[m->unk7E4].unk2E;
            }
            if (bestIdx >= limit) {
                continue;
            }
            gc->unkFE = bestIdx;
            if (gc->unkFA >= 0) {
                off = 0.0f;
            } else {
                func_800B9F60(-1, bestIdx, 0, &next);
                d[0] = D_801407F0.points[next].pos[0] - D_801407F0.points[bestIdx].pos[0];
                d[1] = D_801407F0.points[next].pos[1] - D_801407F0.points[bestIdx].pos[1];
                d[2] = D_801407F0.points[next].pos[2] - D_801407F0.points[bestIdx].pos[2];
                d[1] = 0.0f;
                func_80098A54(d);
                rel[0] = m->pos[0] - D_801407F0.points[bestIdx].pos[0];
                rel[1] = m->pos[1] - D_801407F0.points[bestIdx].pos[1];
                rel[2] = m->pos[2] - D_801407F0.points[bestIdx].pos[2];
                off = rel[0] * d[0] + rel[2] * d[2];
            }
            gc->unk100 = 0.0f;
            if (m->unk7E8 > 0) {
                gc->unk100 += D_80152800 + (m->unk7E8 - 1) * D_801543AC;
                last = (m->unk7E2 < D_80151CE8[0].unk4) ? D_80151CE8[0].unk8 : m->unk7E2;
                for (idx = D_80151CE8[0].unk4; idx < last; idx++) {
                    gc->unk100 += ((TrackLen *)&D_80151CE8[idx])->unk58;
                }
                if (last != m->unk7E2) {
                    for (idx = D_80151CE8[0].unk2; idx < m2->unk7E2; idx++) {
                        gc->unk100 += ((TrackLen *)&D_80151CE8[idx])->unk58;
                    }
                }
            } else {
                for (idx = 0; idx < m2->unk7E2; idx++) {
                    gc->unk100 += ((TrackLen *)&D_80151CE8[idx])->unk58;
                }
            }
            gc->unk100 += audio_channel_alloc(D_80151CE8[m->unk7E2].unk2E, bestIdx);
            gc->unk100 += off;
        }
    }
}
