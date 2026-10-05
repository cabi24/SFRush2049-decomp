/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Header88 {
    /* 0x00 */ u8 pad0[6];
    /* 0x06 */ s8 player;
    /* 0x07 */ u8 mode;
    /* 0x08 */ u8 pad8[14];
    /* 0x16 */ u8 name[13];
    /* 0x23 */ u8 pad23[5];
    /* 0x28 */ u8 ident[8];
    /* 0x30 */ u8 pad30[8];
    /* 0x38 */ f32 score;
    /* 0x3C */ u32 bytes;
    /* 0x40 */ u8 pad40[12];
    /* 0x4C */ u32 offset;
    /* 0x50 */ u32 head;
    /* 0x54 */ u32 tail;
} Header88; /* 0x58 */

typedef struct Record44 {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s8 player;
    /* 0x09 */ u8 mode;
    /* 0x0A */ u8 name[13];
    /* 0x17 */ u8 pad17;
    /* 0x18 */ u8 ident[8];
    /* 0x20 */ f32 score;
    /* 0x24 */ u32 unk24;
    /* 0x28 */ Header88 **handle;
} Record44; /* 0x2C */


typedef struct OSMesgQueue OSMesgQueue;

extern s8 D_80156994;
extern Record44 **D_8012E6F8;
extern u8 D_8013FECA;
extern s32 D_80152770;

s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osJamMesg(OSMesgQueue *, void *, s32);
void audio_reverb_update(u32 address, s32 tag);
void *func_800A47C0(void *dst, const void *src, u32 n);
void *memcpy(void *, const void *, u32);
void *audio_task_complete(s32, s32);

void func_800CB748(Header88 *entry, void *payload) {
    s32 i;
    Header88 **h;
    s32 pad2;
    s32 pad3;
    Record44 **row;
    s32 pad4;
    s32 pad5;
    u32 old;

    if (D_80156994 == 0) {
        row = D_8012E6F8;
    } else {
        row = &D_8012E6F8[((entry->mode ? 6 : 0) + entry->player) * 3];
    }
    for (i = 0; i < D_8013FECA; i++) {
        h = row[i]->handle;
        if (h != 0 && entry->player == (*h)->player && entry->mode == (*h)->mode && !(entry->score < (*h)->score)) {
            break;
        }
    }
    i--;
    if (i >= 0) {
        old = (u32) row[0]->handle;
        if (old != 0) {
            osRecvMesg((OSMesgQueue *) &D_80152770, 0, 1);
            audio_reverb_update(old, 0);
            osJamMesg((OSMesgQueue *) &D_80152770, 0, 0);
        }
        if (i > 0) {
            func_800A47C0(row[0], row[1], i * sizeof(Record44));
        }
        row[i]->player = entry->player;
        row[i]->mode = entry->mode;
        memcpy(row[i]->name, entry->name, 13);
        memcpy(row[i]->ident, entry->ident, 8);
        row[i]->score = entry->score;
        row[i]->handle = audio_task_complete(0, entry->bytes + sizeof(Header88));
        memcpy(*row[i]->handle, entry, sizeof(Header88));
        memcpy((u8 *) *row[i]->handle + sizeof(Header88), payload, entry->bytes);
        (*row[i]->handle)->offset = 0;
        (*row[i]->handle)->tail = 0;
        (*row[i]->handle)->head = (*row[i]->handle)->tail;
    }
}
